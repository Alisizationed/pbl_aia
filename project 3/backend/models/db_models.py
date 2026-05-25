from sqlalchemy import Column, Integer, String, Float, ForeignKey
from database.database import Base


class NetworkNode(Base):
    __tablename__ = "nodes"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, nullable=False)


class NetworkEdge(Base):
    __tablename__ = "edges"

    id = Column(Integer, primary_key=True, index=True)

    from_node_id = Column(Integer, ForeignKey("nodes.id"))
    to_node_id = Column(Integer, ForeignKey("nodes.id"))

    cost = Column(Float, nullable=False)
    distance = Column(Float, nullable=False)
    capacity = Column(Integer, nullable=False)