from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session

from database.database import SessionLocal
from models.models import PathRequest

from services.carriage_service import CarriageService
from services.optimal_route_service import OptimalRouteService

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

    distributed = (
        CarriageService.distribute_carriages(
            request.trains,
            request.carriages
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