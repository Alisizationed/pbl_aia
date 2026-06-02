import { BrowserRouter, Routes, Route, Link } from "react-router-dom";

import GraphPage from "./pages/GraphPage";
import TrainPage from "./pages/TrainPage";
import CarriagesPage from "./pages/CarriagesPage";
import EdgesPage from "./pages/EdgesPage";
import NodesPage from "./pages/NodesPage";
import TimeWindowsPage from "./pages/TimeWindowsPage";

import "./App.css";

function App() {
  return (
    <BrowserRouter>
      <nav className="navbar">
        <Link to="/">Home</Link>
        <Link to="/trains">Trains</Link>
        <Link to="/carriages">Carriages</Link>
        <Link to="/edges">Edges</Link>
        <Link to="/graph">Graph</Link>
        <Link to="/nodes">Nodes</Link>
        <Link to="/time-windows">Time Windows</Link>
      </nav>

      <Routes>
        <Route path="/" element={<h1>Railway Management System</h1>} />

        <Route path="/trains" element={<TrainPage />} />
        <Route path="/carriages" element={<CarriagesPage />} />
        <Route path="/edges" element={<EdgesPage />} />
        <Route path="/graph" element={<GraphPage />} />
        <Route path="/nodes" element={<NodesPage />} />
        <Route path="/time-windows" element={<TimeWindowsPage />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;