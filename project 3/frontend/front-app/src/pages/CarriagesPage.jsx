import {useEffect, useState} from "react";
import API from "../api/api";

export default function CarriagesPage() {
    const [carriages, setCarriages] = useState([]);
    const [weight, setWeight] = useState("");
    const [editingId, setEditingId] = useState(null);
    const [error, setError] = useState("");

    const load = async () => {
        try {
            const r = await API.get("/carriages/");
            setCarriages(r.data);
            setError("");
        } catch {
            setError("Could not load carriages.");
        }
    };
    useEffect(() => {
        load();
    }, []);

    const save = async (e) => {
        e.preventDefault();
        try {
            if (editingId) await API.put(`/carriages/${editingId}?weight=${weight}`);
            else await API.post(`/carriages/?weight=${weight}`);
            setWeight("");
            setEditingId(null);
            load();
        } catch {
            setError("Could not save carriage.");
        }
    };

    const startEdit = (c) => {
        setEditingId(c.id);
        setWeight(c.weight);
    };
    const cancel = () => {
        setEditingId(null);
        setWeight("");
    };
    const del = async (id) => {
        try {
            await API.delete(`/carriages/${id}`);
            load();
        } catch {
            setError("Could not delete.");
        }
    };

    return (
        <div className="crud-page">
            <h1>Carriages</h1>
            {error && <div className="alert alert-error">{error}</div>}
            <form onSubmit={save} className="form-bar">
                <label>
                    Weight
                    <input type="number" placeholder="e.g. 20" value={weight}
                           onChange={(e) => setWeight(e.target.value)} required/>
                </label>
                <button type="submit">{editingId ? "Update" : "Add carriage"}</button>
                {editingId && <button type="button" className="btn-ghost" onClick={cancel}>Cancel</button>}
            </form>
            <div className="table-wrap">
                <table>
                    <thead>
                    <tr>
                        <th>ID</th>
                        <th>Weight</th>
                        <th>Actions</th>
                    </tr>
                    </thead>
                    <tbody>
                    {carriages.map((c) => (
                        <tr key={c.id}>
                            <td>{c.id}</td>
                            <td>{c.weight}</td>
                            <td style={{display: "flex", gap: 6}}>
                                <button className="btn-sm btn-ghost" onClick={() => startEdit(c)}>Edit</button>
                                <button className="btn-sm btn-danger" onClick={() => del(c.id)}>Delete</button>
                            </td>
                        </tr>
                    ))}
                    </tbody>
                </table>
            </div>
        </div>
    );
}