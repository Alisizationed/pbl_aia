import { useEffect } from 'react'
import { BrowserRouter, Routes, Route, NavLink } from "react-router-dom"
import keycloak from './keycloak'
import TrainPage from "./pages/TrainPage"
import CarriagesPage from "./pages/CarriagesPage"
import EdgesPage from "./pages/EdgesPage"
import NodesPage from "./pages/NodesPage"
import TimeWindowsPage from "./pages/TimeWindowsPage"
import Graph from "./pages/Graph"
import OptimizePage from "./pages/OptimizePage"
import "./App.css"

function App() {
  const user = keycloak.tokenParsed
  const username = user?.preferred_username || user?.name || 'User'
  const roles = user?.realm_access?.roles || []

  useEffect(() => {
    const timer = setInterval(() => {
      keycloak.updateToken(30).catch(() => keycloak.logout())
    }, 10_000)
    return () => clearInterval(timer)
  }, [])

  function logout() {
    keycloak.logout({ redirectUri: window.location.origin })
  }

  return (
      <BrowserRouter>
        <header className="app-header">
          <h1>Train Freight Optimization</h1>
          <div className="user-info">
            <span>{username}</span>
            {roles.includes('admin') && <span className="role-badge">admin</span>}
            <button onClick={logout}>Logout</button>
          </div>
        </header>

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
          <Route path="/"             element={<Graph />} />
          <Route path="/optimize"     element={<OptimizePage />} />
          <Route path="/trains"       element={<TrainPage />} />
          <Route path="/carriages"    element={<CarriagesPage />} />
          <Route path="/edges"        element={<EdgesPage />} />
          <Route path="/nodes"        element={<NodesPage />} />
          <Route path="/time-windows" element={<TimeWindowsPage />} />
        </Routes>
      </BrowserRouter>
  )
}

export default App