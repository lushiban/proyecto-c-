#include "rsm/visualization/VisualizadorNoDisponible.hpp"

#include <iostream>

namespace rsm {

void VisualizadorNoDisponible::mostrar(const GraficaDTO& grafica) {
    (void)grafica;
    std::cout << "El soporte grafico no esta habilitado." << std::endl;
}

}
