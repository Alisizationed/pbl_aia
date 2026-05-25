from sqlalchemy.orm import Session

from models.db_models import NetworkNode, NetworkEdge


class NetworkRepository:

    @staticmethod
    def get_all_nodes(db: Session):
        return db.query(NetworkNode).all()

    @staticmethod
    def get_all_edges(db: Session):
        return db.query(NetworkEdge).all()