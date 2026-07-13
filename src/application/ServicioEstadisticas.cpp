#include "soundbridge/application/ServicioEstadisticas.hpp"

#include <utility>

#include "soundbridge/application/SoundBridge.hpp"
#include "soundbridge/domain/Conexion.hpp"
#include "soundbridge/domain/Perfil.hpp"
#include "soundbridge/domain/TipoPerfil.hpp"
#include "soundbridge/utils/TextoUtils.hpp"

namespace soundbridge {
namespace {

void ordenarPuntosPorCantidad(std::vector<PuntoGrafica>& puntos) {
    for (std::size_t i = 0; i < puntos.size(); i++) {
        for (std::size_t j = i + 1; j < puntos.size(); j++) {
            bool cantidadMayor = puntos[j].valor > puntos[i].valor;
            bool mismaCantidad = puntos[j].valor == puntos[i].valor;
            bool nombreMenor = normalizarTexto(puntos[j].etiqueta)
                               < normalizarTexto(puntos[i].etiqueta);

            if (cantidadMayor || (mismaCantidad && nombreMenor)) {
                std::swap(puntos[i], puntos[j]);
            }
        }
    }
}

}

GraficaDTO ServicioEstadisticas::crearGraficaPerfilesPorGenero(
    const SoundBridge& red
) const {
    GraficaDTO grafica;
    grafica.titulo = "Perfiles por genero musical";
    grafica.etiquetaEjeX = "Genero musical";
    grafica.etiquetaEjeY = "Cantidad de perfiles";
    grafica.tipo = TipoGrafica::Barras;

    for (std::size_t i = 0; i < red.obtenerCantidadPerfiles(); i++) {
        const Perfil* perfil = red.obtenerPerfilEn(i);

        if (perfil == nullptr) {
            continue;
        }

        std::string genero = recortarTexto(perfil->obtenerGeneroPrincipal());
        bool generoEncontrado = false;

        for (PuntoGrafica& punto : grafica.puntos) {
            if (normalizarTexto(punto.etiqueta) == normalizarTexto(genero)) {
                punto.valor++;
                generoEncontrado = true;
                break;
            }
        }

        if (!generoEncontrado) {
            PuntoGrafica nuevoPunto;
            nuevoPunto.etiqueta = genero;
            nuevoPunto.valor = 1.0;
            grafica.puntos.push_back(nuevoPunto);
        }
    }

    ordenarPuntosPorCantidad(grafica.puntos);
    return grafica;
}

GraficaDTO ServicioEstadisticas::crearGraficaPerfilesPorTipo(
    const SoundBridge& red
) const {
    GraficaDTO grafica;
    grafica.titulo = "Perfiles por tipo";
    grafica.etiquetaEjeX = "Tipo de perfil";
    grafica.etiquetaEjeY = "Cantidad de perfiles";
    grafica.tipo = TipoGrafica::Circular;
    grafica.puntos = {
        {"Oyente", 0.0},
        {"Artista", 0.0},
        {"Productor", 0.0},
        {"Fan Club", 0.0}
    };

    for (std::size_t i = 0; i < red.obtenerCantidadPerfiles(); i++) {
        const Perfil* perfil = red.obtenerPerfilEn(i);

        if (perfil == nullptr) {
            continue;
        }

        switch (perfil->obtenerTipo()) {
            case TipoPerfil::Oyente:
                grafica.puntos[0].valor++;
                break;
            case TipoPerfil::Artista:
                grafica.puntos[1].valor++;
                break;
            case TipoPerfil::Productor:
                grafica.puntos[2].valor++;
                break;
            case TipoPerfil::FanClub:
                grafica.puntos[3].valor++;
                break;
        }
    }

    return grafica;
}

GraficaDTO ServicioEstadisticas::crearGraficaConexionesPorAfinidad(
    const SoundBridge& red
) const {
    GraficaDTO grafica;
    grafica.titulo = "Conexiones por rango de afinidad";
    grafica.etiquetaEjeX = "Rango de afinidad";
    grafica.etiquetaEjeY = "Cantidad de conexiones";
    grafica.tipo = TipoGrafica::Barras;
    grafica.puntos = {
        {"40-59", 0.0},
        {"60-79", 0.0},
        {"80-100", 0.0}
    };

    for (std::size_t i = 0; i < red.obtenerCantidadConexiones(); i++) {
        const Conexion* conexion = red.obtenerConexionEn(i);

        if (conexion == nullptr) {
            continue;
        }

        int afinidad = conexion->obtenerPorcentajeAfinidad();

        if (afinidad >= 40 && afinidad <= 59) {
            grafica.puntos[0].valor++;
        } else if (afinidad >= 60 && afinidad <= 79) {
            grafica.puntos[1].valor++;
        } else if (afinidad >= 80 && afinidad <= 100) {
            grafica.puntos[2].valor++;
        }
    }

    return grafica;
}

}
