#ifndef RSM_VISUALIZATION_GRAFICA_DTO_HPP
#define RSM_VISUALIZATION_GRAFICA_DTO_HPP

#include <string>
#include <vector>

namespace rsm {

enum class TipoGrafica {
    Barras,
    Circular
};

struct PuntoGrafica {
    std::string etiqueta;
    double valor;
};

struct GraficaDTO {
    std::string titulo;
    std::string etiquetaEjeX;
    std::string etiquetaEjeY;
    TipoGrafica tipo;
    std::vector<PuntoGrafica> puntos;
};

}

#endif
