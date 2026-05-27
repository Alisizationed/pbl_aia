from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session

from database.database import SessionLocal
from models.models import PathRequest, Carriage

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

    distributed = (
        CarriageService.distribute_carriages(
            request.trains,
            carriages
        )
    )

    routes = (
        OptimalRouteService.find_optimal_routes(
            request.start,
            request.end,
            distributed,
            request.departure_time,
            db
        )
    )

    return routes