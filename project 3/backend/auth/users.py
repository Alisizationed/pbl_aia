from fastapi import Depends
from auth.auth_bearer import verify_token


def get_current_user(payload=Depends(verify_token)):
    return {
        "user_id": payload.get("sub"),
        "username": payload.get("preferred_username"),
        "roles": payload.get("realm_access", {}).get("roles", [])
    }