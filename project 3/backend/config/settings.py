from pydantic import BaseModel
import os


class Settings(BaseModel):
    DATABASE_URL: str = os.getenv("DATABASE_URL")

    KEYCLOAK_URL: str = os.getenv("KEYCLOAK_URL")
    KEYCLOAK_REALM: str = os.getenv("KEYCLOAK_REALM")
    KEYCLOAK_CLIENT_ID: str = os.getenv("KEYCLOAK_CLIENT_ID")

    @property
    def JWKS_URL(self):
        return f"{self.KEYCLOAK_URL}/realms/{self.KEYCLOAK_REALM}/protocol/openid-connect/certs"

    @property
    def ISSUER(self):
        return f"{self.KEYCLOAK_URL}/realms/{self.KEYCLOAK_REALM}"


settings = Settings()