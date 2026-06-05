from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from auth.roles import require_role
from auth.users import get_current_user
from database.database import SessionLocal
from models.db_models import Train

router = APIRouter(prefix="/trains", tags=["Trains"])


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@router.get("/")
def get_trains(
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    return db.query(Train).all()


@router.get("/{train_id}")
def get_train(
        train_id: int,
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    train = db.query(Train).filter(Train.id == train_id).first()

    if train is None:
        raise HTTPException(status_code=404, detail="Train not found")

    return train


@router.post("/")
def create_train(
        capacity: float,
        used_weight: float = 0,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    train = Train(capacity=capacity, used_weight=used_weight)

    db.add(train)
    db.commit()
    db.refresh(train)

    return train


@router.put("/{train_id}")
def update_train(
        train_id: int,
        capacity: float,
        used_weight: float,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    train = db.query(Train).filter(Train.id == train_id).first()

    if train is None:
        raise HTTPException(status_code=404, detail="Train not found")

    train.capacity = capacity
    train.used_weight = used_weight

    db.commit()
    db.refresh(train)

    return train


@router.delete("/{train_id}")
def delete_train(
        train_id: int,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    train = db.query(Train).filter(Train.id == train_id).first()

    if train is None:
        raise HTTPException(status_code=404, detail="Train not found")

    db.delete(train)
    db.commit()

    return {"message": "Train deleted successfully"}
