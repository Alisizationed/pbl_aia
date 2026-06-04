import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './index.css'
import App from './App.jsx'
import keycloak from './keycloak.js'

keycloak
  .init({ onLoad: 'login-required', checkLoginIframe: false })
  .then((authenticated) => {
    if (!authenticated) {
      // init() already redirected to Keycloak login, nothing to render
      return
    }

    createRoot(document.getElementById('root')).render(
      <StrictMode>
        <App />
      </StrictMode>,
    )
  })
  .catch((err) => {
    console.error('Keycloak init failed', err)
    document.getElementById('root').textContent =
      'Could not connect to authentication server. Is Keycloak running?'
  })
