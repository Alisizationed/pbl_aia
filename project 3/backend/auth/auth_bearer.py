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
        print(f"JWKS fetched from {JWKS_URL}")
        print(f"Available kids: {[k['kid'] for k in _jwks['keys']]}")
    return _jwks


def get_public_key(jwks, token):
    headers = jwt.get_unverified_header(token)
    kid = headers["kid"]
    print(f"Token kid: {kid}")

    for key in jwks["keys"]:
        if key["kid"] == kid:
            print(f"Key matched: {kid}")
            return key

    print(f"No key matched! Available: {[k['kid'] for k in jwks['keys']]}")
    raise HTTPException(status_code=401, detail="Invalid token key")


async def verify_token(credentials: HTTPAuthorizationCredentials = Depends(security)):
    token = credentials.credentials
    print(f"Token received: {token[:60]}...")
    print(f"ISSUER={ISSUER}  CLIENT_ID={CLIENT_ID}  JWKS_URL={JWKS_URL}")

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
        print(f"Token valid for user: {payload.get('preferred_username')}")
        return payload

    except Exception as e:
        print(f"JWT decode error: {type(e).__name__}: {e}")
        raise HTTPException(status_code=401, detail=str(e))