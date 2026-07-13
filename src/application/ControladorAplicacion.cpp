#include "soundbridge/application/ControladorAplicacion.hpp"

#include <exception>
#include <string>
#include <vector>

#include "soundbridge/application/SoundBridge.hpp"
#include "soundbridge/application/ServicioEstadisticas.hpp"
#include "soundbridge/domain/Conexion.hpp"
#include "soundbridge/domain/Perfil.hpp"
#include "soundbridge/domain/PerfilArtista.hpp"
#include "soundbridge/domain/PerfilFanClub.hpp"
#include "soundbridge/domain/PerfilOyente.hpp"
#include "soundbridge/domain/PerfilProductor.hpp"
#include "soundbridge/domain/TipoPerfil.hpp"
#include "soundbridge/infrastructure/PersistenciaTexto.hpp"

namespace soundbridge {
namespace {

ResultadoOperacion crearResultado(bool correcto, const std::string& mensaje) {
    ResultadoOperacion resultado;
    resultado.correcto = correcto;
    resultado.mensaje = mensaje;
    return resultado;
}

std::string booleanoATexto(bool valor) {
    return valor ? "Si" : "No";
}

}

ControladorAplicacion::ControladorAplicacion() = default;

PerfilDTO ControladorAplicacion::convertirPerfil(int id) const {
    PerfilDTO datos{};
    const Perfil* perfil = red_.buscarPerfil(id);

    if (perfil == nullptr) {
        return datos;
    }

    datos.id = perfil->obtenerId();
    datos.tipo = tipoPerfilATexto(perfil->obtenerTipo());
    datos.nombre = perfil->obtenerNombre();
    datos.edad = perfil->obtenerEdad();
    datos.generoPrincipal = perfil->obtenerGeneroPrincipal();
    datos.generosSecundarios = perfil->obtenerGenerosSecundarios();

    if (perfil->obtenerTipo() == TipoPerfil::Oyente) {
        const PerfilOyente* oyente = dynamic_cast<const PerfilOyente*>(perfil);

        if (oyente != nullptr) {
            datos.camposEspecificos.push_back({
                "Horas de escucha semanales",
                std::to_string(oyente->obtenerHorasEscuchaSemanales())
            });
            datos.camposEspecificos.push_back({
                "Plataforma preferida",
                oyente->obtenerPlataformaPreferida()
            });
        }
    } else if (perfil->obtenerTipo() == TipoPerfil::Artista) {
        const PerfilArtista* artista = dynamic_cast<const PerfilArtista*>(perfil);

        if (artista != nullptr) {
            datos.camposEspecificos.push_back({
                "Nombre artistico",
                artista->obtenerNombreArtistico()
            });
            datos.camposEspecificos.push_back({
                "Instrumento principal",
                artista->obtenerInstrumentoPrincipal()
            });
            datos.camposEspecificos.push_back({
                "Busca colaboraciones",
                booleanoATexto(artista->buscaColaboraciones())
            });
        }
    } else if (perfil->obtenerTipo() == TipoPerfil::Productor) {
        const PerfilProductor* productor =
            dynamic_cast<const PerfilProductor*>(perfil);

        if (productor != nullptr) {
            datos.camposEspecificos.push_back({
                "Software principal",
                productor->obtenerSoftwarePrincipal()
            });
            datos.camposEspecificos.push_back({
                "Anios de experiencia",
                std::to_string(productor->obtenerAniosExperiencia())
            });
            datos.camposEspecificos.push_back({
                "Ofrece servicios",
                booleanoATexto(productor->ofreceServicios())
            });
        }
    } else if (perfil->obtenerTipo() == TipoPerfil::FanClub) {
        const PerfilFanClub* fanClub =
            dynamic_cast<const PerfilFanClub*>(perfil);

        if (fanClub != nullptr) {
            datos.camposEspecificos.push_back({
                "Artista representado",
                fanClub->obtenerArtistaRepresentado()
            });
            datos.camposEspecificos.push_back({
                "Numero de miembros",
                std::to_string(fanClub->obtenerNumeroMiembros())
            });
            datos.camposEspecificos.push_back({
                "Ciudad base",
                fanClub->obtenerCiudadBase()
            });
        }
    }

    return datos;
}

ResultadoCreacionPerfil ControladorAplicacion::crearPerfilOyente(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    int horasEscuchaSemanales,
    const std::string& plataformaPreferida
) {
    ResultadoCreacionPerfil respuesta{};

    try {
        respuesta.idPerfil = red_.crearPerfilOyente(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            horasEscuchaSemanales,
            plataformaPreferida
        );
        respuesta.resultado = crearResultado(
            true,
            "Perfil oyente creado correctamente."
        );
    } catch (const std::exception& error) {
        respuesta.resultado = crearResultado(false, error.what());
    }

    return respuesta;
}

ResultadoCreacionPerfil ControladorAplicacion::crearPerfilArtista(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    const std::string& nombreArtistico,
    const std::string& instrumentoPrincipal,
    bool buscaColaboraciones
) {
    ResultadoCreacionPerfil respuesta{};

    try {
        respuesta.idPerfil = red_.crearPerfilArtista(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            nombreArtistico,
            instrumentoPrincipal,
            buscaColaboraciones
        );
        respuesta.resultado = crearResultado(
            true,
            "Perfil artista creado correctamente."
        );
    } catch (const std::exception& error) {
        respuesta.resultado = crearResultado(false, error.what());
    }

    return respuesta;
}

ResultadoCreacionPerfil ControladorAplicacion::crearPerfilProductor(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    const std::string& softwarePrincipal,
    int aniosExperiencia,
    bool ofreceServicios
) {
    ResultadoCreacionPerfil respuesta{};

    try {
        respuesta.idPerfil = red_.crearPerfilProductor(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            softwarePrincipal,
            aniosExperiencia,
            ofreceServicios
        );
        respuesta.resultado = crearResultado(
            true,
            "Perfil productor creado correctamente."
        );
    } catch (const std::exception& error) {
        respuesta.resultado = crearResultado(false, error.what());
    }

    return respuesta;
}

ResultadoCreacionPerfil ControladorAplicacion::crearPerfilFanClub(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    const std::string& artistaRepresentado,
    int numeroMiembros,
    const std::string& ciudadBase
) {
    ResultadoCreacionPerfil respuesta{};

    try {
        respuesta.idPerfil = red_.crearPerfilFanClub(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            artistaRepresentado,
            numeroMiembros,
            ciudadBase
        );
        respuesta.resultado = crearResultado(
            true,
            "Perfil fan club creado correctamente."
        );
    } catch (const std::exception& error) {
        respuesta.resultado = crearResultado(false, error.what());
    }

    return respuesta;
}

std::vector<PerfilDTO> ControladorAplicacion::obtenerPerfiles() const {
    std::vector<PerfilDTO> perfiles;

    for (std::size_t i = 0; i < red_.obtenerCantidadPerfiles(); i++) {
        const Perfil* perfil = red_.obtenerPerfilEn(i);

        if (perfil != nullptr) {
            perfiles.push_back(convertirPerfil(perfil->obtenerId()));
        }
    }

    return perfiles;
}

ResultadoConsultaPerfil ControladorAplicacion::buscarPerfil(int id) const {
    ResultadoConsultaPerfil respuesta{};

    if (id <= 0) {
        respuesta.resultado = crearResultado(
            false,
            "El ID debe ser mayor que cero."
        );
        return respuesta;
    }

    const Perfil* perfil = red_.buscarPerfil(id);

    if (perfil == nullptr) {
        respuesta.resultado = crearResultado(
            false,
            "No se encontro el perfil solicitado."
        );
        return respuesta;
    }

    respuesta.perfil = convertirPerfil(id);
    respuesta.resultado = crearResultado(true, "Perfil encontrado.");
    return respuesta;
}

ResultadoCompatibles ControladorAplicacion::buscarPerfilesCompatibles(
    int id
) const {
    ResultadoCompatibles respuesta{};
    ResultadoConsultaPerfil consulta = buscarPerfil(id);

    if (!consulta.resultado.correcto) {
        respuesta.resultado = consulta.resultado;
        return respuesta;
    }

    std::vector<PerfilCompatible> compatibles =
        red_.buscarPerfilesCompatibles(id);

    for (const PerfilCompatible& compatible : compatibles) {
        if (compatible.perfil == nullptr) {
            continue;
        }

        PerfilCompatibleDTO datos;
        datos.id = compatible.perfil->obtenerId();
        datos.nombre = compatible.perfil->obtenerNombre();
        datos.tipo = tipoPerfilATexto(compatible.perfil->obtenerTipo());
        datos.afinidad = compatible.afinidad;
        respuesta.perfiles.push_back(datos);
    }

    if (respuesta.perfiles.empty()) {
        respuesta.resultado = crearResultado(
            true,
            "No se encontraron perfiles compatibles disponibles."
        );
    } else {
        respuesta.resultado = crearResultado(
            true,
            "Perfiles compatibles obtenidos correctamente."
        );
    }

    return respuesta;
}

ResultadoAfinidad ControladorAplicacion::calcularAfinidad(
    int perfilAId,
    int perfilBId
) const {
    ResultadoAfinidad respuesta{};

    if (perfilAId <= 0 || perfilBId <= 0) {
        respuesta.resultado = crearResultado(
            false,
            "Los IDs deben ser mayores que cero."
        );
        return respuesta;
    }

    if (perfilAId == perfilBId) {
        respuesta.resultado = crearResultado(
            false,
            "Debe seleccionar dos perfiles diferentes."
        );
        return respuesta;
    }

    respuesta.afinidad = red_.calcularAfinidad(perfilAId, perfilBId);

    if (respuesta.afinidad < 0) {
        respuesta.resultado = crearResultado(
            false,
            "No se encontraron los dos perfiles."
        );
        respuesta.afinidad = 0;
        return respuesta;
    }

    respuesta.resultado = crearResultado(
        true,
        "Afinidad calculada correctamente."
    );
    return respuesta;
}

ResultadoConexionAplicacion ControladorAplicacion::crearConexion(
    int perfilAId,
    int perfilBId
) {
    ResultadoConexionAplicacion respuesta{};

    if (perfilAId <= 0 || perfilBId <= 0) {
        respuesta.resultado = crearResultado(
            false,
            "Los IDs deben ser mayores que cero."
        );
        return respuesta;
    }

    ResultadoConexion resultado = red_.crearConexion(perfilAId, perfilBId);
    respuesta.resultado = crearResultado(
        resultado == ResultadoConexion::Creada,
        resultadoConexionATexto(resultado)
    );

    if (respuesta.resultado.correcto) {
        respuesta.afinidad = red_.calcularAfinidad(perfilAId, perfilBId);
    }

    return respuesta;
}

std::vector<ConexionDTO> ControladorAplicacion::obtenerConexiones() const {
    std::vector<ConexionDTO> conexiones;

    for (std::size_t i = 0; i < red_.obtenerCantidadConexiones(); i++) {
        const Conexion* conexion = red_.obtenerConexionEn(i);

        if (conexion == nullptr) {
            continue;
        }

        const Perfil* perfilA = red_.buscarPerfil(conexion->obtenerPerfilAId());
        const Perfil* perfilB = red_.buscarPerfil(conexion->obtenerPerfilBId());

        if (perfilA == nullptr || perfilB == nullptr) {
            continue;
        }

        ConexionDTO datos;
        datos.perfilAId = perfilA->obtenerId();
        datos.perfilANombre = perfilA->obtenerNombre();
        datos.perfilATipo = tipoPerfilATexto(perfilA->obtenerTipo());
        datos.perfilBId = perfilB->obtenerId();
        datos.perfilBNombre = perfilB->obtenerNombre();
        datos.perfilBTipo = tipoPerfilATexto(perfilB->obtenerTipo());
        datos.afinidad = conexion->obtenerPorcentajeAfinidad();
        conexiones.push_back(datos);
    }

    return conexiones;
}

ResultadoOperacion ControladorAplicacion::eliminarPerfil(int id) {
    if (id <= 0) {
        return crearResultado(false, "El ID debe ser mayor que cero.");
    }

    if (red_.buscarPerfil(id) == nullptr) {
        return crearResultado(false, "No se encontro el perfil solicitado.");
    }

    if (!red_.eliminarPerfil(id)) {
        return crearResultado(false, "No se pudo eliminar el perfil.");
    }

    return crearResultado(true, "Perfil eliminado correctamente.");
}

ResultadoOperacion ControladorAplicacion::eliminarConexion(
    int perfilAId,
    int perfilBId
) {
    if (perfilAId <= 0 || perfilBId <= 0) {
        return crearResultado(false, "Los IDs deben ser mayores que cero.");
    }

    if (perfilAId == perfilBId) {
        return crearResultado(
            false,
            "Debe seleccionar dos perfiles diferentes."
        );
    }

    if (!red_.existeConexion(perfilAId, perfilBId)) {
        return crearResultado(false, "No se encontro la conexion solicitada.");
    }

    if (!red_.eliminarConexion(perfilAId, perfilBId)) {
        return crearResultado(false, "No se pudo eliminar la conexion.");
    }

    return crearResultado(true, "Conexion eliminada correctamente.");
}

ResultadoOperacion ControladorAplicacion::cargarDatos() {
    std::string mensaje;
    bool correcto = PersistenciaTexto::cargar(red_, mensaje);
    return crearResultado(correcto, mensaje);
}

ResultadoOperacion ControladorAplicacion::cargarDatos(
    const std::string& rutaArchivo
) {
    std::string mensaje;
    bool correcto = PersistenciaTexto::cargar(red_, rutaArchivo, mensaje);
    return crearResultado(correcto, mensaje);
}

ResultadoOperacion ControladorAplicacion::guardarDatos() const {
    std::string mensaje;
    bool correcto = PersistenciaTexto::guardar(red_, mensaje);
    return crearResultado(correcto, mensaje);
}

ResultadoOperacion ControladorAplicacion::guardarDatos(
    const std::string& rutaArchivo
) const {
    std::string mensaje;
    bool correcto = PersistenciaTexto::guardar(red_, rutaArchivo, mensaje);
    return crearResultado(correcto, mensaje);
}

GraficaDTO ControladorAplicacion::obtenerGraficaPerfilesPorGenero() const {
    ServicioEstadisticas servicio;
    return servicio.crearGraficaPerfilesPorGenero(red_);
}

GraficaDTO ControladorAplicacion::obtenerGraficaPerfilesPorTipo() const {
    ServicioEstadisticas servicio;
    return servicio.crearGraficaPerfilesPorTipo(red_);
}

GraficaDTO ControladorAplicacion::obtenerGraficaConexionesPorAfinidad() const {
    ServicioEstadisticas servicio;
    return servicio.crearGraficaConexionesPorAfinidad(red_);
}

std::size_t ControladorAplicacion::obtenerCantidadPerfiles() const noexcept {
    return red_.obtenerCantidadPerfiles();
}

std::size_t ControladorAplicacion::obtenerCantidadConexiones() const noexcept {
    return red_.obtenerCantidadConexiones();
}

}
