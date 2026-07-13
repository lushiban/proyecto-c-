#include "rsm/domain/PerfilFanClub.hpp"

#include <ostream>
#include <stdexcept>
#include <utility>

#include "rsm/utils/TextoUtils.hpp"

namespace rsm {

PerfilFanClub::PerfilFanClub(
    int id,
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    std::string artistaRepresentado,
    int numeroMiembros,
    std::string ciudadBase
)
    : Perfil(
          id,
          std::move(nombre),
          edad,
          std::move(generoPrincipal),
          std::move(generosSecundarios)
      ),
      artistaRepresentado_(std::move(artistaRepresentado)),
      numeroMiembros_(numeroMiembros),
      ciudadBase_(std::move(ciudadBase)) {
    if (textoEstaVacio(artistaRepresentado_)) {
        throw std::invalid_argument("El artista representado no puede estar vacio.");
    }

    if (numeroMiembros_ < 1) {
        throw std::invalid_argument("El numero de miembros debe ser mayor que cero.");
    }

    if (textoEstaVacio(ciudadBase_)) {
        throw std::invalid_argument("La ciudad base no puede estar vacia.");
    }
}

const std::string& PerfilFanClub::obtenerArtistaRepresentado() const noexcept {
    return artistaRepresentado_;
}

int PerfilFanClub::obtenerNumeroMiembros() const noexcept {
    return numeroMiembros_;
}

const std::string& PerfilFanClub::obtenerCiudadBase() const noexcept {
    return ciudadBase_;
}

TipoPerfil PerfilFanClub::obtenerTipo() const noexcept {
    return TipoPerfil::FanClub;
}

void PerfilFanClub::imprimir(std::ostream& salida) const {
    imprimirDatosComunes(salida);
    salida << "Artista representado: " << artistaRepresentado_ << '\n';
    salida << "Numero de miembros: " << numeroMiembros_ << '\n';
    salida << "Ciudad base: " << ciudadBase_;
}

std::vector<std::string> PerfilFanClub::obtenerCamposEspecificos() const {
    return {
        artistaRepresentado_,
        std::to_string(numeroMiembros_),
        ciudadBase_
    };
}

}
