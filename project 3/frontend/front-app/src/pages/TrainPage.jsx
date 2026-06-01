import { useEffect, useState } from "react";
import API from "../api/api";

function TrainPage() {
  const [trains, setTrains] = useState([]);
  const [capacity, setCapacity] = useState("");
  const [usedWeight, setUsedWeight] = useState("");
  const [editingId, setEditingId] = useState(null);
  const [error, setError] = useState("");

  const loadTrains = async () => {
    try {
      const response = await API.get("/trains/");
      setTrains(response.data);
      setError("");
    } catch {
      setError("Could not load trains. Backend database may not be running.");
    }
  };

  useEffect(() => {
    loadTrains();
  }, []);

  const saveTrain = async (e) => {
    e.preventDefault();

    try {
      if (editingId) {
        await API.put(
          `/trains/${editingId}?capacity=${capacity}&used_weight=${usedWeight || 0}`
        );
      } else {
        await API.post(`/trains/?capacity=${capacity}&used_weight=${usedWeight || 0}`);
      }

      setCapacity("");
      setUsedWeight("");
      setEditingId(null);
      loadTrains();
    } catch {
      setError("Could not save train.");
    }
  };

  const startEdit = (train) => {
    setEditingId(train.id);
    setCapacity(train.capacity);
    setUsedWeight(train.used_weight);
  };

  const cancelEdit = () => {
    setEditingId(null);
    setCapacity("");
    setUsedWeight("");
  };

  const deleteTrain = async (id) => {
    try {
      await API.delete(`/trains/${id}`);
      loadTrains();
    } catch {
      setError("Could not delete train.");
    }
  };

  return (
    <div className="page">
      <h1>Trains CRUD</h1>

      {error && <p className="error">{error}</p>}

      <form onSubmit={saveTrain} className="form">
        <input
          type="number"
          placeholder="Capacity"
          value={capacity}
          onChange={(e) => setCapacity(e.target.value)}
          required
        />

        <input
          type="number"
          placeholder="Used weight"
          value={usedWeight}
          onChange={(e) => setUsedWeight(e.target.value)}
        />

        <button type="submit">{editingId ? "Update Train" : "Add Train"}</button>

        {editingId && (
          <button type="button" onClick={cancelEdit}>
            Cancel
          </button>
        )}
      </form>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Capacity</th>
            <th>Used Weight</th>
            <th>Actions</th>
          </tr>
        </thead>

        <tbody>
          {trains.map((train) => (
            <tr key={train.id}>
              <td>{train.id}</td>
              <td>{train.capacity}</td>
              <td>{train.used_weight}</td>
              <td>
                <button onClick={() => startEdit(train)}>Edit</button>
                <button onClick={() => deleteTrain(train.id)}>Delete</button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export default TrainPage;