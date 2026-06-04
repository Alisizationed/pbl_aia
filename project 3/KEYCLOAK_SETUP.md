# Keycloak Setup Guide

After running `docker compose up`, open http://localhost:8080 and log in with
**admin / admin**.

---

## 1. Create the realm

1. Click **Create realm** (top-left dropdown)
2. Name it **railway**
3. Click **Create**

---

## 2. Create the client

1. Go to **Clients → Create client**
2. **Client ID**: `frontend`
3. **Client type**: `OpenID Connect`
4. Click **Next**
5. **Client authentication**: OFF (public client — no secret needed)
6. **Authentication flow**: Standard flow ON, Direct access grants ON
7. Click **Next**
8. **Valid redirect URIs**: `http://localhost:5173/*`
9. **Web origins**: `http://localhost:5173`
10. Click **Save**

---

## 3. Create realm roles

1. Go to **Realm roles → Create role**
2. Create two roles: `admin` and `operator`

---

## 4. Create users

1. Go to **Users → Create user**
2. Fill in **Username** (e.g. `alice`), click **Create**
3. Go to the **Credentials** tab → **Set password** (turn off Temporary)
4. Go to the **Role mapping** tab → assign `admin` or `operator`

---

## 5. Backend environment variables

When running the backend locally (outside Docker), create a `.env` file
in `backend/` with:

```
DATABASE_URL=postgresql://railway_user:railway_password@localhost:5432/railway_db
KEYCLOAK_URL=http://localhost:8080
KEYCLOAK_REALM=railway
KEYCLOAK_CLIENT_ID=frontend
```

When running inside Docker Compose, these are already set in `docker-compose.yaml`.

---

## 6. Frontend environment variables

The file `frontend/front-app/.env` already has the right defaults for local dev.
Copy `.env.example` if you ever need to reset them.
