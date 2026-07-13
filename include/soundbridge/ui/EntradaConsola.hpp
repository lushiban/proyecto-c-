#ifndef SOUNDBRIDGE_UI_ENTRADA_CONSOLA_HPP
#define SOUNDBRIDGE_UI_ENTRADA_CONSOLA_HPP

#include <string>
#include <vector>

namespace soundbridge {

class EntradaConsola {
public:
    static std::string leerTexto(const std::string& mensaje);
    static std::string leerTextoNoVacio(const std::string& mensaje);

    static int leerEnteroEnRango(
        const std::string& mensaje,
        int minimo,
        int maximo
    );

    static int leerEnteroMinimo(
        const std::string& mensaje,
        int minimo
    );

    static bool leerConfirmacion(const std::string& mensaje);

    static std::vector<std::string> leerGenerosSecundarios(
        const std::string& generoPrincipal
    );
};

}

#endif
