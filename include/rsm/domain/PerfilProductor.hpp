#ifndef RSM_DOMAIN_PERFIL_PRODUCTOR_HPP
#define RSM_DOMAIN_PERFIL_PRODUCTOR_HPP

#include <string>
#include <vector>

#include "rsm/domain/Perfil.hpp"

namespace rsm {

class PerfilProductor final : public Perfil {
private:
    std::string softwarePrincipal_;
    int aniosExperiencia_;
    bool ofreceServicios_;

public:
    PerfilProductor(
        int id,
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        std::string softwarePrincipal,
        int aniosExperiencia,
        bool ofreceServicios
    );

    const std::string& obtenerSoftwarePrincipal() const noexcept;
    int obtenerAniosExperiencia() const noexcept;
    bool ofreceServicios() const noexcept;

    TipoPerfil obtenerTipo() const noexcept override;
    void imprimir(std::ostream& salida) const override;
    std::vector<std::string> obtenerCamposEspecificos() const override;
};

}

#endif
