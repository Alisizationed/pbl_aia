from fastapi import FastAPI
from routers.optimize_router import router as optimize_router

app = FastAPI()

app.include_router(optimize_router)