from pydantic import BaseModel
from typing import List


class Node(BaseModel):
    id: int
    name: str


class Carriage(BaseModel):
    id: int
    weight: float


class Train(BaseModel):
    id: int
    capacity: float
    used_weight: float


class Route(BaseModel):
    path: List[Node]
    cost: float
    time: int
    distance: float


class PathRequest(BaseModel):
    start: Node
    end: Node
    trains: List[Train]
    carriages: List[Carriage]