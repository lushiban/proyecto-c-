#ifndef SOUNDBRIDGE_DOMAIN_TIPO_PERFIL_HPP
#define SOUNDBRIDGE_DOMAIN_TIPO_PERFIL_HPP

namespace soundbridge {

enum class TipoPerfil {
    Oyente,
    Artista,
    Productor,
    FanClub
};

const char* tipoPerfilATexto(TipoPerfil tipo) noexcept;

}

#endif
