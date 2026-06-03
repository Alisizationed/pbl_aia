import {useCallback, useEffect, useMemo, useState} from 'react'
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
import API from '../api/api'

const API_URL = import.meta.env.VITE_API_URL ?? 'http://localhost:8000'

function getNodePosition(index, total) {
    const radius = Math.max(180, Math.min(360, total * 42))
    const angle = total === 1 ? 0 : (index / total) * Math.PI * 2 - Math.PI / 2
    return {
        x: Math.round(Math.cos(angle) * radius),
        y: Math.round(Math.sin(angle) * radius),
    }
}

function formatMetric(value, suffix = '') {
    return Number.isFinite(Number(value)) ? `${Number(value).toLocaleString()}${suffix}` : '—'
}

function buildFlowNodes(graphNodes, startId, endId) {
    return graphNodes.map((node, index) => {
        const isStart = node.id === startId
        const isEnd = node.id === endId
        let style = {}
        if (isStart) style = {background: '#dcfce7', border: '2px solid #16a34a'}
        else if (isEnd) style = {background: '#fee2e2', border: '2px solid #dc2626'}

        return {
            id: String(node.id),
            type: 'default',
            position: getNodePosition(index, graphNodes.length),
            style,
            data: {
                node,
                label: (
                    <div className="node-label">
                        <strong>{node.name}</strong>
                        <span>
              #{node.id}
                            {isStart ? ' 🟢' : ''}
                            {isEnd ? ' 🔴' : ''}
            </span>
                    </div>
                ),
            },
        }
    })
}

function buildFlowEdges(graphEdges, graphNodes) {
    const nodeIds = new Set(graphNodes.map((n) => String(n.id)))
    return graphEdges
        .filter((e) => nodeIds.has(String(e.from_node_id)) && nodeIds.has(String(e.to_node_id)))
        .map((e) => ({
            id: String(e.id),
            source: String(e.from_node_id),
            target: String(e.to_node_id),
            label: `${formatMetric(e.cost)} cost`,
            markerEnd: {type: MarkerType.ArrowClosed},
            data: e,
        }))
}

function TrainRouteCard({trainId, data, selected, onSelect, nodeMap}) {
    const {carriages, route} = data
    const carriagesStr = carriages.map(c => `#${c.id} (${c.weight})`).join(', ')
    const pathStr = route.path
        .map((e, i) => {
            const fromName = nodeMap[e.from_node_id] ?? e.from_node_id
            const toName = nodeMap[e.to_node_id] ?? e.to_node_id
            return i === 0 ? `${fromName} → ${toName}` : `→ ${toName}`
        })
        .join(' ')

    return (
        <div className={`route-card ${selected ? 'selected' : ''}`} onClick={onSelect}>
            <div className="train-badge">Train #{trainId}</div>
            <div>
                <div className="route-metrics">
                    <div className="route-metric">
                        <span>Cost</span>
                        <strong>{formatMetric(route.cost)}</strong>
                    </div>
                    <div className="route-metric">
                        <span>Distance</span>
                        <strong>{formatMetric(route.distance, ' km')}</strong>
                    </div>
                    <div className="route-metric">
                        <span>Time</span>
                        <strong>{formatMetric(route.time, ' h')}</strong>
                    </div>
                    <div className="route-metric">
                        <span>Stops</span>
                        <strong>{route.path.length}</strong>
                    </div>
                </div>
                <div className="carriages-info">🚆 Carriages: {carriagesStr || 'none'}</div>
                {pathStr && <div className="route-path">{pathStr}</div>}
            </div>
            {/* <button
                className={`btn-sm ${selected ? '' : 'btn-ghost'}`}
                style={{flexShrink: 0}}
                onClick={(e) => {
                    e.stopPropagation()
                    onSelect()
                }}
            >
                {selected ? '✓ Selected' : 'Select'}
            </button> */}
        </div>
    )
}

function EnsembleCard({ensemble, ensembleIndex, selected, onSelect, nodeMap}) {
    const trainIds = Object.keys(ensemble)
    return (
        <div className={`ensemble-card ${selected ? 'selected' : ''}`} onClick={onSelect}>
            <div className="ensemble-header">
                <strong>Ensemble #{ensembleIndex + 1}</strong>
                <span className="badge">{trainIds.length} trains</span>
            </div>
            <div className="train-routes-list">
                {trainIds.map(trainId => (
                    <TrainRouteCard
                        key={trainId}
                        trainId={trainId}
                        data={ensemble[trainId]}
                        selected={false}
                        onSelect={() => {}}
                        nodeMap={nodeMap}
                    />
                ))}
            </div>
            <button
                className="btn-sm btn-ghost"
                style={{marginTop: 12, alignSelf: 'flex-end'}}
                onClick={(e) => {
                    e.stopPropagation()
                    onSelect()
                }}
            >
                {selected ? '✓ Selected' : 'Select this ensemble'}
            </button>
        </div>
    )
}

