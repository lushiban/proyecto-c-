#ifndef SOUNDBRIDGE_DOMAIN_PERFIL_ARTISTA_HPP
#define SOUNDBRIDGE_DOMAIN_PERFIL_ARTISTA_HPP

#include <string>
#include <vector>

#include "soundbridge/domain/Perfil.hpp"

namespace soundbridge {

class PerfilArtista final : public Perfil {
private:
    std::string nombreArtistico_;
    std::string instrumentoPrincipal_;
    bool buscaColaboraciones_;

public:
    PerfilArtista(
        int id,
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        std::string nombreArtistico,
        std::string instrumentoPrincipal,
        bool buscaColaboraciones
    );

    const std::string& obtenerNombreArtistico() const noexcept;
    const std::string& obtenerInstrumentoPrincipal() const noexcept;
    bool buscaColaboraciones() const noexcept;

    TipoPerfil obtenerTipo() const noexcept override;
    void imprimir(std::ostream& salida) const override;
    std::vector<std::string> obtenerCamposEspecificos() const override;
};

}

#endif
