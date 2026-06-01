import { useEffect, useState } from "react";
import API from "../api/api";

function TrainPage() {
  const [trains, setTrains] = useState([]);
  const [capacity, setCapacity] = useState("");
  const [usedWeight, setUsedWeight] = useState("");
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

  const createTrain = async (e) => {
    e.preventDefault();

    try {
      await API.post(`/trains/?capacity=${capacity}&used_weight=${usedWeight || 0}`);
      setCapacity("");
      setUsedWeight("");
      loadTrains();
    } catch {
      setError("Could not create train.");
    }
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

      <form onSubmit={createTrain} className="form">
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

        <button type="submit">Add Train</button>
      </form>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Capacity</th>
            <th>Used Weight</th>
            <th>Action</th>
          </tr>
        </thead>

        <tbody>
          {trains.map((train) => (
            <tr key={train.id}>
              <td>{train.id}</td>
              <td>{train.capacity}</td>
              <td>{train.used_weight}</td>
              <td>
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