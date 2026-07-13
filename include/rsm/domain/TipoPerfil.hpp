#ifndef RSM_DOMAIN_TIPO_PERFIL_HPP
#define RSM_DOMAIN_TIPO_PERFIL_HPP

namespace rsm {

enum class TipoPerfil {
    Oyente,
    Artista,
    Productor,
    FanClub
};

const char* tipoPerfilATexto(TipoPerfil tipo) noexcept;

}

#endif
