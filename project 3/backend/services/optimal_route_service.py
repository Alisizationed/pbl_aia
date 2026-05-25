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
            distributed_trains
    ):
        routes: List[Route] = []

        # Implement NSGA II to find optimal routes

        return routes


    @staticmethod
    def find_optimal_routes(
        start,
        end,
        distributed_trains,
        db
    ):

        nodes = NetworkRepository.get_all_nodes(db)
        edges = NetworkRepository.get_all_edges(db)

        return OptimalRouteService.nsga_ii(
            start,
            end,
            nodes,
            edges,
            distributed_trains
        )