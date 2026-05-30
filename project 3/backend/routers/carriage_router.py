from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from database.database import SessionLocal
from models.db_models import Carriage

router = APIRouter(prefix="/carriages", tags=["Carriages"])


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@router.get("/")
def get_carriages(db: Session = Depends(get_db)):
    return db.query(Carriage).all()


@router.get("/{carriage_id}")
def get_carriage(carriage_id: int, db: Session = Depends(get_db)):
    carriage = db.query(Carriage).filter(Carriage.id == carriage_id).first()

    if carriage is None:
        raise HTTPException(status_code=404, detail="Carriage not found")

    return carriage


@router.post("/")
def create_carriage(weight: float, db: Session = Depends(get_db)):
    carriage = Carriage(weight=weight)

    db.add(carriage)
    db.commit()
    db.refresh(carriage)

    return carriage


@router.put("/{carriage_id}")
def update_carriage(carriage_id: int, weight: float, db: Session = Depends(get_db)):
    carriage = db.query(Carriage).filter(Carriage.id == carriage_id).first()

    if carriage is None:
        raise HTTPException(status_code=404, detail="Carriage not found")

    carriage.weight = weight

    db.commit()
    db.refresh(carriage)

    return carriage


@router.delete("/{carriage_id}")
def delete_carriage(carriage_id: int, db: Session = Depends(get_db)):
    carriage = db.query(Carriage).filter(Carriage.id == carriage_id).first()

    if carriage is None:
        raise HTTPException(status_code=404, detail="Carriage not found")

    db.delete(carriage)
    db.commit()

    return {"message": "Carriage deleted successfully"}