from datetime import datetime
from typing import List

from models.models import Route
from repositories.network_repository import NetworkRepository


class OptimalRouteService:

    @staticmethod
    def nsga_ii(
            start,
            end,
            nodes,
            edges,
            distributed_trains,
            departure_time: datetime
    ):
        routes: List[Route] = []

        # Implement NSGA-II to find optimal routes
        # Use departure_time to track estimated arrival time at each node,
        # and check edge.time_windows to verify the edge is valid at that moment:
        #   any(w.valid_from <= arrival_time <= w.valid_until for w in edge.time_windows)

        return routes

    @staticmethod
    def find_optimal_routes(
        start,
        end,
        distributed_trains,
        departure_time: datetime,
        db
    ):
        nodes = NetworkRepository.get_all_nodes(db)
        edges = NetworkRepository.get_all_edges(db)

        return OptimalRouteService.nsga_ii(
            start,
            end,
            nodes,
            edges,
            distributed_trains,
            departure_time
        )