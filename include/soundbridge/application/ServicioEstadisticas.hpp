#ifndef SOUNDBRIDGE_APPLICATION_SERVICIO_ESTADISTICAS_HPP
#define SOUNDBRIDGE_APPLICATION_SERVICIO_ESTADISTICAS_HPP

#include "soundbridge/visualization/GraficaDTO.hpp"

namespace soundbridge {

class SoundBridge;

class ServicioEstadisticas {
public:
    GraficaDTO crearGraficaPerfilesPorGenero(
        const SoundBridge& red
    ) const;

    GraficaDTO crearGraficaPerfilesPorTipo(
        const SoundBridge& red
    ) const;

    GraficaDTO crearGraficaConexionesPorAfinidad(
        const SoundBridge& red
    ) const;
};

}

#endif
