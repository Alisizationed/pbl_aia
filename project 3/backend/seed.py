"""Seed the database with a few test carriages and trains.

Run from the backend directory once Postgres is up:

    python seed.py

Safe to re-run: it only inserts rows when the tables are empty.
"""

from database.database import Base, SessionLocal, engine
import models.db_models  # noqa: F401  -- registers all tables on Base.metadata
from models.db_models import Carriage, Train


CARRIAGE_WEIGHTS = [12.5, 18.0, 9.75, 22.0, 15.5]
TRAINS = [
    {"capacity": 120.0, "used_weight": 48.0},
    {"capacity": 80.0, "used_weight": 80.0},
    {"capacity": 150.0, "used_weight": 0.0},
]


def seed():
    Base.metadata.create_all(bind=engine)

    db = SessionLocal()
    try:
        if db.query(Carriage).count() == 0:
            db.add_all(Carriage(weight=w) for w in CARRIAGE_WEIGHTS)
            print(f"Inserted {len(CARRIAGE_WEIGHTS)} carriages.")
        else:
            print("Carriages already present, skipping.")

        if db.query(Train).count() == 0:
            db.add_all(Train(**t) for t in TRAINS)
            print(f"Inserted {len(TRAINS)} trains.")
        else:
            print("Trains already present, skipping.")

        db.commit()
    finally:
        db.close()


if __name__ == "__main__":
    seed()
