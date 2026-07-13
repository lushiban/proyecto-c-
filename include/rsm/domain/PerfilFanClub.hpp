#ifndef RSM_DOMAIN_PERFIL_FAN_CLUB_HPP
#define RSM_DOMAIN_PERFIL_FAN_CLUB_HPP

#include <string>
#include <vector>

#include "rsm/domain/Perfil.hpp"

namespace rsm {

class PerfilFanClub final : public Perfil {
private:
    std::string artistaRepresentado_;
    int numeroMiembros_;
    std::string ciudadBase_;

public:
    PerfilFanClub(
        int id,
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        std::string artistaRepresentado,
        int numeroMiembros,
        std::string ciudadBase
    );

    const std::string& obtenerArtistaRepresentado() const noexcept;
    int obtenerNumeroMiembros() const noexcept;
    const std::string& obtenerCiudadBase() const noexcept;

    TipoPerfil obtenerTipo() const noexcept override;
    void imprimir(std::ostream& salida) const override;
    std::vector<std::string> obtenerCamposEspecificos() const override;
};

}

#endif
