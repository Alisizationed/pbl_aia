import { useCallback, useEffect, useMemo, useState } from 'react'
import {
  Background,
  Controls,
  MarkerType,
  MiniMap,
  ReactFlow,
  useEdgesState,
  useNodesState,
} from '@xyflow/react'
import '@xyflow/react/dist/style.css'
import './App.css'
import FleetView from './FleetView'

const API_URL = import.meta.env.VITE_API_URL ?? 'http://localhost:8000'

async function loadNetworkGraph() {
  const response = await fetch(`${API_URL}/network/graph`)

  if (!response.ok) {
    throw new Error(`Backend returned ${response.status}`)
  }

  return response.json()
}

function getNodePosition(index, total) {
  const radius = Math.max(180, Math.min(360, total * 42))
  const angle = total === 1 ? 0 : (index / total) * Math.PI * 2 - Math.PI / 2

  return {
    x: Math.round(Math.cos(angle) * radius),
    y: Math.round(Math.sin(angle) * radius),
  }
}

function formatMetric(value, suffix = '') {
  return Number.isFinite(Number(value)) ? `${Number(value).toLocaleString()}${suffix}` : '-'
}

function createFlowGraph(graph) {
  const nodes = graph.nodes.map((node, index) => ({
    id: String(node.id),
    type: 'default',
    position: getNodePosition(index, graph.nodes.length),
    data: {
      label: (
        <div className="node-label">
          <strong>{node.name}</strong>
          <span>#{node.id}</span>
        </div>
      ),
    },
  }))

  const nodeIds = new Set(nodes.map((node) => node.id))
  const edges = graph.edges
    .filter((edge) => nodeIds.has(String(edge.from_node_id)) && nodeIds.has(String(edge.to_node_id)))
    .map((edge) => ({
      id: String(edge.id),
      source: String(edge.from_node_id),
      target: String(edge.to_node_id),
      label: `cost ${formatMetric(edge.cost)}`,
      markerEnd: { type: MarkerType.ArrowClosed },
      data: edge,
    }))

  return { nodes, edges }
}

function App() {
  const [graph, setGraph] = useState({ nodes: [], edges: [] })
  const [nodes, setNodes, onNodesChange] = useNodesState([])
  const [edges, setEdges, onEdgesChange] = useEdgesState([])
  const [status, setStatus] = useState('loading')
  const [error, setError] = useState('')
  const [selectedEdge, setSelectedEdge] = useState(null)
  const [view, setView] = useState('graph')

  const refreshGraph = useCallback(() => {
    setStatus('loading')
    setError('')
    loadNetworkGraph()
      .then((nextGraph) => {
        const flowGraph = createFlowGraph(nextGraph)

        setGraph(nextGraph)
        setNodes(flowGraph.nodes)
        setEdges(flowGraph.edges)
        setSelectedEdge(null)
        setStatus('ready')
      })
      .catch((err) => {
        setStatus('error')
        setError(err instanceof Error ? err.message : 'Could not load graph')
      })
  }, [setEdges, setNodes])

  useEffect(() => {
    let ignore = false

    loadNetworkGraph()
      .then((nextGraph) => {
        if (ignore) {
          return
        }

        const flowGraph = createFlowGraph(nextGraph)

        setGraph(nextGraph)
        setNodes(flowGraph.nodes)
        setEdges(flowGraph.edges)
        setSelectedEdge(null)
        setStatus('ready')
      })
      .catch((err) => {
        if (ignore) {
          return
        }

        setStatus('error')
        setError(err instanceof Error ? err.message : 'Could not load graph')
      })

    return () => {
      ignore = true
    }
  }, [setEdges, setNodes])

  const totals = useMemo(
    () => ({
      nodes: graph.nodes.length,
      edges: graph.edges.length,
      capacity: graph.edges.reduce((sum, edge) => sum + Number(edge.capacity || 0), 0),
    }),
    [graph],
  )

  return (
    <main className="network-page">
      <nav className="view-tabs" aria-label="Views">
        <button
          type="button"
          className={view === 'graph' ? 'active' : ''}
          aria-pressed={view === 'graph'}
          onClick={() => setView('graph')}
        >
          Network graph
        </button>
        <button
          type="button"
          className={view === 'fleet' ? 'active' : ''}
          aria-pressed={view === 'fleet'}
          onClick={() => setView('fleet')}
        >
          Fleet
        </button>
      </nav>

      {view === 'fleet' ? <FleetView /> : null}

      {view === 'graph' ? (
        <>
      <header className="topbar">
        <div>
           <p className="eyebrow">Railway network</p>
          <h1>Graph overview</h1>
        </div>
        <button type="button" onClick={refreshGraph} disabled={status === 'loading'}>
          {status === 'loading' ? 'Loading...' : 'Refresh'}
        </button>
      </header>

      <section className="stats" aria-label="Network totals">
        <div>
          <span>Nodes</span>
          <strong>{totals.nodes}</strong>
        </div>
        <div>
          <span>Edges</span>
          <strong>{totals.edges}</strong>
        </div>
        <div>
          <span>Total capacity</span>
          <strong>{formatMetric(totals.capacity)}</strong>
        </div>
      </section>

       <section className="graph-shell">
        {status === 'error' ? (
          <div className="empty-state" role="alert">
            <strong>Graph could not be loaded</strong>
            <span>{error}</span>
          </div>
        ) : null}

        {status !== 'error' ? (
          <ReactFlow
            nodes={nodes}
            edges={edges}
            onNodesChange={onNodesChange}
            onEdgesChange={onEdgesChange}
            onEdgeClick={(_, edge) => setSelectedEdge(edge.data)}
            fitView
            fitViewOptions={{ padding: 0.2 }}
          >
            <Background gap={22} size={1} />
            <MiniMap pannable zoomable />
            <Controls />
          </ReactFlow>
        ) : null}

        <aside className="details-panel">
          <h2>Edge details</h2>
          {selectedEdge ? (
            <dl>
              <div>
                <dt>Route</dt>
                <dd>
                  {selectedEdge.from_node_id} {'->'} {selectedEdge.to_node_id}
                </dd>
              </div>
              <div>
                <dt>Cost</dt>
                <dd>{formatMetric(selectedEdge.cost)}</dd>
              </div>
              <div>
                <dt>Distance</dt>
                <dd>{formatMetric(selectedEdge.distance, ' km')}</dd>
              </div>
              <div>
                <dt>Capacity</dt>
                <dd>{formatMetric(selectedEdge.capacity)}</dd>
              </div>
              <div>
                <dt>Time</dt>
                <dd>{formatMetric(selectedEdge.time, ' h')}</dd>
              </div>
            </dl>
          ) : (
            <p>Select an edge to inspect the values returned by the backend.</p>
          )}
        </aside>
      </section>
        </>
      ) : null}
    </main>
  )
}

export default App
