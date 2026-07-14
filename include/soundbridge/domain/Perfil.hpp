#ifndef SOUNDBRIDGE_DOMAIN_PERFIL_HPP
#define SOUNDBRIDGE_DOMAIN_PERFIL_HPP

#include <iosfwd>
#include <string>
#include <vector>

#include "soundbridge/domain/TipoPerfil.hpp"

namespace soundbridge {

class Perfil {
private:
    int id_;
    std::string nombre_;
    int edad_;
    std::string generoPrincipal_;
    std::vector<std::string> generosSecundarios_;

protected:
    void imprimirDatosComunes(std::ostream& salida) const;

public:
    Perfil(
        int id,
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios
    );

    virtual ~Perfil() = default;

    int obtenerId() const noexcept;
    const std::string& obtenerNombre() const noexcept;
    int obtenerEdad() const noexcept;
    const std::string& obtenerGeneroPrincipal() const noexcept;
    const std::vector<std::string>& obtenerGenerosSecundarios() const noexcept;

    virtual TipoPerfil obtenerTipo() const noexcept = 0;
    virtual void imprimir(std::ostream& salida) const = 0;
    virtual std::vector<std::string> obtenerCamposEspecificos() const = 0;
};

std::ostream& operator<<(std::ostream& salida, const Perfil& perfil);

}

#endif
