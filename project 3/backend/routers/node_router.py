from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from database.database import SessionLocal
from models.db_models import NetworkNode

from auth.users import get_current_user
from auth.roles import require_role

router = APIRouter(prefix="/nodes", tags=["Nodes"])


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@router.get("/")
def get_nodes(
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    return db.query(NetworkNode).all()


@router.get("/{node_id}")
def get_node(
        node_id: int,
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    node = db.query(NetworkNode).filter(NetworkNode.id == node_id).first()

    if node is None:
        raise HTTPException(status_code=404, detail="Node not found")

    return node


@router.post("/")
def create_node(
        name: str,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    node = NetworkNode(name=name)

    db.add(node)
    db.commit()
    db.refresh(node)

    return node


@router.put("/{node_id}")
def update_node(
        node_id: int,
        name: str,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    node = db.query(NetworkNode).filter(NetworkNode.id == node_id).first()

    if node is None:
        raise HTTPException(status_code=404, detail="Node not found")

    node.name = name

    db.commit()
    db.refresh(node)

    return node


@router.delete("/{node_id}")
def delete_node(
        node_id: int,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    node = db.query(NetworkNode).filter(NetworkNode.id == node_id).first()

    if node is None:
        raise HTTPException(status_code=404, detail="Node not found")

    db.delete(node)
    db.commit()

    return {"message": "Node deleted successfully"}
