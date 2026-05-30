from fastapi import FastAPI
from routers.optimize_router import router as optimize_router
from routers.train_router import router as train_router
from routers.carriage_router import router as carriage_router

app = FastAPI()

app.include_router(optimize_router)
app.include_router(train_router)
app.include_router(carriage_router)