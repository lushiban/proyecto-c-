#ifndef RSM_INFRASTRUCTURE_PERSISTENCIA_TEXTO_HPP
#define RSM_INFRASTRUCTURE_PERSISTENCIA_TEXTO_HPP

#include <string>

namespace rsm {

class RedSocialMusical;

class PersistenciaTexto {
public:
    static bool guardar(
        const RedSocialMusical& red,
        std::string& mensaje
    );

    static bool guardar(
        const RedSocialMusical& red,
        const std::string& rutaArchivo,
        std::string& mensaje
    );

    static bool cargar(
        RedSocialMusical& red,
        std::string& mensaje
    );

    static bool cargar(
        RedSocialMusical& red,
        const std::string& rutaArchivo,
        std::string& mensaje
    );
};

}

#endif
