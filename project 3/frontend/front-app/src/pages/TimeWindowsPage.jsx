import { useEffect, useState } from "react";
import API from "../api/api";

function TimeWindowsPage() {
  const [timeWindows, setTimeWindows] = useState([]);

  const [edgeId, setEdgeId] = useState("");
  const [validFrom, setValidFrom] = useState("");
  const [validUntil, setValidUntil] = useState("");

  const [editingId, setEditingId] = useState(null);
  const [error, setError] = useState("");

  const formatDate = (value) => {
    if (!value) return "";

    const date = new Date(value);

    const year = date.getFullYear();
    const month = String(date.getMonth() + 1).padStart(2, "0");
    const day = String(date.getDate()).padStart(2, "0");
    const hours = String(date.getHours()).padStart(2, "0");
    const minutes = String(date.getMinutes()).padStart(2, "0");

    return `${year}-${month}-${day} ${hours}:${minutes}`;
  };

  const loadTimeWindows = async () => {
    try {
      const response = await API.get("/time-windows/");
      setTimeWindows(response.data);
      setError("");
    } catch {
      setError(
        "Could not load time windows. Backend database may not be running."
      );
    }
  };

  useEffect(() => {
    loadTimeWindows();
  }, []);

  const saveTimeWindow = async (e) => {
    e.preventDefault();

    try {
      if (editingId) {
        await API.put(
          `/time-windows/${editingId}?edge_id=${edgeId}&valid_from=${validFrom}&valid_until=${validUntil}`
        );
      } else {
        await API.post(
          `/time-windows/?edge_id=${edgeId}&valid_from=${validFrom}&valid_until=${validUntil}`
        );
      }

      setEdgeId("");
      setValidFrom("");
      setValidUntil("");
      setEditingId(null);

      loadTimeWindows();
    } catch {
      setError("Could not save time window.");
    }
  };

  const startEdit = (timeWindow) => {
    setEditingId(timeWindow.id);
    setEdgeId(timeWindow.edge_id);
    setValidFrom(timeWindow.valid_from?.slice(0, 16));
    setValidUntil(timeWindow.valid_until?.slice(0, 16));
  };

  const cancelEdit = () => {
    setEditingId(null);
    setEdgeId("");
    setValidFrom("");
    setValidUntil("");
  };

  const deleteTimeWindow = async (id) => {
    try {
      await API.delete(`/time-windows/${id}`);
      loadTimeWindows();
    } catch {
      setError("Could not delete time window.");
    }
  };

  return (
    <div className="page">
      <h1>Time Windows CRUD</h1>

      {error && <p className="error">{error}</p>}

      <form onSubmit={saveTimeWindow} className="form-container">
        <div className="form-row">
          <input
            type="number"
            placeholder="Edge ID"
            value={edgeId}
            onChange={(e) => setEdgeId(e.target.value)}
            required
          />

          <input
            type="datetime-local"
            value={validFrom}
            onChange={(e) => setValidFrom(e.target.value)}
            required
          />

          <input
            type="datetime-local"
            value={validUntil}
            onChange={(e) => setValidUntil(e.target.value)}
            required
          />

          <button type="submit">
            {editingId ? "Update Time Window" : "Add Time Window"}
          </button>

          {editingId && (
            <button type="button" onClick={cancelEdit}>
              Cancel
            </button>
          )}
        </div>
      </form>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Edge ID</th>
            <th>Valid From</th>
            <th>Valid Until</th>
            <th>Actions</th>
          </tr>
        </thead>

        <tbody>
          {timeWindows.map((timeWindow) => (
            <tr key={timeWindow.id}>
              <td>{timeWindow.id}</td>
              <td>{timeWindow.edge_id}</td>

              <td>{formatDate(timeWindow.valid_from)}</td>

              <td>{formatDate(timeWindow.valid_until)}</td>

              <td>
                <button onClick={() => startEdit(timeWindow)}>
                  Edit
                </button>

                <button
                  onClick={() => deleteTimeWindow(timeWindow.id)}
                >
                  Delete
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export default TimeWindowsPage;