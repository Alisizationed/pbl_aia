from fastapi import Depends, HTTPException
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from jose import jwt
import httpx

from auth.keycloak_config import JWKS_URL, ISSUER, CLIENT_ID

security = HTTPBearer()

_jwks = None


async def get_jwks():
    global _jwks
    if _jwks is None:
        async with httpx.AsyncClient() as client:
            resp = await client.get(JWKS_URL)
            _jwks = resp.json()
    return _jwks


def get_public_key(jwks, token):
    headers = jwt.get_unverified_header(token)
    kid = headers["kid"]

    for key in jwks["keys"]:
        if key["kid"] == kid:
            return key

    raise HTTPException(status_code=401, detail="Invalid token key")


async def verify_token(credentials: HTTPAuthorizationCredentials = Depends(security)):
    token = credentials.credentials
    jwks = await get_jwks()
    key = get_public_key(jwks, token)

    try:
        payload = jwt.decode(
            token,
            key,
            algorithms=["RS256"],
            audience=CLIENT_ID,
            issuer=ISSUER,
            options={"verify_aud": False}
        )
        return payload

    except Exception:
        raise HTTPException(status_code=401, detail="Invalid or expired token")
