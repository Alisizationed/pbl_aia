from sqlalchemy import Column, Integer, String, Float, ForeignKey, DateTime
from sqlalchemy.orm import relationship
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

    time_windows = relationship("EdgeTimeWindow", backref="edge")


class EdgeTimeWindow(Base):
    __tablename__ = "edge_time_windows"

    id = Column(Integer, primary_key=True, index=True)
    edge_id = Column(Integer, ForeignKey("edges.id"))
    valid_from = Column(DateTime, nullable=False)
    valid_until = Column(DateTime, nullable=False)


class Carriage(Base):
    __tablename__ = "carriages"

    id = Column(Integer, primary_key=True, index=True)
    weight = Column(Float, nullable=False)


class Train(Base):
    __tablename__ = "trains"

    id = Column(Integer, primary_key=True, index=True)
    capacity = Column(Float, nullable=False)
    used_weight = Column(Float, nullable=False)