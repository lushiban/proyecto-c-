#include "soundbridge/domain/PerfilOyente.hpp"

#include <ostream>
#include <stdexcept>
#include <utility>

#include "soundbridge/utils/TextoUtils.hpp"

namespace soundbridge {

PerfilOyente::PerfilOyente(
    int id,
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    int horasEscuchaSemanales,
    std::string plataformaPreferida
)
    : Perfil(
          id,
          std::move(nombre),
          edad,
          std::move(generoPrincipal),
          std::move(generosSecundarios)
      ),
      horasEscuchaSemanales_(horasEscuchaSemanales),
      plataformaPreferida_(std::move(plataformaPreferida)) {
    if (horasEscuchaSemanales_ < 0 || horasEscuchaSemanales_ > 168) {
        throw std::invalid_argument(
            "Las horas de escucha deben estar entre 0 y 168."
        );
    }

    if (textoEstaVacio(plataformaPreferida_)) {
        throw std::invalid_argument("La plataforma preferida no puede estar vacia.");
    }
}

int PerfilOyente::obtenerHorasEscuchaSemanales() const noexcept {
    return horasEscuchaSemanales_;
}

const std::string& PerfilOyente::obtenerPlataformaPreferida() const noexcept {
    return plataformaPreferida_;
}

TipoPerfil PerfilOyente::obtenerTipo() const noexcept {
    return TipoPerfil::Oyente;
}

void PerfilOyente::imprimir(std::ostream& salida) const {
    imprimirDatosComunes(salida);
    salida << "Horas de escucha semanales: " << horasEscuchaSemanales_ << '\n';
    salida << "Plataforma preferida: " << plataformaPreferida_;
}

std::vector<std::string> PerfilOyente::obtenerCamposEspecificos() const {
    return {
        std::to_string(horasEscuchaSemanales_),
        plataformaPreferida_
    };
}

}
