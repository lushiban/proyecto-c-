#ifndef SOUNDBRIDGE_VISUALIZATION_VISUALIZADOR_NO_DISPONIBLE_HPP
#define SOUNDBRIDGE_VISUALIZATION_VISUALIZADOR_NO_DISPONIBLE_HPP

#include "soundbridge/visualization/IVisualizadorGraficas.hpp"

namespace soundbridge {

class VisualizadorNoDisponible final : public IVisualizadorGraficas {
public:
    void mostrar(const GraficaDTO& grafica) override;
};

}

#endif
