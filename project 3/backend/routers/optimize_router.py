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
async def optimize(
    request: PathRequest,
    db: Session = Depends(get_db)
):
    db_carriages = NetworkRepository.get_carriages_by_ids(db, request.carriage_ids)
    carriages = [Carriage(id=c.id, weight=c.weight) for c in db_carriages]

    db_trains = NetworkRepository.get_trains_by_ids(db, request.train_ids)
    trains = [Train(id=t.id, capacity=t.capacity, used_weight=t.used_weight) for t in db_trains]

    distributed = CarriageService.distribute_carriages(trains, carriages)

    if not distributed:
        raise HTTPException(status_code=400, detail="Could not distribute carriages across trains")

    departure_time = request.departure_time.replace(tzinfo=None)

    routes_per_train = OptimalRouteService.find_optimal_routes(
        request.start,
        request.end,
        distributed,
        departure_time,
        db
    )

    if not routes_per_train:
        raise HTTPException(status_code=404, detail="No valid routes found")

    with open("file.txt", "w") as f:
        f.write(f"{routes_per_train}")

    return {
        train_id: [route.model_dump() for route in route_list]
        for train_id, route_list in routes_per_train.items()
    }