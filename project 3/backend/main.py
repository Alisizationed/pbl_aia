from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from routers.optimize_router import router as optimize_router
from routers.train_router import router as train_router
from routers.carriage_router import router as carriage_router
from routers.node_router import router as node_router
from routers.edge_router import router as edge_router
from routers.time_window_router import router as time_window_router
from routers.network_router import router as network_router

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=[
        "http://localhost:5173",
        "http://127.0.0.1:5173",
    ],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(optimize_router)
app.include_router(train_router)
app.include_router(carriage_router)
app.include_router(node_router)
app.include_router(edge_router)
app.include_router(time_window_router)
app.include_router(network_router)