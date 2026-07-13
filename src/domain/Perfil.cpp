#include "soundbridge/domain/Perfil.hpp"

#include <ostream>
#include <stdexcept>
#include <utility>

#include "soundbridge/utils/TextoUtils.hpp"

namespace soundbridge {

Perfil::Perfil(
    int id,
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios
)
    : id_(id),
      nombre_(std::move(nombre)),
      edad_(edad),
      generoPrincipal_(std::move(generoPrincipal)),
      generosSecundarios_(std::move(generosSecundarios)) {
    if (id_ <= 0) {
        throw std::invalid_argument("El ID debe ser mayor que cero.");
    }

    if (textoEstaVacio(nombre_)) {
        throw std::invalid_argument("El nombre no puede estar vacio.");
    }

    if (edad_ < 13 || edad_ > 120) {
        throw std::invalid_argument("La edad debe estar entre 13 y 120.");
    }

    if (textoEstaVacio(generoPrincipal_)) {
        throw std::invalid_argument("El genero principal no puede estar vacio.");
    }

    if (generosSecundarios_.empty()) {
        throw std::invalid_argument("Debe registrar al menos un genero secundario.");
    }

    for (const std::string& genero : generosSecundarios_) {
        if (textoEstaVacio(genero)) {
            throw std::invalid_argument("Los generos secundarios no pueden estar vacios.");
        }
    }

    if (tieneTextosDuplicados(generosSecundarios_)) {
        throw std::invalid_argument("Los generos secundarios no pueden repetirse.");
    }

    if (contieneTexto(generosSecundarios_, generoPrincipal_)) {
        throw std::invalid_argument(
            "El genero principal no puede repetirse como secundario."
        );
    }
}

int Perfil::obtenerId() const noexcept {
    return id_;
}

const std::string& Perfil::obtenerNombre() const noexcept {
    return nombre_;
}

int Perfil::obtenerEdad() const noexcept {
    return edad_;
}

const std::string& Perfil::obtenerGeneroPrincipal() const noexcept {
    return generoPrincipal_;
}

const std::vector<std::string>& Perfil::obtenerGenerosSecundarios() const noexcept {
    return generosSecundarios_;
}

void Perfil::imprimirDatosComunes(std::ostream& salida) const {
    salida << "ID: " << id_ << '\n';
    salida << "Tipo: " << tipoPerfilATexto(obtenerTipo()) << '\n';
    salida << "Nombre: " << nombre_ << '\n';
    salida << "Edad: " << edad_ << '\n';
    salida << "Genero principal: " << generoPrincipal_ << '\n';
    salida << "Generos secundarios: ";

    for (std::size_t i = 0; i < generosSecundarios_.size(); i++) {
        salida << generosSecundarios_[i];

        if (i + 1 < generosSecundarios_.size()) {
            salida << ", ";
        }
    }

    salida << '\n';
}

std::ostream& operator<<(std::ostream& salida, const Perfil& perfil) {
    perfil.imprimir(salida);
    return salida;
}

}
