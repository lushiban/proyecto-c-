#ifndef RSM_VISUALIZATION_I_VISUALIZADOR_GRAFICAS_HPP
#define RSM_VISUALIZATION_I_VISUALIZADOR_GRAFICAS_HPP

#include "rsm/visualization/GraficaDTO.hpp"

namespace rsm {

class IVisualizadorGraficas {
public:
    virtual ~IVisualizadorGraficas() = default;

    virtual void mostrar(const GraficaDTO& grafica) = 0;
};

}

#endif
