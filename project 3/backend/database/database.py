from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, declarative_base

from config.settings import Settings

# DATABASE_URL = (
#     "postgresql://postgres:password@localhost:5432/railway_db"
# )

engine = create_engine(Settings.DATABASE_URL)

SessionLocal = sessionmaker(
    autocommit=False,
    autoflush=False,
    bind=engine
)

Base = declarative_base()