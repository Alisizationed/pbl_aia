import { useEffect, useState } from "react";
import API from "../api/api";

export default function TrainPage() {
  const [trains, setTrains]       = useState([]);
  const [capacity, setCapacity]   = useState("");
  const [usedWeight, setUsedWeight] = useState("");
  const [editingId, setEditingId] = useState(null);
  const [error, setError]         = useState("");

  const load = async () => {
    try { const r = await API.get("/trains/"); setTrains(r.data); setError(""); }
    catch { setError("Could not load trains."); }
  };
  useEffect(() => { load(); }, []);

  const save = async (e) => {
    e.preventDefault();
    try {
      if (editingId) await API.put(`/trains/${editingId}?capacity=${capacity}&used_weight=${usedWeight || 0}`);
      else           await API.post(`/trains/?capacity=${capacity}&used_weight=${usedWeight || 0}`);
      setCapacity(""); setUsedWeight(""); setEditingId(null); load();
    } catch { setError("Could not save train."); }
  };

  const startEdit = (t) => { setEditingId(t.id); setCapacity(t.capacity); setUsedWeight(t.used_weight); };
  const cancel    = ()  => { setEditingId(null); setCapacity(""); setUsedWeight(""); };
  const del       = async (id) => { try { await API.delete(`/trains/${id}`); load(); } catch { setError("Could not delete."); } };

  return (
    <div className="crud-page">
      <h1>Trains</h1>
      {error && <div className="alert alert-error">{error}</div>}
      <form onSubmit={save} className="form-bar">
        <label>
          Capacity
          <input type="number" placeholder="e.g. 500" value={capacity} onChange={(e) => setCapacity(e.target.value)} required />
        </label>
        <label>
          Used weight
          <input type="number" placeholder="e.g. 0" value={usedWeight} onChange={(e) => setUsedWeight(e.target.value)} />
        </label>
        <button type="submit">{editingId ? "Update" : "Add train"}</button>
        {editingId && <button type="button" className="btn-ghost" onClick={cancel}>Cancel</button>}
      </form>
      <div className="table-wrap">
        <table>
          <thead><tr><th>ID</th><th>Capacity</th><th>Used weight</th><th>Actions</th></tr></thead>
          <tbody>
            {trains.map((t) => (
              <tr key={t.id}>
                <td>{t.id}</td><td>{t.capacity}</td><td>{t.used_weight}</td>
                <td style={{ display:"flex", gap:6 }}>
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
JSXEOF

cat > /home/claude/front-app/src/pages/CarriagesPage.jsx << 'JSXEOF'
import { useEffect, useState } from "react";
import API from "../api/api";

export default function CarriagesPage() {
  const [carriages, setCarriages] = useState([]);
  const [weight, setWeight]       = useState("");
  const [editingId, setEditingId] = useState(null);
  const [error, setError]         = useState("");

  const load = async () => {
    try { const r = await API.get("/carriages/"); setCarriages(r.data); setError(""); }
    catch { setError("Could not load carriages."); }
  };
  useEffect(() => { load(); }, []);

  const save = async (e) => {
    e.preventDefault();
    try {
      if (editingId) await API.put(`/carriages/${editingId}?weight=${weight}`);
      else           await API.post(`/carriages/?weight=${weight}`);
      setWeight(""); setEditingId(null); load();
    } catch { setError("Could not save carriage."); }
  };

  const startEdit = (c) => { setEditingId(c.id); setWeight(c.weight); };
  const cancel    = ()  => { setEditingId(null); setWeight(""); };
  const del       = async (id) => { try { await API.delete(`/carriages/${id}`); load(); } catch { setError("Could not delete."); } };

  return (
    <div className="crud-page">
      <h1>Carriages</h1>
      {error && <div className="alert alert-error">{error}</div>}
      <form onSubmit={save} className="form-bar">
        <label>
          Weight
          <input type="number" placeholder="e.g. 20" value={weight} onChange={(e) => setWeight(e.target.value)} required />
        </label>
        <button type="submit">{editingId ? "Update" : "Add carriage"}</button>
        {editingId && <button type="button" className="btn-ghost" onClick={cancel}>Cancel</button>}
      </form>
      <div className="table-wrap">
        <table>
          <thead><tr><th>ID</th><th>Weight</th><th>Actions</th></tr></thead>
          <tbody>
            {carriages.map((c) => (
              <tr key={c.id}>
                <td>{c.id}</td><td>{c.weight}</td>
                <td style={{ display:"flex", gap:6 }}>
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
JSXEOF

cat > /home/claude/front-app/src/pages/NodesPage.jsx << 'JSXEOF'
import { useEffect, useState } from "react";
import API from "../api/api";

export default function NodesPage() {
  const [nodes, setNodes] = useState([]);
  const [name, setName]   = useState("");
  const [editingId, setEditingId] = useState(null);
  const [error, setError] = useState("");

  const load = async () => {
    try { const r = await API.get("/nodes/"); setNodes(r.data); setError(""); }
    catch { setError("Could not load nodes."); }
  };
  useEffect(() => { load(); }, []);

  const save = async (e) => {
    e.preventDefault();
    try {
      if (editingId) await API.put(`/nodes/${editingId}?name=${name}`);
      else           await API.post(`/nodes/?name=${name}`);
      setName(""); setEditingId(null); load();
    } catch { setError("Could not save node."); }
  };

  const startEdit = (n) => { setEditingId(n.id); setName(n.name); };
  const cancel    = ()  => { setEditingId(null); setName(""); };
  const del       = async (id) => { try { await API.delete(`/nodes/${id}`); load(); } catch { setError("Could not delete."); } };

  return (
    <div className="crud-page">
      <h1>Nodes</h1>
      {error && <div className="alert alert-error">{error}</div>}
      <form onSubmit={save} className="form-bar">
        <label>
          Name
          <input type="text" placeholder="Station name" value={name} onChange={(e) => setName(e.target.value)} required />
        </label>
        <button type="submit">{editingId ? "Update" : "Add node"}</button>
        {editingId && <button type="button" className="btn-ghost" onClick={cancel}>Cancel</button>}
      </form>
      <div className="table-wrap">
        <table>
          <thead><tr><th>ID</th><th>Name</th><th>Actions</th></tr></thead>
          <tbody>
            {nodes.map((n) => (
              <tr key={n.id}>
                <td>{n.id}</td><td>{n.name}</td>
                <td style={{ display:"flex", gap:6 }}>
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