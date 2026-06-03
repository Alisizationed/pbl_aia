import {useEffect, useState} from "react";
import API from "../api/api";

export default function NodesPage() {
    const [nodes, setNodes] = useState([]);
    const [name, setName] = useState("");
    const [editingId, setEditingId] = useState(null);
    const [error, setError] = useState("");

    const load = async () => {
        try {
            const r = await API.get("/nodes/");
            setNodes(r.data);
            setError("");
        } catch {
            setError("Could not load nodes.");
        }
    };
    useEffect(() => {
        load();
    }, []);

    const save = async (e) => {
        e.preventDefault();
        try {
            if (editingId) await API.put(`/nodes/${editingId}?name=${name}`);
            else await API.post(`/nodes/?name=${name}`);
            setName("");
            setEditingId(null);
            load();
        } catch {
            setError("Could not save node.");
        }
    };

    const startEdit = (n) => {
        setEditingId(n.id);
        setName(n.name);
    };
    const cancel = () => {
        setEditingId(null);
        setName("");
    };
    const del = async (id) => {
        try {
            await API.delete(`/nodes/${id}`);
            load();
        } catch {
            setError("Could not delete.");
        }
    };

    return (
        <div className="crud-page">
            <h1>Nodes</h1>
            {error && <div className="alert alert-error">{error}</div>}
            <form onSubmit={save} className="form-bar">
                <label>
                    Name
                    <input type="text" placeholder="Station name" value={name} onChange={(e) => setName(e.target.value)}
                           required/>
                </label>
                <button type="submit">{editingId ? "Update" : "Add node"}</button>
                {editingId && <button type="button" className="btn-ghost" onClick={cancel}>Cancel</button>}
            </form>
            <div className="table-wrap">
                <table>
                    <thead>
                    <tr>
                        <th>ID</th>
                        <th>Name</th>
                        <th>Actions</th>
                    </tr>
                    </thead>
                    <tbody>
                    {nodes.map((n) => (
                        <tr key={n.id}>
                            <td>{n.id}</td>
                            <td>{n.name}</td>
                            <td style={{display: "flex", gap: 6}}>
                                <button className="btn-sm btn-ghost" onClick={() => startEdit(n)}>Edit</button>
                                <button className="btn-sm btn-danger" onClick={() => del(n.id)}>Delete</button>
                            </td>
                        </tr>
                    ))}
                    </tbody>
                </table>
            </div>
        </div>
    );
}