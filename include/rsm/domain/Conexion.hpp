#ifndef RSM_DOMAIN_CONEXION_HPP
#define RSM_DOMAIN_CONEXION_HPP

namespace rsm {

class Conexion {
private:
    int perfilAId_;
    int perfilBId_;
    int porcentajeAfinidad_;

public:
    Conexion(int perfilAId, int perfilBId, int porcentajeAfinidad);

    int obtenerPerfilAId() const noexcept;
    int obtenerPerfilBId() const noexcept;
    int obtenerPorcentajeAfinidad() const noexcept;

    bool contienePerfil(int perfilId) const noexcept;
    bool conectaPerfiles(int primerPerfilId, int segundoPerfilId) const noexcept;
};

}

#endif
