from fastapi import Depends, HTTPException
from auth.users import get_current_user


def require_role(role: str):
    def checker(user=Depends(get_current_user)):
        if role not in user["roles"]:
            raise HTTPException(
                status_code=403,
                detail="Not enough permissions"
            )
        return user

    return checker