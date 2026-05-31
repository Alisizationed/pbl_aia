import random
from datetime import datetime, timedelta
from typing import List

from repositories.network_repository import NetworkRepository
from models.models import Route, Edge


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

        fronts: List[List[Route]] = []
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

            obj_range = (
                getattr(sorted_front[-1], objective) -
                getattr(sorted_front[0], objective)
            ) or 1e-9

            for i in range(1, n - 1):
                distances[id(sorted_front[i])] += (
                    getattr(sorted_front[i + 1], objective) -
                    getattr(sorted_front[i - 1], objective)
                ) / obj_range

        return distances

    @staticmethod
    def tournament_select(front: List[Route], distances: dict) -> Route:
        a, b = random.sample(front, 2)
        return a if distances[id(a)] >= distances[id(b)] else b

    @staticmethod
    def generate_path(
            start,
            end,
            departure_time,
            route: Route,
            visited: set,
            total_weight: float,
            db) -> Route | None:

        if start == end:
            return route

        edges = NetworkRepository.get_edges_at(db, departure_time)
        random.shuffle(edges)

        for edge in edges:
            if edge.from_node_id != start:
                continue
            if edge.to_node_id in visited:
                continue
            if edge.capacity < total_weight:
                continue

            visited.add(edge.to_node_id)
            route.path.append(Edge(
                id=edge.id,
                from_node_id=edge.from_node_id,
                to_node_id=edge.to_node_id,
                cost=edge.cost,
                distance=edge.distance,
                capacity=edge.capacity,
                time=edge.time
            ))
            route.cost += edge.cost
            route.distance += edge.distance
            route.time += edge.time

            arrival_time = departure_time + timedelta(hours=edge.time)

            result = OptimalRouteService.generate_path(
                edge.to_node_id,
                end,
                arrival_time,
                route,
                visited,
                total_weight,
                db
            )

            if result is not None:
                return result

            last = route.path.pop()
            route.cost -= last.cost
            route.distance -= last.distance
            route.time -= last.time
            visited.remove(edge.to_node_id)

        return None

    @staticmethod
    def mutate(
        path: List[Edge],
        end,
        departure_time,
        total_weight: float,
        db
    ) -> Route | None:
        if not path:
            return None

        cut = random.randint(0, len(path) - 1)
        kept_edges = path[:cut]

        new_route = Route(path=kept_edges, cost=0.0, time=0.0, distance=0.0)
        for edge in kept_edges:
            new_route.cost += edge.cost
            new_route.distance += edge.distance
            new_route.time += edge.time

        cut_departure = departure_time
        for edge in kept_edges:
            cut_departure += timedelta(hours=edge.time)

        start_id = kept_edges[-1].to_node_id if kept_edges else path[0].from_node_id
        visited = {e.from_node_id for e in kept_edges} | {start_id}

        return OptimalRouteService.generate_path(
            start_id,
            end,
            cut_departure,
            new_route,
            visited,
            total_weight,
            db
        )

    @staticmethod
    def crossover(
            route1: Route,
            route2: Route,
            end,
            departure_time,
            total_weight: float,
            db
    ) -> Route | None:
        nodes1 = {edge.to_node_id for edge in route1.path[:-1]}
        nodes2 = {edge.from_node_id for edge in route2.path[1:]}
        intersection = nodes1.intersection(nodes2)

        if not intersection:
            return OptimalRouteService.mutate(
                route1.path, end, departure_time, total_weight, db
            )

        shared_node = random.choice(list(intersection))

        cut1 = next(i + 1 for i, e in enumerate(route1.path) if e.to_node_id == shared_node)
        cut2 = next(i for i, e in enumerate(route2.path) if e.from_node_id == shared_node)

        new_path = route1.path[:cut1] + route2.path[cut2:]
        new_route = Route(path=new_path, cost=0.0, time=0.0, distance=0.0)
        for edge in new_path:
            new_route.cost += edge.cost
            new_route.distance += edge.distance
            new_route.time += edge.time

        return new_route

    @staticmethod
    def scm(
            selection_list: List[List[Route]],
            end,
            departure_time,
            total_weight: float,
            db
    ) -> List[Route]:
        offspring = []

        for front in selection_list:
            for i in range(len(front) - 1):
                child = OptimalRouteService.crossover(
                    front[i],
                    front[i + 1],
                    end,
                    departure_time,
                    total_weight,
                    db
                )
                if child is not None:
                    offspring.append(child)

        return offspring

    @staticmethod
    def nsga_ii(
            start,
            end,
            total_weight: float,
            departure_time: datetime,
            db,
            pop_size: int = 50,
            generations: int = 100
    ) -> List[Route]:

        population: List[Route] = []
        attempts = 0
        while len(population) < pop_size and attempts < pop_size * 3:
            route = OptimalRouteService.generate_path(
                start.id,
                end.id,
                departure_time,
                Route(path=[], cost=0.0, time=0.0, distance=0.0),
                {start.id},
                total_weight,
                db
            )
            if route is not None:
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
                end,
                departure_time,
                total_weight,
                db
            )

            combined = population + offspring
            combined_fronts = OptimalRouteService.non_dominated_sort(combined)

            next_population: List[Route] = []
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

        sorted_trains = sorted(
            distributed_trains.items(),
            key=lambda item: (
                item[1][0].capacity - item[1][0].used_weight -
                sum(c.weight for c in item[1][1])
            )
        )

        results = {}
        for train_id, (train, carriages) in sorted_trains:
            total_weight = train.used_weight + sum(c.weight for c in carriages)

            routes = OptimalRouteService.nsga_ii(
                start,
                end,
                total_weight,
                departure_time,
                db
            )

            results[train_id] = routes

        return results