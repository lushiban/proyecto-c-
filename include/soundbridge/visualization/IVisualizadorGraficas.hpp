#ifndef SOUNDBRIDGE_VISUALIZATION_I_VISUALIZADOR_GRAFICAS_HPP
#define SOUNDBRIDGE_VISUALIZATION_I_VISUALIZADOR_GRAFICAS_HPP

#include "soundbridge/visualization/GraficaDTO.hpp"

namespace soundbridge {

class IVisualizadorGraficas {
public:
    virtual ~IVisualizadorGraficas() = default;

    virtual void mostrar(const GraficaDTO& grafica) = 0;
};

}

#endif
