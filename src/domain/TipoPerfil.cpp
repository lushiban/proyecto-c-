#include "rsm/domain/TipoPerfil.hpp"

namespace rsm {

const char* tipoPerfilATexto(TipoPerfil tipo) noexcept {
    switch (tipo) {
        case TipoPerfil::Oyente:
            return "Oyente";
        case TipoPerfil::Artista:
            return "Artista";
        case TipoPerfil::Productor:
            return "Productor";
        case TipoPerfil::FanClub:
            return "Fan Club";
    }

    return "Desconocido";
}

}
