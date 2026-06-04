import {useEffect, useState} from "react";
import API from "../api/api";

function EdgesPage() {
    const [edges, setEdges] = useState([]);

    const [fromNodeId, setFromNodeId] = useState("");
    const [toNodeId, setToNodeId] = useState("");
    const [cost, setCost] = useState("");
    const [distance, setDistance] = useState("");
    const [capacity, setCapacity] = useState("");
    const [time, setTime] = useState("");

    const [editingId, setEditingId] = useState(null);
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

    const saveEdge = async (e) => {
        e.preventDefault();

        try {
            if (editingId) {
                await API.put(
                    `/edges/${editingId}?from_node_id=${fromNodeId}&to_node_id=${toNodeId}&cost=${cost}&distance=${distance}&capacity=${capacity}&time=${time}`
                );
            } else {
                await API.post(
                    `/edges/?from_node_id=${fromNodeId}&to_node_id=${toNodeId}&cost=${cost}&distance=${distance}&capacity=${capacity}&time=${time}`
                );
            }

            setFromNodeId("");
            setToNodeId("");
            setCost("");
            setDistance("");
            setCapacity("");
            setTime("");
            setEditingId(null);

            loadEdges();
        } catch {
            setError("Could not save edge.");
        }
    };

    const startEdit = (edge) => {
        setEditingId(edge.id);

        setFromNodeId(edge.from_node_id);
        setToNodeId(edge.to_node_id);
        setCost(edge.cost);
        setDistance(edge.distance);
        setCapacity(edge.capacity);
        setTime(edge.time);
    };

    const cancelEdit = () => {
        setEditingId(null);

        setFromNodeId("");
        setToNodeId("");
        setCost("");
        setDistance("");
        setCapacity("");
        setTime("");
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

            <form onSubmit={saveEdge} className="form-container">

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

                    <button type="submit">
                        {editingId ? "Update Edge" : "Add Edge"}
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
                    <th>From Node</th>
                    <th>To Node</th>
                    <th>Cost</th>
                    <th>Distance</th>
                    <th>Capacity</th>
                    <th>Time</th>
                    <th>Actions</th>
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
                            <button onClick={() => startEdit(edge)}>
                                Edit
                            </button>

                            <button onClick={() => deleteEdge(edge.id)}>
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

export default EdgesPage;