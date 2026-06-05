from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    DATABASE_URL: str
    KEYCLOAK_URL: str
    KEYCLOAK_REALM: str
    KEYCLOAK_CLIENT_ID: str
    ISSUER: str = ""

    @property
    def JWKS_URL(self):
        return f"{self.KEYCLOAK_URL}/realms/{self.KEYCLOAK_REALM}/protocol/openid-connect/certs"

    @property
    def effective_issuer(self):
        return self.ISSUER or f"{self.KEYCLOAK_URL}/realms/{self.KEYCLOAK_REALM}"

    class Config:
        env_file = ".env"

settings = Settings()