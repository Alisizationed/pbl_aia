from datetime import datetime

from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from database.database import SessionLocal
from models.db_models import EdgeTimeWindow

from auth.users import get_current_user
from auth.roles import require_role

router = APIRouter(prefix="/time-windows", tags=["Time Windows"])


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@router.get("/")
def get_time_windows(
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    return db.query(EdgeTimeWindow).all()


@router.get("/{time_window_id}")
def get_time_window(
        time_window_id: int,
        db: Session = Depends(get_db),
        _user=Depends(get_current_user)
):
    time_window = (
        db.query(EdgeTimeWindow)
        .filter(EdgeTimeWindow.id == time_window_id)
        .first()
    )

    if time_window is None:
        raise HTTPException(status_code=404, detail="Time window not found")

    return time_window


@router.post("/")
def create_time_window(
        edge_id: int,
        valid_from: datetime,
        valid_until: datetime,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    time_window = EdgeTimeWindow(
        edge_id=edge_id,
        valid_from=valid_from,
        valid_until=valid_until
    )

    db.add(time_window)
    db.commit()
    db.refresh(time_window)

    return time_window


@router.put("/{time_window_id}")
def update_time_window(
        time_window_id: int,
        edge_id: int,
        valid_from: datetime,
        valid_until: datetime,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    time_window = (
        db.query(EdgeTimeWindow)
        .filter(EdgeTimeWindow.id == time_window_id)
        .first()
    )

    if time_window is None:
        raise HTTPException(status_code=404, detail="Time window not found")

    time_window.edge_id = edge_id
    time_window.valid_from = valid_from
    time_window.valid_until = valid_until

    db.commit()
    db.refresh(time_window)

    return time_window


@router.delete("/{time_window_id}")
def delete_time_window(
        time_window_id: int,
        db: Session = Depends(get_db),
        _user=Depends(require_role("admin"))
):
    time_window = (
        db.query(EdgeTimeWindow)
        .filter(EdgeTimeWindow.id == time_window_id)
        .first()
    )

    if time_window is None:
        raise HTTPException(status_code=404, detail="Time window not found")

    db.delete(time_window)
    db.commit()

    return {"message": "Time window deleted successfully"}
