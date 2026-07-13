#ifndef RSM_APPLICATION_CONTROLADOR_APLICACION_HPP
#define RSM_APPLICATION_CONTROLADOR_APLICACION_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "rsm/application/RedSocialMusical.hpp"
#include "rsm/visualization/GraficaDTO.hpp"

namespace rsm {

struct ResultadoOperacion {
    bool correcto;
    std::string mensaje;
};

struct ResultadoCreacionPerfil {
    ResultadoOperacion resultado;
    int idPerfil;
};

struct CampoDetallePerfilDTO {
    std::string nombre;
    std::string valor;
};

struct PerfilDTO {
    int id;
    std::string tipo;
    std::string nombre;
    int edad;
    std::string generoPrincipal;
    std::vector<std::string> generosSecundarios;
    std::vector<CampoDetallePerfilDTO> camposEspecificos;
};

struct ResultadoConsultaPerfil {
    ResultadoOperacion resultado;
    PerfilDTO perfil;
};

struct PerfilCompatibleDTO {
    int id;
    std::string nombre;
    std::string tipo;
    int afinidad;
};

struct ResultadoCompatibles {
    ResultadoOperacion resultado;
    std::vector<PerfilCompatibleDTO> perfiles;
};

struct ConexionDTO {
    int perfilAId;
    std::string perfilANombre;
    std::string perfilATipo;
    int perfilBId;
    std::string perfilBNombre;
    std::string perfilBTipo;
    int afinidad;
};

struct ResultadoConexionAplicacion {
    ResultadoOperacion resultado;
    int afinidad;
};

struct ResultadoAfinidad {
    ResultadoOperacion resultado;
    int afinidad;
};

class ControladorAplicacion {
private:
    RedSocialMusical red_;

    PerfilDTO convertirPerfil(int id) const;

public:
    ControladorAplicacion();

    ControladorAplicacion(const ControladorAplicacion&) = delete;
    ControladorAplicacion& operator=(const ControladorAplicacion&) = delete;

    ResultadoCreacionPerfil crearPerfilOyente(
        const std::string& nombre,
        int edad,
        const std::string& generoPrincipal,
        const std::vector<std::string>& generosSecundarios,
        int horasEscuchaSemanales,
        const std::string& plataformaPreferida
    );

    ResultadoCreacionPerfil crearPerfilArtista(
        const std::string& nombre,
        int edad,
        const std::string& generoPrincipal,
        const std::vector<std::string>& generosSecundarios,
        const std::string& nombreArtistico,
        const std::string& instrumentoPrincipal,
        bool buscaColaboraciones
    );

    ResultadoCreacionPerfil crearPerfilProductor(
        const std::string& nombre,
        int edad,
        const std::string& generoPrincipal,
        const std::vector<std::string>& generosSecundarios,
        const std::string& softwarePrincipal,
        int aniosExperiencia,
        bool ofreceServicios
    );

    ResultadoCreacionPerfil crearPerfilFanClub(
        const std::string& nombre,
        int edad,
        const std::string& generoPrincipal,
        const std::vector<std::string>& generosSecundarios,
        const std::string& artistaRepresentado,
        int numeroMiembros,
        const std::string& ciudadBase
    );

    std::vector<PerfilDTO> obtenerPerfiles() const;
    ResultadoConsultaPerfil buscarPerfil(int id) const;
    ResultadoCompatibles buscarPerfilesCompatibles(int id) const;
    ResultadoAfinidad calcularAfinidad(int perfilAId, int perfilBId) const;

    ResultadoConexionAplicacion crearConexion(
        int perfilAId,
        int perfilBId
    );

    std::vector<ConexionDTO> obtenerConexiones() const;
    ResultadoOperacion eliminarPerfil(int id);
    ResultadoOperacion eliminarConexion(int perfilAId, int perfilBId);

    ResultadoOperacion cargarDatos();
    ResultadoOperacion cargarDatos(const std::string& rutaArchivo);
    ResultadoOperacion guardarDatos() const;
    ResultadoOperacion guardarDatos(const std::string& rutaArchivo) const;

    GraficaDTO obtenerGraficaPerfilesPorGenero() const;
    GraficaDTO obtenerGraficaPerfilesPorTipo() const;
    GraficaDTO obtenerGraficaConexionesPorAfinidad() const;

    std::size_t obtenerCantidadPerfiles() const noexcept;
    std::size_t obtenerCantidadConexiones() const noexcept;
};

}

#endif
