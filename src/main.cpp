#include <exception>
#include <iostream>

#include "soundbridge/Config.hpp"
#include "soundbridge/application/ControladorAplicacion.hpp"
#include "soundbridge/ui/MenuConsola.hpp"
#include "soundbridge/visualization/VisualizadorNoDisponible.hpp"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << soundbridge::NOMBRE_PROYECTO << std::endl;
    std::cout << "Version: " << soundbridge::VERSION_PROYECTO << std::endl;
    std::cout << "========================================" << std::endl;

    try {
        soundbridge::ControladorAplicacion controlador;
        soundbridge::ResultadoOperacion carga = controlador.cargarDatos();

        if (carga.correcto) {
            std::cout << carga.mensaje << std::endl;
        } else {
            std::cout << "No se pudieron cargar los datos: "
                      << carga.mensaje << std::endl;
            std::cout << "El programa continuara con la red actual."
                      << std::endl;
        }

        soundbridge::VisualizadorNoDisponible visualizador;
        soundbridge::MenuConsola menu(controlador, visualizador);
        menu.ejecutar();
    } catch (const std::exception& error) {
        std::cerr << "El programa finalizo por un error: "
                  << error.what() << std::endl;
        return 1;
    }

    return 0;
}
