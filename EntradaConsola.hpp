#ifndef SOUNDBRIDGE_APPLICATION_SOUNDBRIDGE_HPP
#define SOUNDBRIDGE_APPLICATION_SOUNDBRIDGE_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace soundbridge {

class Conexion;
class Perfil;
class PersistenciaTexto;

struct PerfilCompatible {
    const Perfil* perfil;
    int afinidad;
};

enum class ResultadoConexion {
    Creada,
    PerfilNoEncontrado,
    MismoPerfil,
    YaExiste,
    AfinidadInsuficiente
};

const char* resultadoConexionATexto(ResultadoConexion resultado) noexcept;

class SoundBridge {
    friend class PersistenciaTexto;

private:
    Perfil** perfiles_;
    std::size_t cantidadPerfiles_;
    std::size_t capacidadPerfiles_;

    Conexion** conexiones_;
    std::size_t cantidadConexiones_;
    std::size_t capacidadConexiones_;

    int siguientePerfilId_;

    void redimensionarPerfiles();
    void redimensionarConexiones();
    void agregarPerfil(Perfil* perfil);
    void agregarConexion(Conexion* conexion);
    void eliminarConexionesDePerfil(int perfilId) noexcept;
    int calcularAfinidadEntrePerfiles(
        const Perfil& perfilA,
        const Perfil& perfilB
    ) const noexcept;

public:
    SoundBridge();
    ~SoundBridge();

    SoundBridge(const SoundBridge&) = delete;
    SoundBridge& operator=(const SoundBridge&) = delete;

    void intercambiar(SoundBridge& otra) noexcept;

    int crearPerfilOyente(
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        int horasEscuchaSemanales,
        std::string plataformaPreferida
    );

    int crearPerfilArtista(
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        std::string nombreArtistico,
        std::string instrumentoPrincipal,
        bool buscaColaboraciones
    );

    int crearPerfilProductor(
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        std::string softwarePrincipal,
        int aniosExperiencia,
        bool ofreceServicios
    );

    int crearPerfilFanClub(
        std::string nombre,
        int edad,
        std::string generoPrincipal,
        std::vector<std::string> generosSecundarios,
        std::string artistaRepresentado,
        int numeroMiembros,
        std::string ciudadBase
    );

    const Perfil* buscarPerfil(int id) const noexcept;
    bool eliminarPerfil(int id) noexcept;

    int calcularAfinidad(int perfilAId, int perfilBId) const noexcept;
    std::vector<PerfilCompatible> buscarPerfilesCompatibles(
        int perfilId
    ) const;
    ResultadoConexion crearConexion(int perfilAId, int perfilBId);
    bool existeConexion(int perfilAId, int perfilBId) const noexcept;
    bool eliminarConexion(int perfilAId, int perfilBId) noexcept;

    std::size_t obtenerCantidadPerfiles() const noexcept;
    std::size_t obtenerCapacidadPerfiles() const noexcept;
    int obtenerSiguientePerfilId() const noexcept;
    const Perfil* obtenerPerfilEn(std::size_t posicion) const noexcept;

    std::size_t obtenerCantidadConexiones() const noexcept;
    std::size_t obtenerCapacidadConexiones() const noexcept;
    const Conexion* obtenerConexionEn(std::size_t posicion) const noexcept;
};

}

#endif
