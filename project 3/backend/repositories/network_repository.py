from datetime import datetime
from sqlalchemy.orm import Session

from models.db_models import NetworkNode, NetworkEdge, EdgeTimeWindow, Carriage, Train
from models.models import Edge


class NetworkRepository:

    @staticmethod
    def get_all_nodes(db: Session):
        return db.query(NetworkNode).all()

    @staticmethod
    def get_all_edges(db: Session):
        return db.query(NetworkEdge).all()

    @staticmethod
    def get_node_by_id(db: Session, node_id: int):
        return db.query(NetworkNode).filter(NetworkNode.id == node_id).first()

    from models.models import Edge

    @staticmethod
    def get_edges_at(db: Session, departure_time: datetime) -> list[Edge]:
        db_edges = db.query(NetworkEdge).all()  # или с фильтром по времени
        return [
            Edge(
                id=e.id,
                from_node_id=e.from_node_id,
                to_node_id=e.to_node_id,
                cost=e.cost,
                distance=e.distance,
                capacity=e.capacity,
                time=e.time
            )
            for e in db_edges
        ]

    @staticmethod
    def get_carriages_by_ids(db: Session, carriage_ids: list[int]):
        return (
            db.query(Carriage)
            .filter(Carriage.id.in_(carriage_ids))
            .all()
        )

    @staticmethod
    def get_trains_by_ids(db: Session, train_ids: list[int]):
        return (
            db.query(Train)
            .filter(Train.id.in_(train_ids))
            .all()
        )