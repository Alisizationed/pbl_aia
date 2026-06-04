import { useEffect, useState } from 'react'
import keycloak from './keycloak'
import { api } from './api'
import './App.css'

function App() {
  const user = keycloak.tokenParsed
  const username = user?.preferred_username || user?.name || 'User'
  const roles = user?.realm_access?.roles || []

  const [nodes, setNodes] = useState([])
  const [error, setError] = useState('')

  // load nodes as a quick test that auth + backend works
  useEffect(() => {
    api.get('/nodes/')
      .then(setNodes)
      .catch((err) => setError(err.message))

    // keep the token alive: refresh when <30 seconds left
    const timer = setInterval(() => {
      keycloak.updateToken(30).catch(() => keycloak.logout())
    }, 10_000)

    return () => clearInterval(timer)
  }, [])

  function logout() {
    keycloak.logout({ redirectUri: window.location.origin })
  }

  return (
    <>
      <header className="app-header">
        <h1>Train Freight Optimization</h1>
        <div className="user-info">
          <span>{username}</span>
          {roles.includes('admin') && <span className="role-badge">admin</span>}
          <button onClick={logout}>Logout</button>
        </div>
      </header>

      <main className="app-main">
        <h2>Railway Nodes</h2>

        {error && <p className="error">Backend error: {error}</p>}

        {nodes.length === 0 && !error && (
          <p className="hint">No nodes yet — add some through the API.</p>
        )}

        <ul>
          {nodes.map((n) => (
            <li key={n.id}>{n.name}</li>
          ))}
        </ul>
      </main>
    </>
  )
}

export default App
