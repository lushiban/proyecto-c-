#include <exception>
#include <iostream>

#include "rsm/Config.hpp"
#include "rsm/application/ControladorAplicacion.hpp"
#include "rsm/ui/MenuConsola.hpp"
#include "rsm/visualization/VisualizadorNoDisponible.hpp"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << rsm::NOMBRE_PROYECTO << std::endl;
    std::cout << "Version: " << rsm::VERSION_PROYECTO << std::endl;
    std::cout << "========================================" << std::endl;

    try {
        rsm::ControladorAplicacion controlador;
        rsm::ResultadoOperacion carga = controlador.cargarDatos();

        if (carga.correcto) {
            std::cout << carga.mensaje << std::endl;
        } else {
            std::cout << "No se pudieron cargar los datos: "
                      << carga.mensaje << std::endl;
            std::cout << "El programa continuara con la red actual."
                      << std::endl;
        }

        rsm::VisualizadorNoDisponible visualizador;
        rsm::MenuConsola menu(controlador, visualizador);
        menu.ejecutar();
    } catch (const std::exception& error) {
        std::cerr << "El programa finalizo por un error: "
                  << error.what() << std::endl;
        return 1;
    }

    return 0;
}
