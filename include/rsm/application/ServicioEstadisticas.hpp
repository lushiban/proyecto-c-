#ifndef RSM_APPLICATION_SERVICIO_ESTADISTICAS_HPP
#define RSM_APPLICATION_SERVICIO_ESTADISTICAS_HPP

#include "rsm/visualization/GraficaDTO.hpp"

namespace rsm {

class RedSocialMusical;

class ServicioEstadisticas {
public:
    GraficaDTO crearGraficaPerfilesPorGenero(
        const RedSocialMusical& red
    ) const;

    GraficaDTO crearGraficaPerfilesPorTipo(
        const RedSocialMusical& red
    ) const;

    GraficaDTO crearGraficaConexionesPorAfinidad(
        const RedSocialMusical& red
    ) const;
};

}

#endif
