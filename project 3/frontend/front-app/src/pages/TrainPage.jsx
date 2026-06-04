import {useEffect, useState} from "react";
import API from "../api/api";

export default function TrainPage() {
    const [trains, setTrains] = useState([]);
    const [capacity, setCapacity] = useState("");
    const [usedWeight, setUsedWeight] = useState("");
    const [editingId, setEditingId] = useState(null);
    const [error, setError] = useState("");

    const load = async () => {
        try {
            const r = await API.get("/trains/");
            setTrains(r.data);
            setError("");
        } catch {
            setError("Could not load trains.");
        }
    };
    useEffect(() => {
        load();
    }, []);

    const save = async (e) => {
        e.preventDefault();
        try {
            if (editingId) await API.put(`/trains/${editingId}?capacity=${capacity}&used_weight=${usedWeight || 0}`);
            else await API.post(`/trains/?capacity=${capacity}&used_weight=${usedWeight || 0}`);
            setCapacity("");
            setUsedWeight("");
            setEditingId(null);
            load();
        } catch {
            setError("Could not save train.");
        }
    };

    const startEdit = (t) => {
        setEditingId(t.id);
        setCapacity(t.capacity);
        setUsedWeight(t.used_weight);
    };
    const cancel = () => {
        setEditingId(null);
        setCapacity("");
        setUsedWeight("");
    };
    const del = async (id) => {
        try {
            await API.delete(`/trains/${id}`);
            load();
        } catch {
            setError("Could not delete.");
        }
    };

    return (
        <div className="crud-page">
            <h1>Trains</h1>
            {error && <div className="alert alert-error">{error}</div>}
            <form onSubmit={save} className="form-bar">
                <label>
                    Capacity
                    <input type="number" placeholder="e.g. 500" value={capacity}
                           onChange={(e) => setCapacity(e.target.value)} required/>
                </label>
                <label>
                    Used weight
                    <input type="number" placeholder="e.g. 0" value={usedWeight}
                           onChange={(e) => setUsedWeight(e.target.value)}/>
                </label>
                <button type="submit">{editingId ? "Update" : "Add train"}</button>
                {editingId && <button type="button" className="btn-ghost" onClick={cancel}>Cancel</button>}
            </form>
            <div className="table-wrap">
                <table>
                    <thead>
                    <tr>
                        <th>ID</th>
                        <th>Capacity</th>
                        <th>Used weight</th>
                        <th>Actions</th>
                    </tr>
                    </thead>
                    <tbody>
                    {trains.map((t) => (
                        <tr key={t.id}>
                            <td>{t.id}</td>
                            <td>{t.capacity}</td>
                            <td>{t.used_weight}</td>
                            <td style={{display: "flex", gap: 6}}>
                                <button className="btn-sm btn-ghost" onClick={() => startEdit(t)}>Edit</button>
                                <button className="btn-sm btn-danger" onClick={() => del(t.id)}>Delete</button>
                            </td>
                        </tr>
                    ))}
                    </tbody>
                </table>
            </div>
        </div>
    );
}