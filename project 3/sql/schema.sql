DROP TABLE edges CASCADE;
DROP TABLE nodes CASCADE;

CREATE TABLE IF NOT EXISTS nodes (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS edges (
    id SERIAL PRIMARY KEY,

    from_node_id INTEGER NOT NULL,
    to_node_id INTEGER NOT NULL,

    cost DOUBLE PRECISION NOT NULL,
    distance DOUBLE PRECISION NOT NULL,
    capacity INTEGER NOT NULL,
    time DOUBLE PRECISION NOT NULL,

    CONSTRAINT fk_edges_from_node
        FOREIGN KEY (from_node_id)
        REFERENCES nodes(id),

    CONSTRAINT fk_edges_to_node
        FOREIGN KEY (to_node_id)
        REFERENCES nodes(id)
);

CREATE TABLE IF NOT EXISTS edge_time_windows (
    id SERIAL PRIMARY KEY,

    edge_id INTEGER NOT NULL,

    valid_from TIMESTAMP NOT NULL,
    valid_until TIMESTAMP NOT NULL,

    CONSTRAINT fk_edge_time_windows_edge
        FOREIGN KEY (edge_id)
        REFERENCES edges(id)
        ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS carriages (
    id SERIAL PRIMARY KEY,
    weight DOUBLE PRECISION NOT NULL
);

CREATE TABLE IF NOT EXISTS trains (
    id SERIAL PRIMARY KEY,
    capacity DOUBLE PRECISION NOT NULL,
    used_weight DOUBLE PRECISION NOT NULL
);

CREATE INDEX IF NOT EXISTS idx_edges_from_node
    ON edges(from_node_id);

CREATE INDEX IF NOT EXISTS idx_edges_to_node
    ON edges(to_node_id);

CREATE INDEX IF NOT EXISTS idx_edge_time_windows_edge
    ON edge_time_windows(edge_id);