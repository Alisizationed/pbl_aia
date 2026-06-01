import { useEffect, useState } from "react";
import API from "../api/api";

function CarriagesPage() {
  const [carriages, setCarriages] = useState([]);
  const [weight, setWeight] = useState("");
  const [error, setError] = useState("");

  const loadCarriages = async () => {
    try {
      const response = await API.get("/carriages/");
      setCarriages(response.data);
      setError("");
    } catch {
      setError("Could not load carriages. Backend database may not be running.");
    }
  };

  useEffect(() => {
    loadCarriages();
  }, []);

  const createCarriage = async (e) => {
    e.preventDefault();

    try {
      await API.post(`/carriages/?weight=${weight}`);
      setWeight("");
      loadCarriages();
    } catch {
      setError("Could not create carriage.");
    }
  };

  const deleteCarriage = async (id) => {
    try {
      await API.delete(`/carriages/${id}`);
      loadCarriages();
    } catch {
      setError("Could not delete carriage.");
    }
  };

  return (
    <div className="page">
      <h1>Carriages CRUD</h1>

      {error && <p className="error">{error}</p>}

      <form onSubmit={createCarriage} className="form">
        <input
          type="number"
          placeholder="Weight"
          value={weight}
          onChange={(e) => setWeight(e.target.value)}
          required
        />

        <button type="submit">Add Carriage</button>
      </form>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Weight</th>
            <th>Action</th>
          </tr>
        </thead>

        <tbody>
          {carriages.map((carriage) => (
            <tr key={carriage.id}>
              <td>{carriage.id}</td>
              <td>{carriage.weight}</td>
              <td>
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