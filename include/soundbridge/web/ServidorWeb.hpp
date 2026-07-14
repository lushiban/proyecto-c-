#ifndef SOUNDBRIDGE_WEB_SERVIDOR_WEB_HPP
#define SOUNDBRIDGE_WEB_SERVIDOR_WEB_HPP

#include <string>

#include "soundbridge/application/ControladorAplicacion.hpp"

namespace soundbridge {

class ServidorWeb {
private:
    ControladorAplicacion& controlador_;
    std::string carpetaFrontend_;
    std::string rutaDatos_;
    int puerto_;
    bool ejecutando_;

public:
    ServidorWeb(
        ControladorAplicacion& controlador,
        std::string carpetaFrontend,
        std::string rutaDatos,
        int puerto
    );

    int ejecutar(bool abrirNavegador = true);
};

}

#endif
