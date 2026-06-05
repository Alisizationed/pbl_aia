from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session

from auth.users import get_current_user
from database.database import SessionLocal
from models.models import Edge, NetworkGraph, Node
from repositories.network_repository import NetworkRepository

router = APIRouter(prefix="/network", tags=["Network"])


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@router.get("/graph", response_model=NetworkGraph)
def get_network_graph(
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
    ):
    nodes = [
        Node(id=node.id, name=node.name)
        for node in NetworkRepository.get_all_nodes(db)
    ]
    edges = [
        Edge(
            id=edge.id,
            from_node_id=edge.from_node_id,
            to_node_id=edge.to_node_id,
            cost=edge.cost,
            distance=edge.distance,
            capacity=edge.capacity,
            time=edge.time,
        )
        for edge in NetworkRepository.get_all_edges(db)
    ]

    return NetworkGraph(nodes=nodes, edges=edges)