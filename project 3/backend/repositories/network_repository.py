from datetime import datetime
from sqlalchemy.orm import Session

from models.db_models import NetworkNode, NetworkEdge, EdgeTimeWindow


class NetworkRepository:

    @staticmethod
    def get_all_nodes(db: Session):
        return db.query(NetworkNode).all()

    @staticmethod
    def get_all_edges(db: Session):
        return db.query(NetworkEdge).all()

    @staticmethod
    def get_edges_at(db: Session, timestamp: datetime):
        return (
            db.query(NetworkEdge)
            .join(EdgeTimeWindow)
            .filter(
                EdgeTimeWindow.valid_from <= timestamp,
                EdgeTimeWindow.valid_until >= timestamp
            )
            .all()
        )