#ifndef RSM_VISUALIZATION_VISUALIZADOR_NO_DISPONIBLE_HPP
#define RSM_VISUALIZATION_VISUALIZADOR_NO_DISPONIBLE_HPP

#include "rsm/visualization/IVisualizadorGraficas.hpp"

namespace rsm {

class VisualizadorNoDisponible final : public IVisualizadorGraficas {
public:
    void mostrar(const GraficaDTO& grafica) override;
};

}

#endif
