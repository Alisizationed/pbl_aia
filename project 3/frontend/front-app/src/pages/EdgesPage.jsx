import { useEffect, useState } from "react";
import API from "../api/api";

function EdgesPage() {
  const [edges, setEdges] = useState([]);

  const [fromNodeId, setFromNodeId] = useState("");
  const [toNodeId, setToNodeId] = useState("");
  const [cost, setCost] = useState("");
  const [distance, setDistance] = useState("");
  const [capacity, setCapacity] = useState("");
  const [time, setTime] = useState("");

  const [error, setError] = useState("");

  const loadEdges = async () => {
    try {
      const response = await API.get("/edges/");
      setEdges(response.data);
      setError("");
    } catch {
      setError("Could not load edges. Backend database may not be running.");
    }
  };

  useEffect(() => {
    loadEdges();
  }, []);

  const createEdge = async (e) => {
    e.preventDefault();

    try {
      await API.post(
        `/edges/?from_node_id=${fromNodeId}&to_node_id=${toNodeId}&cost=${cost}&distance=${distance}&capacity=${capacity}&time=${time}`
      );

      setFromNodeId("");
      setToNodeId("");
      setCost("");
      setDistance("");
      setCapacity("");
      setTime("");

      loadEdges();
    } catch {
      setError("Could not create edge.");
    }
  };

  const deleteEdge = async (id) => {
    try {
      await API.delete(`/edges/${id}`);
      loadEdges();
    } catch {
      setError("Could not delete edge.");
    }
  };

  return (
    <div className="page">
      <h1>Edges CRUD</h1>

      {error && <p className="error">{error}</p>}

      <form onSubmit={createEdge} className="form-container">

  <div className="form-row">
    <input
      type="number"
      placeholder="From node ID"
      value={fromNodeId}
      onChange={(e) => setFromNodeId(e.target.value)}
      required
    />

    <input
      type="number"
      placeholder="To node ID"
      value={toNodeId}
      onChange={(e) => setToNodeId(e.target.value)}
      required
    />

    <input
      type="number"
      placeholder="Cost"
      value={cost}
      onChange={(e) => setCost(e.target.value)}
      required
    />
  </div>

  <div className="form-row">
    <input
      type="number"
      placeholder="Distance"
      value={distance}
      onChange={(e) => setDistance(e.target.value)}
      required
    />

    <input
      type="number"
      placeholder="Capacity"
      value={capacity}
      onChange={(e) => setCapacity(e.target.value)}
      required
    />

    <input
      type="number"
      placeholder="Time"
      value={time}
      onChange={(e) => setTime(e.target.value)}
      required
    />

    <button type="submit">Add Edge</button>
  </div>

</form>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>From Node</th>
            <th>To Node</th>
            <th>Cost</th>
            <th>Distance</th>
            <th>Capacity</th>
            <th>Time</th>
            <th>Action</th>
          </tr>
        </thead>

        <tbody>
          {edges.map((edge) => (
            <tr key={edge.id}>
              <td>{edge.id}</td>
              <td>{edge.from_node_id}</td>
              <td>{edge.to_node_id}</td>
              <td>{edge.cost}</td>
              <td>{edge.distance}</td>
              <td>{edge.capacity}</td>
              <td>{edge.time}</td>
              <td>
                <button onClick={() => deleteEdge(edge.id)}>Delete</button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export default EdgesPage;