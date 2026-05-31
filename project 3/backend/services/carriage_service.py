from collections import defaultdict
from models.models import Carriage, Train


class CarriageService:

    @staticmethod
    def distribute_carriages(trains: list[Train], carriages: list[Carriage]):
        distributed: dict[int, tuple[Train, list[Carriage]]] = None

        return distributed