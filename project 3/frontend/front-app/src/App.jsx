import { BrowserRouter, Routes, Route, Link } from "react-router-dom";

import TrainPage from "./pages/TrainPage";
import CarriagesPage from "./pages/CarriagesPage";
import EdgesPage from "./pages/EdgesPage";

import "./App.css";

function App() {
  return (
    <BrowserRouter>
      <nav className="navbar">
        <Link to="/">Home</Link>
        <Link to="/trains">Trains</Link>
        <Link to="/carriages">Carriages</Link>
        <Link to="/edges">Edges</Link>
      </nav>

      <Routes>
        <Route path="/" element={<h1>Railway Management System</h1>} />
        <Route path="/trains" element={<TrainPage />} />
        <Route path="/carriages" element={<CarriagesPage />} />
        <Route path="/edges" element={<EdgesPage />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;