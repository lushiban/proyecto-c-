#include "soundbridge/visualization/VisualizadorNoDisponible.hpp"

#include <iostream>

namespace soundbridge {

void VisualizadorNoDisponible::mostrar(const GraficaDTO& grafica) {
    (void)grafica;
    std::cout << "El soporte grafico no esta habilitado." << std::endl;
}

}
