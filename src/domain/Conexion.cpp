#include "soundbridge/domain/Conexion.hpp"

#include <stdexcept>
#include <utility>

namespace soundbridge {

Conexion::Conexion(
    int perfilAId,
    int perfilBId,
    int porcentajeAfinidad
)
    : perfilAId_(perfilAId),
      perfilBId_(perfilBId),
      porcentajeAfinidad_(porcentajeAfinidad) {
    if (perfilAId_ <= 0 || perfilBId_ <= 0) {
        throw std::invalid_argument("Los IDs de la conexion deben ser positivos.");
    }

    if (perfilAId_ == perfilBId_) {
        throw std::invalid_argument("Una conexion necesita dos perfiles diferentes.");
    }

    if (porcentajeAfinidad_ < 0 || porcentajeAfinidad_ > 100) {
        throw std::invalid_argument("La afinidad debe estar entre 0 y 100.");
    }

    // El ID menor se guarda primero para representar un par sin direccion.
    if (perfilAId_ > perfilBId_) {
        std::swap(perfilAId_, perfilBId_);
    }
}

int Conexion::obtenerPerfilAId() const noexcept {
    return perfilAId_;
}

int Conexion::obtenerPerfilBId() const noexcept {
    return perfilBId_;
}

int Conexion::obtenerPorcentajeAfinidad() const noexcept {
    return porcentajeAfinidad_;
}

bool Conexion::contienePerfil(int perfilId) const noexcept {
    return perfilAId_ == perfilId || perfilBId_ == perfilId;
}

bool Conexion::conectaPerfiles(
    int primerPerfilId,
    int segundoPerfilId
) const noexcept {
    if (primerPerfilId > segundoPerfilId) {
        int temporal = primerPerfilId;
        primerPerfilId = segundoPerfilId;
        segundoPerfilId = temporal;
    }

    return perfilAId_ == primerPerfilId
           && perfilBId_ == segundoPerfilId;
}

}
