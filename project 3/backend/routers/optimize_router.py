from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from database.database import SessionLocal
from models.models import PathRequest, Carriage, Train
from services.carriage_service import CarriageService
from services.optimal_route_service import OptimalRouteService
from repositories.network_repository import NetworkRepository

router = APIRouter()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@router.post("/optimize")
async def optimize(request: PathRequest, db: Session = Depends(get_db)):
    db_carriages = NetworkRepository.get_carriages_by_ids(db, request.carriage_ids)
    carriages = [Carriage(id=c.id, weight=c.weight) for c in db_carriages]

    db_trains = NetworkRepository.get_trains_by_ids(db, request.train_ids)
    trains = [Train(id=t.id, capacity=t.capacity, used_weight=t.used_weight) for t in db_trains]

    fixed_distribution = CarriageService.distribute_carriages(trains, carriages)
    if not fixed_distribution:
        raise HTTPException(status_code=400, detail="Could not distribute carriages")

    departure_time = request.departure_time.replace(tzinfo=None)

    ensembles = OptimalRouteService.generate_random_ensembles(
        start=request.start,
        end=request.end,
        fixed_distribution=fixed_distribution,
        departure_time=departure_time,
        db=db,
        num_ensembles=10,
        routes_per_train=10
    )

    if not ensembles:
        raise HTTPException(status_code=404, detail="No valid ensembles found")

    return {"ensembles": ensembles}