import { useEffect, useState } from "react";
import {api} from "../api/api";

function NodesPage() {
  const [nodes, setNodes] = useState([]);
  const [name, setName] = useState("");
  const [editingId, setEditingId] = useState(null);
  const [error, setError] = useState("");

  const loadNodes = async () => {
    try {
      const nodes = await api.get("/nodes/");
      setNodes(nodes);
      setError("");
    } catch {
      setError("Could not load nodes. Backend database may not be running.");
    }
  };

  useEffect(() => {
    loadNodes();
  }, []);

  const saveNode = async (e) => {
    e.preventDefault();

    try {
      if (editingId) {
        await api.put(`/nodes/${editingId}?name=${name}`);
      } else {
        await api.post(`/nodes/?name=${name}`);
      }

      setName("");
      setEditingId(null);
      loadNodes();
    } catch {
      setError("Could not save node.");
    }
  };

  const startEdit = (node) => {
    setEditingId(node.id);
    setName(node.name);
  };

  const cancelEdit = () => {
    setEditingId(null);
    setName("");
  };

  const deleteNode = async (id) => {
    try {
      await api.delete(`/nodes/${id}`);
      loadNodes();
    } catch {
      setError("Could not delete node.");
    }
  };

  return (
    <div className="page">
      <h1>Nodes CRUD</h1>

      {error && <p className="error">{error}</p>}

      <form onSubmit={saveNode} className="form">
        <input
          type="text"
          placeholder="Node name"
          value={name}
          onChange={(e) => setName(e.target.value)}
          required
        />

        <button type="submit">{editingId ? "Update Node" : "Add Node"}</button>

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
            <th>Name</th>
            <th>Actions</th>
          </tr>
        </thead>

        <tbody>
          {nodes.map((node) => (
            <tr key={node.id}>
              <td>{node.id}</td>
              <td>{node.name}</td>
              <td>
                <button onClick={() => startEdit(node)}>Edit</button>
                <button onClick={() => deleteNode(node.id)}>Delete</button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export default NodesPage;
