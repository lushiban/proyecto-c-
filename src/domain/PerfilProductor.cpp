#include "rsm/domain/PerfilProductor.hpp"

#include <ostream>
#include <stdexcept>
#include <utility>

#include "rsm/utils/TextoUtils.hpp"

namespace rsm {

PerfilProductor::PerfilProductor(
    int id,
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    std::string softwarePrincipal,
    int aniosExperiencia,
    bool ofreceServicios
)
    : Perfil(
          id,
          std::move(nombre),
          edad,
          std::move(generoPrincipal),
          std::move(generosSecundarios)
      ),
      softwarePrincipal_(std::move(softwarePrincipal)),
      aniosExperiencia_(aniosExperiencia),
      ofreceServicios_(ofreceServicios) {
    if (textoEstaVacio(softwarePrincipal_)) {
        throw std::invalid_argument("El software principal no puede estar vacio.");
    }

    if (aniosExperiencia_ < 0 || aniosExperiencia_ > 80) {
        throw std::invalid_argument(
            "Los anios de experiencia deben estar entre 0 y 80."
        );
    }
}

const std::string& PerfilProductor::obtenerSoftwarePrincipal() const noexcept {
    return softwarePrincipal_;
}

int PerfilProductor::obtenerAniosExperiencia() const noexcept {
    return aniosExperiencia_;
}

bool PerfilProductor::ofreceServicios() const noexcept {
    return ofreceServicios_;
}

TipoPerfil PerfilProductor::obtenerTipo() const noexcept {
    return TipoPerfil::Productor;
}

void PerfilProductor::imprimir(std::ostream& salida) const {
    imprimirDatosComunes(salida);
    salida << "Software principal: " << softwarePrincipal_ << '\n';
    salida << "Anios de experiencia: " << aniosExperiencia_ << '\n';
    salida << "Ofrece servicios: " << (ofreceServicios_ ? "Si" : "No");
}

std::vector<std::string> PerfilProductor::obtenerCamposEspecificos() const {
    return {
        softwarePrincipal_,
        std::to_string(aniosExperiencia_),
        ofreceServicios_ ? "1" : "0"
    };
}

}
