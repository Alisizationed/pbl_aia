import { useCallback, useEffect, useMemo, useState } from 'react'

const API_URL = import.meta.env.VITE_API_URL ?? 'http://localhost:8000'

async function loadJson(path) {
  const response = await fetch(`${API_URL}${path}`)

  if (!response.ok) {
    throw new Error(`Backend returned ${response.status}`)
  }

  return response.json()
}

async function loadFleet() {
  const [carriages, trains] = await Promise.all([
    loadJson('/carriages/'),
    loadJson('/trains/'),
  ])

  return { carriages, trains }
}

function formatMetric(value, suffix = '') {
  return Number.isFinite(Number(value)) ? `${Number(value).toLocaleString()}${suffix}` : '-'
}

function utilization(train) {
  const capacity = Number(train.capacity)

  if (!Number.isFinite(capacity) || capacity <= 0) {
    return null
  }

  return Math.min(100, Math.round((Number(train.used_weight || 0) / capacity) * 100))
}

function FleetView() {
  const [fleet, setFleet] = useState({ carriages: [], trains: [] })
  const [status, setStatus] = useState('loading')
  const [error, setError] = useState('')

  const refresh = useCallback(() => {
    setStatus('loading')
    setError('')

    loadFleet()
      .then((nextFleet) => {
        setFleet(nextFleet)
        setStatus('ready')
      })
      .catch((err) => {
        setStatus('error')
        setError(err instanceof Error ? err.message : 'Could not load fleet')
      })
  }, [])

  useEffect(() => {
    let ignore = false

    loadFleet()
      .then((nextFleet) => {
        if (ignore) {
          return
        }

        setFleet(nextFleet)
        setStatus('ready')
      })
      .catch((err) => {
        if (ignore) {
          return
        }

        setStatus('error')
        setError(err instanceof Error ? err.message : 'Could not load fleet')
      })

    return () => {
      ignore = true
    }
  }, [])

  const totals = useMemo(
    () => ({
      carriages: fleet.carriages.length,
      trains: fleet.trains.length,
      capacity: fleet.trains.reduce((sum, train) => sum + Number(train.capacity || 0), 0),
    }),
    [fleet],
  )

  return (
    <>
      <header className="topbar">
        <div>
          <p className="eyebrow">Railway network</p>
          <h1>Fleet overview</h1>
        </div>
        <button type="button" onClick={refresh} disabled={status === 'loading'}>
          {status === 'loading' ? 'Loading...' : 'Refresh'}
        </button>
      </header>

      <section className="stats" aria-label="Fleet totals">
        <div>
          <span>Carriages</span>
          <strong>{totals.carriages}</strong>
        </div>
        <div>
          <span>Trains</span>
          <strong>{totals.trains}</strong>
        </div>
        <div>
          <span>Total capacity</span>
          <strong>{formatMetric(totals.capacity)}</strong>
        </div>
      </section>

      <section className="fleet-shell">
        {status === 'error' ? (
          <div className="empty-state" role="alert">
            <strong>Fleet could not be loaded</strong>
            <span>{error}</span>
          </div>
        ) : (
          <div className="fleet-grid">
            <div className="fleet-card">
              <h2>Trains</h2>
              {fleet.trains.length === 0 ? (
                <p className="fleet-empty">No trains returned by the backend.</p>
              ) : (
                <table className="fleet-table">
                  <thead>
                    <tr>
                      <th>ID</th>
                      <th>Capacity</th>
                      <th>Used weight</th>
                      <th>Utilization</th>
                    </tr>
                  </thead>
                  <tbody>
                    {fleet.trains.map((train) => {
                      const used = utilization(train)

                      return (
                        <tr key={train.id}>
                          <td>#{train.id}</td>
                          <td>{formatMetric(train.capacity)}</td>
                          <td>{formatMetric(train.used_weight)}</td>
                          <td>
                            {used === null ? (
                              '-'
                            ) : (
                              <div className="util">
                                <div className="util-bar">
                                  <span style={{ width: `${used}%` }} />
                                </div>
                                <span className="util-value">{used}%</span>
                              </div>
                            )}
                          </td>
                        </tr>
                      )
                    })}
                  </tbody>
                </table>
              )}
            </div>

            <div className="fleet-card">
              <h2>Carriages</h2>
              {fleet.carriages.length === 0 ? (
                <p className="fleet-empty">No carriages returned by the backend.</p>
              ) : (
                <table className="fleet-table">
                  <thead>
                    <tr>
                      <th>ID</th>
                      <th>Weight</th>
                    </tr>
                  </thead>
                  <tbody>
                    {fleet.carriages.map((carriage) => (
                      <tr key={carriage.id}>
                        <td>#{carriage.id}</td>
                        <td>{formatMetric(carriage.weight)}</td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              )}
            </div>
          </div>
        )}
      </section>
    </>
  )
}

export default FleetView
