from pydantic import BaseModel
from typing import List
from datetime import datetime


class Node(BaseModel):
    id: int
    name: str


class Edge(BaseModel):
    id: int
    from_node_id: int
    to_node_id: int
    cost: float
    distance: float
    capacity: int
    time: float

class NetworkGraph(BaseModel):
    nodes: List[Node]
    edges: List[Edge]

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
    path: List[Edge]
    cost: float
    time: float
    distance: float


class PathRequest(BaseModel):
    start: Node
    end: Node
    train_ids: List[int]
    carriage_ids: List[int]
    departure_time: datetime
