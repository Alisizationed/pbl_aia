import { useEffect, useState } from "react";
import {api} from "../api/api";

function CarriagesPage() {
  const [carriages, setCarriages] = useState([]);
  const [weight, setWeight] = useState("");
  const [editingId, setEditingId] = useState(null);
  const [error, setError] = useState("");

  const loadCarriages = async () => {
    try {
      const carriages = await api.get("/carriages/");
      setCarriages(carriages);
      setError("");
    } catch {
      setError("Could not load carriages. Backend database may not be running.");
    }
  };

  useEffect(() => {
    loadCarriages();
  }, []);

  const saveCarriage = async (e) => {
    e.preventDefault();

    try {
      if (editingId) {
        await api.put(`/carriages/${editingId}?weight=${weight}`);
      } else {
        await api.post(`/carriages/?weight=${weight}`);
      }

      setWeight("");
      setEditingId(null);
      loadCarriages();
    } catch {
      setError("Could not save carriage.");
    }
  };

  const startEdit = (carriage) => {
    setEditingId(carriage.id);
    setWeight(carriage.weight);
  };

  const cancelEdit = () => {
    setEditingId(null);
    setWeight("");
  };

  const deleteCarriage = async (id) => {
    try {
      await api.delete(`/carriages/${id}`);
      loadCarriages();
    } catch {
      setError("Could not delete carriage.");
    }
  };

  return (
    <div className="page">
      <h1>Carriages CRUD</h1>

      {error && <p className="error">{error}</p>}

      <form onSubmit={saveCarriage} className="form">
        <input
          type="number"
          placeholder="Weight"
          value={weight}
          onChange={(e) => setWeight(e.target.value)}
          required
        />

        <button type="submit">
          {editingId ? "Update Carriage" : "Add Carriage"}
        </button>

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
            <th>Weight</th>
            <th>Actions</th>
          </tr>
        </thead>

        <tbody>
          {carriages.map((carriage) => (
            <tr key={carriage.id}>
              <td>{carriage.id}</td>
              <td>{carriage.weight}</td>
              <td>
                <button onClick={() => startEdit(carriage)}>Edit</button>
                <button onClick={() => deleteCarriage(carriage.id)}>
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

export default CarriagesPage;
