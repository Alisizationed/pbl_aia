import { BrowserRouter, Routes, Route, NavLink } from "react-router-dom";

import TrainPage from "./pages/TrainPage";
import CarriagesPage from "./pages/CarriagesPage";
import EdgesPage from "./pages/EdgesPage";
import NodesPage from "./pages/NodesPage";
import TimeWindowsPage from "./pages/TimeWindowsPage";
import Graph from "./pages/Graph";
import OptimizePage from "./pages/OptimizePage";

import "./App.css";

function App() {
  return (
    <BrowserRouter>
      <nav className="navbar">
        <NavLink to="/">Graph</NavLink>
        <NavLink to="/optimize">Optimize</NavLink>
        <NavLink to="/trains">Trains</NavLink>
        <NavLink to="/carriages">Carriages</NavLink>
        <NavLink to="/edges">Edges</NavLink>
        <NavLink to="/nodes">Nodes</NavLink>
        <NavLink to="/time-windows">Time Windows</NavLink>
      </nav>

      <Routes>
        <Route path="/"            element={<Graph />} />
        <Route path="/optimize"    element={<OptimizePage />} />
        <Route path="/trains"      element={<TrainPage />} />
        <Route path="/carriages"   element={<CarriagesPage />} />
        <Route path="/edges"       element={<EdgesPage />} />
        <Route path="/nodes"       element={<NodesPage />} />
        <Route path="/time-windows" element={<TimeWindowsPage />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;