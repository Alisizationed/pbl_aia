import keycloak from './keycloak'

const BASE = import.meta.env.VITE_API_URL

// wraps fetch to always attach the bearer token
async function request(path, options = {}) {
  // refresh the token if it expires in less than 30 seconds
  await keycloak.updateToken(30).catch(() => keycloak.logout())

  const res = await fetch(`${BASE}${path}`, {
    ...options,
    headers: {
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${keycloak.token}`,
      ...options.headers,
    },
  })

  if (!res.ok) {
    const err = await res.json().catch(() => ({}))
    throw new Error(err.detail || `Request failed: ${res.status}`)
  }

  return res.json()
}

export const api = {
  get: (path) => request(path),
  post: (path, body) => request(path, { method: 'POST', body: JSON.stringify(body) }),
  put: (path, body) => request(path, { method: 'PUT', body: JSON.stringify(body) }),
  delete: (path) => request(path, { method: 'DELETE' }),
}
