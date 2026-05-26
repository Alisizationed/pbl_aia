from pydantic import BaseModel
from typing import List
from datetime import datetime


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


class TimeWindow(BaseModel):
    valid_from: datetime
    valid_until: datetime


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
    departure_time: datetime