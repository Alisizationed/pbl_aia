import random
from datetime import datetime, timedelta
from typing import List, Dict, Tuple

from repositories.network_repository import NetworkRepository
from models.models import Route, Edge
from database.database import SessionLocal
from models.db_models import EdgeTimeWindow 


class OptimalRouteService:

    @staticmethod
    def dominates(a: Route, b: Route) -> bool:
        return (
            a.cost <= b.cost and
            a.distance <= b.distance and
            a.time <= b.time and
            (a.cost < b.cost or a.distance < b.distance or a.time < b.time)
        )

    @staticmethod
    def non_dominated_sort(routes: List[Route]) -> List[List[Route]]:
        dominated_by_count = {id(r): 0 for r in routes}
        dominates_map = {id(r): [] for r in routes}
        route_by_id = {id(r): r for r in routes}

        for route in routes:
            for other in routes:
                if route is other:
                    continue
                if OptimalRouteService.dominates(route, other):
                    dominates_map[id(route)].append(id(other))
                elif OptimalRouteService.dominates(other, route):
                    dominated_by_count[id(route)] += 1

        fronts = []
        current_front_ids = [id(r) for r in routes if dominated_by_count[id(r)] == 0]

        while current_front_ids:
            fronts.append([route_by_id[rid] for rid in current_front_ids])
            next_front_ids = []
            for rid in current_front_ids:
                for dominated_id in dominates_map[rid]:
                    dominated_by_count[dominated_id] -= 1
                    if dominated_by_count[dominated_id] == 0:
                        next_front_ids.append(dominated_id)
            current_front_ids = next_front_ids

        return fronts

    @staticmethod
    def crowding_distance(front: List[Route]) -> dict:
        distances = {id(r): 0.0 for r in front}
        n = len(front)

        if n <= 2:
            for r in front:
                distances[id(r)] = float("inf")
            return distances

        for objective in ["cost", "distance", "time"]:
            sorted_front = sorted(front, key=lambda r: getattr(r, objective))
            distances[id(sorted_front[0])] = float("inf")
            distances[id(sorted_front[-1])] = float("inf")
            obj_range = (getattr(sorted_front[-1], objective) - getattr(sorted_front[0], objective)) or 1e-9
            for i in range(1, n - 1):
                distances[id(sorted_front[i])] += (
                    getattr(sorted_front[i + 1], objective) - getattr(sorted_front[i - 1], objective)
                ) / obj_range

        return distances

    @staticmethod
    def tournament_select(front: List[Route], distances: dict) -> Route:
        a, b = random.sample(front, 2)
        return a if distances[id(a)] >= distances[id(b)] else b

    @staticmethod
    def is_edge_available(edge, arrival_time: datetime, time_windows: Dict[int, List[Tuple[datetime, datetime]]]) -> bool:
        """Проверяет, попадает ли arrival_time в одно из временных окон ребра."""
        windows = time_windows.get(edge.id, [])
        if not windows:
            return True  # если окон нет, ребро доступно всегда
        for valid_from, valid_until in windows:
            if valid_from <= arrival_time <= valid_until:
                return True
        return False

    @staticmethod
    def generate_path(
            start,
            end,
            departure_time: datetime,
            route: Route,
            visited: set,
            total_weight: float,
            db,
            all_edges: list,
            time_windows: Dict[int, List[Tuple[datetime, datetime]]]
    ) -> Route | None:

        stack = [(start, route, visited, departure_time, 0)]
        while stack:
            node, cur_route, cur_visited, cur_time, next_edge_idx = stack[-1]
            available = [e for e in all_edges if e.from_node_id == node]
            random.shuffle(available)
            if next_edge_idx >= len(available):
                stack.pop()
                continue

            edge = available[next_edge_idx]
            stack[-1] = (node, cur_route, cur_visited, cur_time, next_edge_idx + 1)

            if edge.to_node_id in cur_visited:
                continue
            if edge.capacity < total_weight:
                continue

            arrival_time = cur_time + timedelta(hours=edge.time)
            if not OptimalRouteService.is_edge_available(edge, arrival_time, time_windows):
                continue

            from models.models import Edge as EdgeModel
            edge_model = EdgeModel(
                id=edge.id,
                from_node_id=edge.from_node_id,
                to_node_id=edge.to_node_id,
                cost=edge.cost,
                distance=edge.distance,
                capacity=edge.capacity,
                time=edge.time
            )

            new_route = Route(
                path=cur_route.path + [edge_model],
                cost=cur_route.cost + edge_model.cost,
                distance=cur_route.distance + edge_model.distance,
                time=cur_route.time + edge_model.time
            )
            new_visited = cur_visited | {edge.to_node_id}

            if edge.to_node_id == end:
                return new_route
            stack.append((edge.to_node_id, new_route, new_visited, arrival_time, 0))

        return None

    @staticmethod
    def mutate(
        path: List[Edge],
        start_node_id,
        end,
        departure_time: datetime,
        total_weight: float,
        db,
        all_edges: list,
        time_windows: Dict[int, List[Tuple[datetime, datetime]]]
    ) -> Route | None:
        if not path:
            return None

        cut = random.randint(0, len(path) - 1)
        kept_edges = path[:cut]

        new_route = Route(path=list(kept_edges), cost=0.0, time=0.0, distance=0.0)
        for edge in kept_edges:
            new_route.cost += edge.cost
            new_route.distance += edge.distance
            new_route.time += edge.time

        cut_departure = departure_time
        for edge in kept_edges:
            cut_departure += timedelta(hours=edge.time)

        current_start = kept_edges[-1].to_node_id if kept_edges else start_node_id
        visited = {e.from_node_id for e in kept_edges} | {current_start, start_node_id}

        return OptimalRouteService.generate_path(
            current_start,
            end,
            cut_departure,
            new_route,
            visited,
            total_weight,
            db,
            all_edges,
            time_windows
        )

    @staticmethod
    def crossover(
            route1: Route,
            route2: Route,
            start_node_id,
            end,
            departure_time: datetime,
            total_weight: float,
            db,
            all_edges: list,
            time_windows: Dict[int, List[Tuple[datetime, datetime]]]
    ) -> Route | None:
        if not route1.path or not route2.path:
            return None

        nodes1 = {edge.to_node_id for edge in route1.path[:-1]}
        nodes2 = {edge.from_node_id for edge in route2.path[1:]}
        intersection = nodes1.intersection(nodes2)

        if not intersection:
            return route1 if random.random() < 0.5 else route2

        shared_node = random.choice(list(intersection))
        try:
            cut1 = next(i + 1 for i, e in enumerate(route1.path) if e.to_node_id == shared_node)
            cut2 = next(i for i, e in enumerate(route2.path) if e.from_node_id == shared_node)
        except StopIteration:
            return None

        new_path = route1.path[:cut1] + route2.path[cut2:]

        visited_nodes = set()
        for edge in new_path:
            if edge.from_node_id in visited_nodes:
                return None
            visited_nodes.add(edge.from_node_id)

        new_route = Route(path=new_path, cost=0.0, time=0.0, distance=0.0)
        for edge in new_path:
            new_route.cost += edge.cost
            new_route.distance += edge.distance
            new_route.time += edge.time

        cur_time = departure_time
        for edge in new_path:
            arrival_time = cur_time + timedelta(hours=edge.time)
            if not OptimalRouteService.is_edge_available(edge, arrival_time, time_windows):
                return None
            cur_time = arrival_time

        return new_route

    @staticmethod
    def scm(
            selection_list: List[List[Route]],
            start_node_id,
            end,
            departure_time: datetime,
            total_weight: float,
            db,
            all_edges: list,
            time_windows: Dict[int, List[Tuple[datetime, datetime]]]
    ) -> List[Route]:
        offspring = []
        max_offspring = 20

        for front in selection_list:
            if len(front) < 2:
                continue
            for _ in range(min(30, len(front) * 2)):
                i, j = random.sample(range(len(front)), 2)
                child = OptimalRouteService.crossover(
                    front[i],
                    front[j],
                    start_node_id,
                    end,
                    departure_time,
                    total_weight,
                    db,
                    all_edges,
                    time_windows
                )
                if child is not None:
                    offspring.append(child)
                    if len(offspring) >= max_offspring:
                        return offspring
        return offspring

    @staticmethod
    def nsga_ii(
            start,
            end,
            total_weight: float,
            departure_time: datetime,
            db,
            pop_size: int = 3,
            generations: int = 100
    ) -> List[Route]:

        all_edges = NetworkRepository.get_edges_at(db, departure_time)

        time_windows: Dict[int, List[Tuple[datetime, datetime]]] = {}
        db_local = SessionLocal()
        try:
            windows = db_local.query(EdgeTimeWindow).all()
            for w in windows:
                if w.edge_id not in time_windows:
                    time_windows[w.edge_id] = []
                time_windows[w.edge_id].append((w.valid_from, w.valid_until))
        finally:
            db_local.close()

        random.shuffle(all_edges)

        population = []
        attempts = 0
        while len(population) < pop_size and attempts < pop_size * 20:
            route = OptimalRouteService.generate_path(
                start.id,
                end.id,
                departure_time,
                Route(path=[], cost=0.0, time=0.0, distance=0.0),
                {start.id},
                total_weight,
                db,
                all_edges,
                time_windows
            )
            if route is not None:
                if random.random() < 0.3:
                    mutated = OptimalRouteService.mutate(
                        route.path, start.id, end.id, departure_time,
                        total_weight, db, all_edges, time_windows
                    )
                    if mutated is not None:
                        route = mutated
                population.append(route)
            attempts += 1

        if not population:
            return []

        for _ in range(generations):
            fronts = OptimalRouteService.non_dominated_sort(population)

            selection_list = []
            for front in fronts:
                distances = OptimalRouteService.crowding_distance(front)
                sorted_front = sorted(front, key=lambda r: distances[id(r)], reverse=True)
                selection_list.append(sorted_front)

            offspring = OptimalRouteService.scm(
                selection_list,
                start.id,
                end.id,
                departure_time,
                total_weight,
                db,
                all_edges,
                time_windows
            )

            for i in range(len(offspring)):
                if random.random() < 0.2:
                    mutated = OptimalRouteService.mutate(
                        offspring[i].path, start.id, end.id, departure_time,
                        total_weight, db, all_edges, time_windows
                    )
                    if mutated is not None:
                        offspring[i] = mutated

            combined = population + offspring
            combined_fronts = OptimalRouteService.non_dominated_sort(combined)

            next_population = []
            for front in combined_fronts:
                if len(next_population) + len(front) <= pop_size:
                    next_population.extend(front)
                else:
                    distances = OptimalRouteService.crowding_distance(front)
                    remaining = pop_size - len(next_population)
                    sorted_front = sorted(
                        front,
                        key=lambda r: distances[id(r)],
                        reverse=True
                    )
                    next_population.extend(sorted_front[:remaining])
                    break

            population = next_population

        return OptimalRouteService.non_dominated_sort(population)[0]

    @staticmethod
    def find_optimal_routes(
        start,
        end,
        distributed_trains: dict[int, tuple],
        departure_time: datetime,
        db
    ) -> dict[int, List[Route]]:

        results = {}
        for train_id, (train, carriages) in distributed_trains.items():
            total_weight = train.used_weight + sum(c.weight for c in carriages)
            routes = OptimalRouteService.nsga_ii(
                start,
                end,
                total_weight,
                departure_time,
                db
            )
            if routes:
                results[train_id] = routes
        return results