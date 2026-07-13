#ifndef RSM_DOMAIN_PERFIL_OYENTE_HPP
#define RSM_DOMAIN_PERFIL_OYENTE_HPP

#include <string>
#include <vector>

#include "rsm/domain/Perfil.hpp"

namespace rsm {

class PerfilOyente final : public Perfil {
private:
    int horasEscuchaSemanales_;
    std::string plataformaPreferida_;

public:
    PerfilOyente(
        int id,
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        int horasEscuchaSemanales,
        std::string plataformaPreferida
    );

    int obtenerHorasEscuchaSemanales() const noexcept;
    const std::string& obtenerPlataformaPreferida() const noexcept;

    TipoPerfil obtenerTipo() const noexcept override;
    void imprimir(std::ostream& salida) const override;
    std::vector<std::string> obtenerCamposEspecificos() const override;
};

}

#endif
