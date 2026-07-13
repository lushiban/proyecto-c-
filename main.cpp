#include "AplicacionSoundBridge.h"
#include "ServidorWeb.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace {

std::string localizarCarpetaFrontend(const char* rutaEjecutable) {
    const std::filesystem::path carpetaActual = std::filesystem::current_path();
    const std::filesystem::path juntoAlProyecto = carpetaActual / "frontend";

    if (std::filesystem::exists(juntoAlProyecto / "index.html")) {
        return juntoAlProyecto.string();
    }

    std::error_code error;
    const std::filesystem::path ejecutableAbsoluto =
        std::filesystem::absolute(rutaEjecutable, error);

    if (!error) {
        const std::filesystem::path juntoAlEjecutable =
            ejecutableAbsoluto.parent_path() / "frontend";
        if (std::filesystem::exists(juntoAlEjecutable / "index.html")) {
            return juntoAlEjecutable.string();
        }
    }

    // El servidor mostrara una explicacion clara si la carpeta no existe.
    return juntoAlProyecto.string();
}

void mostrarAyuda(const char* nombrePrograma) {
    std::cout << "Uso:\n"
              << "  " << nombrePrograma << "                       Inicia la interfaz web\n"
              << "  " << nombrePrograma << " --consola             Inicia el menu de consola\n"
              << "  " << nombrePrograma << " --sin-navegador       No abre el navegador automaticamente\n"
              << "  " << nombrePrograma << " --puerto 8081         Cambia el puerto local\n"
              << "  " << nombrePrograma << " --ayuda               Muestra esta ayuda\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    bool modoConsola = false;
    bool abrirNavegador = true;
    int puerto = 8080;

    for (int indice = 1; indice < argc; indice++) {
        const std::string argumento = argv[indice];

        if (argumento == "--consola") {
            modoConsola = true;
        } else if (argumento == "--sin-navegador") {
            abrirNavegador = false;
        } else if (argumento == "--puerto") {
            if (indice + 1 >= argc) {
                std::cerr << "Falta indicar el numero de puerto.\n";
                return 1;
            }

            try {
                puerto = std::stoi(argv[++indice]);
            } catch (...) {
                std::cerr << "El puerto debe ser un numero.\n";
                return 1;
            }

            if (puerto < 1024 || puerto > 65535) {
                std::cerr << "El puerto debe estar entre 1024 y 65535.\n";
                return 1;
            }
        } else if (argumento == "--ayuda" || argumento == "-h") {
            mostrarAyuda(argv[0]);
            return 0;
        } else {
            std::cerr << "Argumento no reconocido: " << argumento << "\n\n";
            mostrarAyuda(argv[0]);
            return 1;
        }
    }

    if (modoConsola) {
        // Se conserva la interfaz original para demostrar que la logica no depende del navegador.
        AplicacionSoundBridge aplicacion("datos.txt");
        return aplicacion.ejecutar();
    }

    const std::string carpetaFrontend = localizarCarpetaFrontend(argv[0]);
    ServidorWeb servidor("datos.txt", carpetaFrontend, puerto);
    return servidor.ejecutar(abrirNavegador);
}
