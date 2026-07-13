#include "soundbridge/domain/PerfilArtista.hpp"

#include <ostream>
#include <stdexcept>
#include <utility>

#include "soundbridge/utils/TextoUtils.hpp"

namespace soundbridge {

PerfilArtista::PerfilArtista(
    int id,
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    std::string nombreArtistico,
    std::string instrumentoPrincipal,
    bool buscaColaboraciones
)
    : Perfil(
          id,
          std::move(nombre),
          edad,
          std::move(generoPrincipal),
          std::move(generosSecundarios)
      ),
      nombreArtistico_(std::move(nombreArtistico)),
      instrumentoPrincipal_(std::move(instrumentoPrincipal)),
      buscaColaboraciones_(buscaColaboraciones) {
    if (textoEstaVacio(nombreArtistico_)) {
        throw std::invalid_argument("El nombre artistico no puede estar vacio.");
    }

    if (textoEstaVacio(instrumentoPrincipal_)) {
        throw std::invalid_argument("El instrumento principal no puede estar vacio.");
    }
}

const std::string& PerfilArtista::obtenerNombreArtistico() const noexcept {
    return nombreArtistico_;
}

const std::string& PerfilArtista::obtenerInstrumentoPrincipal() const noexcept {
    return instrumentoPrincipal_;
}

bool PerfilArtista::buscaColaboraciones() const noexcept {
    return buscaColaboraciones_;
}

TipoPerfil PerfilArtista::obtenerTipo() const noexcept {
    return TipoPerfil::Artista;
}

void PerfilArtista::imprimir(std::ostream& salida) const {
    imprimirDatosComunes(salida);
    salida << "Nombre artistico: " << nombreArtistico_ << '\n';
    salida << "Instrumento principal: " << instrumentoPrincipal_ << '\n';
    salida << "Busca colaboraciones: "
           << (buscaColaboraciones_ ? "Si" : "No");
}

std::vector<std::string> PerfilArtista::obtenerCamposEspecificos() const {
    return {
        nombreArtistico_,
        instrumentoPrincipal_,
        buscaColaboraciones_ ? "1" : "0"
    };
}

}