export default function OptimizePage() {
    const [graphData, setGraphData] = useState({nodes: [], edges: []})
    const [flowNodes, setFlowNodes, onNodesChange] = useNodesState([])
    const [flowEdges, setFlowEdges, onEdgesChange] = useEdgesState([])
    const [graphStatus, setGraphStatus] = useState('loading')

    const [startNode, setStartNode] = useState(null)
    const [endNode, setEndNode] = useState(null)
    const [pickMode, setPickMode] = useState('start')

    const [trains, setTrains] = useState([])
    const [carriages, setCarriages] = useState([])
    const [selTrains, setSelTrains] = useState([])
    const [selCarriages, setSelCarriages] = useState([])
    const [departure, setDeparture] = useState('')

    const [ensembles, setEnsembles] = useState([])       // массив ансамблей
    const [selectedEnsembleIdx, setSelectedEnsembleIdx] = useState(null)
    const [loading, setLoading] = useState(false)
    const [error, setError] = useState('')

    const nodeMap = useMemo(() => {
        const m = {}
        graphData.nodes.forEach((n) => { m[n.id] = n.name })
        return m
    }, [graphData.nodes])

    useEffect(() => {
        let ignore = false
        fetch(`${API_URL}/network/graph`)
            .then((r) => {
                if (!r.ok) throw new Error(r.status)
                return r.json()
            })
            .then((g) => {
                if (ignore) return
                setGraphData(g)
                setFlowNodes(buildFlowNodes(g.nodes, null, null))
                setFlowEdges(buildFlowEdges(g.edges, g.nodes))
                setGraphStatus('ready')
            })
            .catch(() => {
                if (!ignore) setGraphStatus('error')
            })

        API.get('/trains/').then((r) => setTrains(r.data)).catch(() => {})
        API.get('/carriages/').then((r) => setCarriages(r.data)).catch(() => {})

        return () => { ignore = true }
    }, [setFlowEdges, setFlowNodes])

    useEffect(() => {
        if (!graphData.nodes.length) return
        setFlowNodes(buildFlowNodes(graphData.nodes, startNode?.id ?? null, endNode?.id ?? null))
    }, [startNode, endNode, graphData.nodes, setFlowNodes])

    const handleNodeClick = useCallback((_, node) => {
        const raw = node.data.node
        if (pickMode === 'start') {
            setStartNode(raw)
            setPickMode('end')
        } else if (pickMode === 'end') {
            if (raw.id === startNode?.id) return
            setEndNode(raw)
            setPickMode(null)
        }
    }, [pickMode, startNode])

    const clearSelection = () => {
        setStartNode(null)
        setEndNode(null)
        setPickMode('start')
        setEnsembles([])
        setSelectedEnsembleIdx(null)
    }

    const toggleTrain = (id) =>
        setSelTrains((prev) => prev.includes(id) ? prev.filter((x) => x !== id) : [...prev, id])

    const toggleCarriage = (id) =>
        setSelCarriages((prev) => prev.includes(id) ? prev.filter((x) => x !== id) : [...prev, id])

    const canSubmit = startNode && endNode && departure && selTrains.length > 0 && selCarriages.length > 0 && !loading

    const handleSubmit = async () => {
        if (!canSubmit) return
        setLoading(true)
        setError('')
        setEnsembles([])
        setSelectedEnsembleIdx(null)

        try {
            const body = {
                start: startNode,
                end: endNode,
                train_ids: selTrains,
                carriage_ids: selCarriages,
                departure_time: new Date(departure).toISOString(),
            }
            const res = await API.post('/optimize', body)
            const receivedEnsembles = res.data.ensembles || []
            setEnsembles(receivedEnsembles)
            if (receivedEnsembles.length > 0) setSelectedEnsembleIdx(0)
        } catch (err) {
            setError(err?.response?.data?.detail ?? 'Optimization failed. Check your inputs and backend.')
        } finally {
            setLoading(false)
        }
    }

    useEffect(() => {
        if (!graphData.edges.length || selectedEnsembleIdx === null || !ensembles[selectedEnsembleIdx]) {
            if (flowEdges.length && graphData.edges.length) {
                setFlowEdges(buildFlowEdges(graphData.edges, graphData.nodes))
            }
            return
        }
        const selectedEnsemble = ensembles[selectedEnsembleIdx]
        const usedEdgeIds = new Set()
        Object.values(selectedEnsemble).forEach(({route}) => {
            route.path.forEach(edge => usedEdgeIds.add(edge.id))
        })
        const newEdges = buildFlowEdges(graphData.edges, graphData.nodes).map(e => ({
            ...e,
            style: usedEdgeIds.has(Number(e.id))
                ? {stroke: '#f59e0b', strokeWidth: 4}
                : {stroke: '#94a3b8', strokeWidth: 1.5},
            animated: usedEdgeIds.has(Number(e.id))
        }))
        setFlowEdges(newEdges)
    }, [selectedEnsembleIdx, ensembles, graphData.edges, graphData.nodes, setFlowEdges])

    return (
        <main className="page-shell">
            <header className="topbar">
                <div>
                    <p className="eyebrow">Route planner</p>
                    <h1>Optimize routes</h1>
                </div>
                <button onClick={clearSelection} className="btn-ghost" style={{fontSize: 14, minHeight: 36}}>
                    Reset
                </button>
            </header>

            <div className="optimize-layout">
                <div className="optimize-graph-pane">
                    {graphStatus === 'error' ? (
                        <div className="empty-state" role="alert">
                            <strong>Graph could not be loaded</strong>
                            <span>Is the backend running?</span>
                        </div>
                    ) : (
                        <ReactFlow
                            nodes={flowNodes}
                            edges={flowEdges}
                            onNodesChange={onNodesChange}
                            onEdgesChange={onEdgesChange}
                            onNodeClick={handleNodeClick}
                            fitView
                            fitViewOptions={{padding: 0.2}}
                            style={{cursor: pickMode ? 'crosshair' : 'default'}}
                        >
                            <Background gap={22} size={1}/>
                            <MiniMap pannable zoomable/>
                            <Controls/>
                        </ReactFlow>
                    )}
                </div>

                <aside className="optimize-panel">
                    <div className="opt-section">
                        <h3>Route endpoints</h3>
                        <p className="node-picker-hint">
                            {!startNode
                                ? '① Click a node on the graph to set the start point.'
                                : !endNode
                                    ? '② Click another node to set the end point.'
                                    : 'Both endpoints are set. Fill in the form below and run.'}
                        </p>
                        <div className="node-pills-row">
                            <div
                                className={`node-pill${startNode ? ' set-start' : ''}`}
                                style={{cursor: 'pointer'}}
                                title="Click to re-pick start"
                                onClick={() => {
                                    setStartNode(null)
                                    setEndNode(null)
                                    setPickMode('start')
                                }}
                            >
                                🟢 Start: {startNode ? startNode.name : '—'}
                            </div>
                            <div
                                className={`node-pill${endNode ? ' set-end' : ''}`}
                                style={{cursor: 'pointer'}}
                                title="Click to re-pick end"
                                onClick={() => {
                                    setEndNode(null)
                                    setPickMode('end')
                                }}
                            >
                                🔴 End: {endNode ? endNode.name : '—'}
                            </div>
                        </div>
                    </div>

                    <div className="opt-section">
                        <h3>Departure time</h3>
                        <label>
                            Date & time
                            <input
                                type="datetime-local"
                                value={departure}
                                onChange={(e) => setDeparture(e.target.value)}
                                style={{width: '100%'}}
                            />
                        </label>
                    </div>

                    <div className="opt-section">
                        <h3>Trains</h3>
                        {trains.length === 0 ? (
                            <p className="node-picker-hint">No trains loaded.</p>
                        ) : (
                            <div className="multi-select-list">
                                {trains.map((t) => (
                                    <label key={t.id}>
                                        <input
                                            type="checkbox"
                                            checked={selTrains.includes(t.id)}
                                            onChange={() => toggleTrain(t.id)}
                                        />
                                        Train #{t.id} — capacity {t.capacity}, used {t.used_weight}
                                    </label>
                                ))}
                            </div>
                        )}
                    </div>

                    <div className="opt-section">
                        <h3>Carriages</h3>
                        {carriages.length === 0 ? (
                            <p className="node-picker-hint">No carriages loaded.</p>
                        ) : (
                            <div className="multi-select-list">
                                {carriages.map((c) => (
                                    <label key={c.id}>
                                        <input
                                            type="checkbox"
                                            checked={selCarriages.includes(c.id)}
                                            onChange={() => toggleCarriage(c.id)}
                                        />
                                        Carriage #{c.id} — weight {c.weight}
                                    </label>
                                ))}
                            </div>
                        )}
                    </div>

                    {error && (
                        <div className="alert alert-error" style={{margin: '0 24px'}}>
                            {error}
                        </div>
                    )}

                    <button className="opt-run-btn" onClick={handleSubmit} disabled={!canSubmit}>
                        {loading ? 'Running optimizer…' : 'Find optimal routes'}
                    </button>
                </aside>
            </div>

            {/* Результаты: список ансамблей */}
            {ensembles.length > 0 && (
                <section className="results-pane">
                    <h2>Optimal ensembles</h2>
                    <div className="ensembles-list">
                        {ensembles.map((ensemble, idx) => (
                            <EnsembleCard
                                key={idx}
                                ensemble={ensemble}
                                ensembleIndex={idx}
                                selected={selectedEnsembleIdx === idx}
                                onSelect={() => setSelectedEnsembleIdx(idx)}
                                nodeMap={nodeMap}
                            />
                        ))}
                    </div>
                </section>
            )}

            {ensembles.length === 0 && !loading && (
                <section className="results-pane">
                    <div className="empty-state">
                        <strong>No ensembles found</strong>
                        <span>Try different nodes, carriages, or departure time.</span>
                    </div>
                </section>
            )}
        </main>
    )
}