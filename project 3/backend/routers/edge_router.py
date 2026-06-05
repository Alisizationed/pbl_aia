from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from database.database import SessionLocal
from models.db_models import NetworkEdge

from auth.users import get_current_user
from auth.roles import require_role

router = APIRouter(prefix="/edges", tags=["Edges"])


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@router.get("/")
def get_edges(
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    return db.query(NetworkEdge).all()


@router.get("/{edge_id}")
def get_edge(
        edge_id: int,
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    edge = db.query(NetworkEdge).filter(NetworkEdge.id == edge_id).first()

    if edge is None:
        raise HTTPException(status_code=404, detail="Edge not found")

    return edge


@router.post("/")
def create_edge(
        from_node_id: int,
        to_node_id: int,
        cost: float,
        distance: float,
        capacity: int,
        time: float,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    edge = NetworkEdge(
        from_node_id=from_node_id,
        to_node_id=to_node_id,
        cost=cost,
        distance=distance,
        capacity=capacity,
        time=time
    )

    db.add(edge)
    db.commit()
    db.refresh(edge)

    return edge


@router.put("/{edge_id}")
def update_edge(
        edge_id: int,
        from_node_id: int,
        to_node_id: int,
        cost: float,
        distance: float,
        capacity: int,
        time: float,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    edge = db.query(NetworkEdge).filter(NetworkEdge.id == edge_id).first()

    if edge is None:
        raise HTTPException(status_code=404, detail="Edge not found")

    edge.from_node_id = from_node_id
    edge.to_node_id = to_node_id
    edge.cost = cost
    edge.distance = distance
    edge.capacity = capacity
    edge.time = time

    db.commit()
    db.refresh(edge)

    return edge


@router.delete("/{edge_id}")
def delete_edge(
        edge_id: int,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    edge = db.query(NetworkEdge).filter(NetworkEdge.id == edge_id).first()

    if edge is None:
        raise HTTPException(status_code=404, detail="Edge not found")

    db.delete(edge)
    db.commit()

    return {"message": "Edge deleted successfully"}
