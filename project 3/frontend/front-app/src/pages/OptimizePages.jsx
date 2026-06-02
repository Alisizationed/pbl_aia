import { useState } from "react";
import API from "../api/api";

function OptimizePage() {
  const [startId, setStartId] = useState("");
  const [startName, setStartName] = useState("");
  const [endId, setEndId] = useState("");
  const [endName, setEndName] = useState("");
  const [trainIds, setTrainIds] = useState("");
  const [carriageIds, setCarriageIds] = useState("");
  const [departureTime, setDepartureTime] = useState("");

  const [routes, setRoutes] = useState(null);
  const [error, setError] = useState("");

  const requestOptimalRoute = async (e) => {
    e.preventDefault();

    const requestBody = {
      start: {
        id: Number(startId),
        name: startName,
      },
      end: {
        id: Number(endId),
        name: endName,
      },
      train_ids: trainIds.split(",").map((id) => Number(id.trim())),
      carriage_ids: carriageIds.split(",").map((id) => Number(id.trim())),
      departure_time: departureTime,
    };

    try {
      const response = await API.post("/optimize", requestBody);
      setRoutes(response.data);
      setError("");
    } catch {
      setError("Could not request optimal route. Backend/database may not be running.");
    }
  };

  return (
    <div className="page">
      <h1>Optimal Route Request</h1>

      {error && <p className="error">{error}</p>}

      <form onSubmit={requestOptimalRoute} className="form-container">
        <div className="form-row">
          <input type="number" placeholder="Start node ID" value={startId} onChange={(e) => setStartId(e.target.value)} required />
          <input type="text" placeholder="Start node name" value={startName} onChange={(e) => setStartName(e.target.value)} required />
        </div>

        <div className="form-row">
          <input type="number" placeholder="End node ID" value={endId} onChange={(e) => setEndId(e.target.value)} required />
          <input type="text" placeholder="End node name" value={endName} onChange={(e) => setEndName(e.target.value)} required />
        </div>

        <div className="form-row">
          <input type="text" placeholder="Train IDs: 1,2" value={trainIds} onChange={(e) => setTrainIds(e.target.value)} required />
          <input type="text" placeholder="Carriage IDs: 1,2,3" value={carriageIds} onChange={(e) => setCarriageIds(e.target.value)} required />
        </div>

        <div className="form-row">
          <input type="text" placeholder="Departure time: 2026-06-01T14:30" value={departureTime} onChange={(e) => setDepartureTime(e.target.value)} required />
          <button type="submit">Find Optimal Route</button>
        </div>
      </form>

      {routes && (
        <div>
          <h2>Optimization Result</h2>
          <pre>{JSON.stringify(routes, null, 2)}</pre>
        </div>
      )}
    </div>
  );
}

export default OptimizePage;