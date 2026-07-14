#ifndef SOUNDBRIDGE_INFRASTRUCTURE_PERSISTENCIA_TEXTO_HPP
#define SOUNDBRIDGE_INFRASTRUCTURE_PERSISTENCIA_TEXTO_HPP

#include <string>

namespace soundbridge {

class SoundBridge;

class PersistenciaTexto {
public:
    static bool guardar(
        const SoundBridge& red,
        std::string& mensaje
    );

    static bool guardar(
        const SoundBridge& red,
        const std::string& rutaArchivo,
        std::string& mensaje
    );

    static bool cargar(
        SoundBridge& red,
        std::string& mensaje
    );

    static bool cargar(
        SoundBridge& red,
        const std::string& rutaArchivo,
        std::string& mensaje
    );
};

}

#endif
