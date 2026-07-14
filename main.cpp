#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "soundbridge/Config.hpp"
#include "soundbridge/application/ControladorAplicacion.hpp"
#include "soundbridge/ui/MenuConsola.hpp"
#include "soundbridge/visualization/VisualizadorNoDisponible.hpp"
#include "soundbridge/web/ServidorWeb.hpp"

namespace {

std::filesystem::path carpetaDelEjecutable(const char* rutaEjecutable) {
    std::error_code error;
    std::filesystem::path ruta = std::filesystem::absolute(rutaEjecutable, error);
    if (error) {
        return std::filesystem::current_path();
    }
    return ruta.parent_path();
}

std::string localizarCarpetaFrontend(const char* rutaEjecutable) {
    const std::filesystem::path actual = std::filesystem::current_path();
    const std::filesystem::path juntoAlProyecto = actual / "frontend";
    if (std::filesystem::exists(juntoAlProyecto / "index.html")) {
        return juntoAlProyecto.string();
    }

    const std::filesystem::path juntoAlEjecutable =
        carpetaDelEjecutable(rutaEjecutable) / "frontend";
    if (std::filesystem::exists(juntoAlEjecutable / "index.html")) {
        return juntoAlEjecutable.string();
    }

    return juntoAlProyecto.string();
}

std::string localizarRutaDatos(const char* rutaEjecutable) {
    const std::filesystem::path actual = std::filesystem::current_path() / "data" / "datos.txt";
    if (std::filesystem::exists(actual)) {
        return actual.string();
    }

    const std::filesystem::path juntoAlEjecutable =
        carpetaDelEjecutable(rutaEjecutable) / "data" / "datos.txt";
    return juntoAlEjecutable.string();
}

void mostrarAyuda(const char* nombrePrograma) {
    std::cout
        << "Uso:\n"
        << "  " << nombrePrograma << "                    Inicia la interfaz web\n"
        << "  " << nombrePrograma << " --consola          Inicia el menu de consola\n"
        << "  " << nombrePrograma << " --sin-navegador    No abre el navegador automaticamente\n"
        << "  " << nombrePrograma << " --puerto 8081      Cambia el puerto local\n"
        << "  " << nombrePrograma << " --ayuda            Muestra esta ayuda\n";
}

}

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

    try {
        soundbridge::ControladorAplicacion controlador;
        const std::string rutaDatos = localizarRutaDatos(argv[0]);
        const soundbridge::ResultadoOperacion carga = controlador.cargarDatos(rutaDatos);

        std::cout << "========================================\n";
        std::cout << soundbridge::NOMBRE_PROYECTO << "\n";
        std::cout << "Version: " << soundbridge::VERSION_PROYECTO << "\n";
        std::cout << "========================================\n";
        std::cout << carga.mensaje << "\n";

        if (modoConsola) {
            soundbridge::VisualizadorNoDisponible visualizador;
            soundbridge::MenuConsola menu(controlador, visualizador);
            menu.ejecutar();
            controlador.guardarDatos(rutaDatos);
            return 0;
        }

        soundbridge::ServidorWeb servidor(
            controlador,
            localizarCarpetaFrontend(argv[0]),
            rutaDatos,
            puerto
        );
        return servidor.ejecutar(abrirNavegador);
    } catch (const std::exception& error) {
        std::cerr << "SoundBridge finalizo por un error: " << error.what() << "\n";
        return 1;
    }
}
    return servidor.ejecutar(abrirNavegador);
}
