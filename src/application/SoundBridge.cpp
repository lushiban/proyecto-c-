#include "soundbridge/application/SoundBridge.hpp"

#include <utility>

#include "soundbridge/domain/Conexion.hpp"
#include "soundbridge/domain/Perfil.hpp"
#include "soundbridge/domain/PerfilArtista.hpp"
#include "soundbridge/domain/PerfilFanClub.hpp"
#include "soundbridge/domain/PerfilOyente.hpp"
#include "soundbridge/domain/PerfilProductor.hpp"
#include "soundbridge/utils/TextoUtils.hpp"

namespace soundbridge {

const char* resultadoConexionATexto(ResultadoConexion resultado) noexcept {
    switch (resultado) {
        case ResultadoConexion::Creada:
            return "Conexion creada correctamente.";
        case ResultadoConexion::PerfilNoEncontrado:
            return "No se encontraron los dos perfiles.";
        case ResultadoConexion::MismoPerfil:
            return "Un perfil no puede conectarse consigo mismo.";
        case ResultadoConexion::YaExiste:
            return "La conexion ya existe.";
        case ResultadoConexion::AfinidadInsuficiente:
            return "La afinidad debe ser igual o superior a 40.";
    }

    return "Resultado de conexion desconocido.";
}

SoundBridge::SoundBridge()
    : perfiles_(new Perfil*[4]),
      cantidadPerfiles_(0),
      capacidadPerfiles_(4),
      conexiones_(nullptr),
      cantidadConexiones_(0),
      capacidadConexiones_(4),
      siguientePerfilId_(1) {
    try {
        conexiones_ = new Conexion*[capacidadConexiones_];
    } catch (...) {
        delete[] perfiles_;
        throw;
    }
}

void SoundBridge::intercambiar(SoundBridge& otra) noexcept {
    std::swap(perfiles_, otra.perfiles_);
    std::swap(cantidadPerfiles_, otra.cantidadPerfiles_);
    std::swap(capacidadPerfiles_, otra.capacidadPerfiles_);
    std::swap(conexiones_, otra.conexiones_);
    std::swap(cantidadConexiones_, otra.cantidadConexiones_);
    std::swap(capacidadConexiones_, otra.capacidadConexiones_);
    std::swap(siguientePerfilId_, otra.siguientePerfilId_);
}

SoundBridge::~SoundBridge() {
    for (std::size_t i = 0; i < cantidadPerfiles_; i++) {
        delete perfiles_[i];
    }

    for (std::size_t i = 0; i < cantidadConexiones_; i++) {
        delete conexiones_[i];
    }

    delete[] perfiles_;
    delete[] conexiones_;
}

void SoundBridge::redimensionarPerfiles() {
    std::size_t nuevaCapacidad = capacidadPerfiles_ * 2;
    Perfil** nuevosPerfiles = new Perfil*[nuevaCapacidad];

    for (std::size_t i = 0; i < cantidadPerfiles_; i++) {
        nuevosPerfiles[i] = perfiles_[i];
    }

    delete[] perfiles_;
    perfiles_ = nuevosPerfiles;
    capacidadPerfiles_ = nuevaCapacidad;
}

void SoundBridge::redimensionarConexiones() {
    std::size_t nuevaCapacidad = capacidadConexiones_ * 2;
    Conexion** nuevasConexiones = new Conexion*[nuevaCapacidad];

    for (std::size_t i = 0; i < cantidadConexiones_; i++) {
        nuevasConexiones[i] = conexiones_[i];
    }

    delete[] conexiones_;
    conexiones_ = nuevasConexiones;
    capacidadConexiones_ = nuevaCapacidad;
}

void SoundBridge::agregarPerfil(Perfil* perfil) {
    if (cantidadPerfiles_ == capacidadPerfiles_) {
        try {
            redimensionarPerfiles();
        } catch (...) {
            delete perfil;
            throw;
        }
    }

    perfiles_[cantidadPerfiles_] = perfil;
    cantidadPerfiles_++;
}

void SoundBridge::agregarConexion(Conexion* conexion) {
    if (cantidadConexiones_ == capacidadConexiones_) {
        try {
            redimensionarConexiones();
        } catch (...) {
            delete conexion;
            throw;
        }
    }

    conexiones_[cantidadConexiones_] = conexion;
    cantidadConexiones_++;
}

void SoundBridge::eliminarConexionesDePerfil(int perfilId) noexcept {
    std::size_t posicion = 0;

    while (posicion < cantidadConexiones_) {
        if (conexiones_[posicion]->contienePerfil(perfilId)) {
            delete conexiones_[posicion];

            for (std::size_t i = posicion; i + 1 < cantidadConexiones_; i++) {
                conexiones_[i] = conexiones_[i + 1];
            }

            cantidadConexiones_--;
            conexiones_[cantidadConexiones_] = nullptr;
        } else {
            posicion++;
        }
    }
}

int SoundBridge::calcularAfinidadEntrePerfiles(
    const Perfil& perfilA,
    const Perfil& perfilB
) const noexcept {
    int afinidad = 0;

    if (normalizarTexto(perfilA.obtenerGeneroPrincipal())
        == normalizarTexto(perfilB.obtenerGeneroPrincipal())) {
        afinidad += 60;
    }

    if (contieneTexto(
            perfilB.obtenerGenerosSecundarios(),
            perfilA.obtenerGeneroPrincipal()
        )) {
        afinidad += 15;
    }

    if (contieneTexto(
            perfilA.obtenerGenerosSecundarios(),
            perfilB.obtenerGeneroPrincipal()
        )) {
        afinidad += 15;
    }

    int puntosSecundarios = 0;

    for (const std::string& generoA : perfilA.obtenerGenerosSecundarios()) {
        if (contieneTexto(perfilB.obtenerGenerosSecundarios(), generoA)) {
            puntosSecundarios += 5;
        }
    }

    if (puntosSecundarios > 10) {
        puntosSecundarios = 10;
    }

    afinidad += puntosSecundarios;

    if (afinidad > 100) {
        afinidad = 100;
    }

    return afinidad;
}

int SoundBridge::crearPerfilOyente(
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    int horasEscuchaSemanales,
    std::string plataformaPreferida
) {
    int idAsignado = siguientePerfilId_;

    Perfil* perfil = new PerfilOyente(
        idAsignado,
        std::move(nombre),
        edad,
        std::move(generoPrincipal),
        std::move(generosSecundarios),
        horasEscuchaSemanales,
        std::move(plataformaPreferida)
    );

    agregarPerfil(perfil);
    siguientePerfilId_++;
    return idAsignado;
}

int SoundBridge::crearPerfilArtista(
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    std::string nombreArtistico,
    std::string instrumentoPrincipal,
    bool buscaColaboraciones
) {
    int idAsignado = siguientePerfilId_;

    Perfil* perfil = new PerfilArtista(
        idAsignado,
        std::move(nombre),
        edad,
        std::move(generoPrincipal),
        std::move(generosSecundarios),
        std::move(nombreArtistico),
        std::move(instrumentoPrincipal),
        buscaColaboraciones
    );

    agregarPerfil(perfil);
    siguientePerfilId_++;
    return idAsignado;
}

int SoundBridge::crearPerfilProductor(
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    std::string softwarePrincipal,
    int aniosExperiencia,
    bool ofreceServicios
) {
    int idAsignado = siguientePerfilId_;

    Perfil* perfil = new PerfilProductor(
        idAsignado,
        std::move(nombre),
        edad,
        std::move(generoPrincipal),
        std::move(generosSecundarios),
        std::move(softwarePrincipal),
        aniosExperiencia,
        ofreceServicios
    );

    agregarPerfil(perfil);
    siguientePerfilId_++;
    return idAsignado;
}

int SoundBridge::crearPerfilFanClub(
    std::string nombre,
    int edad,
    std::string generoPrincipal,
    std::vector<std::string> generosSecundarios,
    std::string artistaRepresentado,
    int numeroMiembros,
    std::string ciudadBase
) {
    int idAsignado = siguientePerfilId_;

    Perfil* perfil = new PerfilFanClub(
        idAsignado,
        std::move(nombre),
        edad,
        std::move(generoPrincipal),
        std::move(generosSecundarios),
        std::move(artistaRepresentado),
        numeroMiembros,
        std::move(ciudadBase)
    );

    agregarPerfil(perfil);
    siguientePerfilId_++;
    return idAsignado;
}

const Perfil* SoundBridge::buscarPerfil(int id) const noexcept {
    for (std::size_t i = 0; i < cantidadPerfiles_; i++) {
        if (perfiles_[i]->obtenerId() == id) {
            return perfiles_[i];
        }
    }

    return nullptr;
}

bool SoundBridge::eliminarPerfil(int id) noexcept {
    for (std::size_t i = 0; i < cantidadPerfiles_; i++) {
        if (perfiles_[i]->obtenerId() == id) {
            // Primero se eliminan las conexiones relacionadas con el perfil.
            eliminarConexionesDePerfil(id);
            delete perfiles_[i];

            for (std::size_t j = i; j + 1 < cantidadPerfiles_; j++) {
                perfiles_[j] = perfiles_[j + 1];
            }

            cantidadPerfiles_--;
            perfiles_[cantidadPerfiles_] = nullptr;
            return true;
        }
    }

    return false;
}

int SoundBridge::calcularAfinidad(
    int perfilAId,
    int perfilBId
) const noexcept {
    const Perfil* perfilA = buscarPerfil(perfilAId);
    const Perfil* perfilB = buscarPerfil(perfilBId);

    if (perfilA == nullptr || perfilB == nullptr) {
        return -1;
    }

    return calcularAfinidadEntrePerfiles(*perfilA, *perfilB);
}

std::vector<PerfilCompatible> SoundBridge::buscarPerfilesCompatibles(
    int perfilId
) const {
    std::vector<PerfilCompatible> compatibles;
    const Perfil* perfilSeleccionado = buscarPerfil(perfilId);

    if (perfilSeleccionado == nullptr) {
        return compatibles;
    }

    for (std::size_t i = 0; i < cantidadPerfiles_; i++) {
        const Perfil* perfilActual = perfiles_[i];

        if (perfilActual == nullptr
            || perfilActual->obtenerId() == perfilId
            || existeConexion(perfilId, perfilActual->obtenerId())) {
            continue;
        }

        int afinidad = calcularAfinidadEntrePerfiles(
            *perfilSeleccionado,
            *perfilActual
        );

        if (afinidad >= 40) {
            PerfilCompatible resultado;
            resultado.perfil = perfilActual;
            resultado.afinidad = afinidad;
            compatibles.push_back(resultado);
        }
    }

    for (std::size_t i = 0; i < compatibles.size(); i++) {
        for (std::size_t j = i + 1; j < compatibles.size(); j++) {
            bool afinidadMayor = compatibles[j].afinidad
                                  > compatibles[i].afinidad;
            bool mismaAfinidad = compatibles[j].afinidad
                                 == compatibles[i].afinidad;
            bool idMenor = compatibles[j].perfil->obtenerId()
                           < compatibles[i].perfil->obtenerId();

            if (afinidadMayor || (mismaAfinidad && idMenor)) {
                std::swap(compatibles[i], compatibles[j]);
            }
        }
    }

    return compatibles;
}

ResultadoConexion SoundBridge::crearConexion(
    int perfilAId,
    int perfilBId
) {
    const Perfil* perfilA = buscarPerfil(perfilAId);
    const Perfil* perfilB = buscarPerfil(perfilBId);

    if (perfilA == nullptr || perfilB == nullptr) {
        return ResultadoConexion::PerfilNoEncontrado;
    }

    if (perfilAId == perfilBId) {
        return ResultadoConexion::MismoPerfil;
    }

    if (existeConexion(perfilAId, perfilBId)) {
        return ResultadoConexion::YaExiste;
    }

    int afinidad = calcularAfinidadEntrePerfiles(*perfilA, *perfilB);

    if (afinidad < 40) {
        return ResultadoConexion::AfinidadInsuficiente;
    }

    Conexion* conexion = new Conexion(perfilAId, perfilBId, afinidad);
    agregarConexion(conexion);
    return ResultadoConexion::Creada;
}

bool SoundBridge::existeConexion(
    int perfilAId,
    int perfilBId
) const noexcept {
    for (std::size_t i = 0; i < cantidadConexiones_; i++) {
        if (conexiones_[i]->conectaPerfiles(perfilAId, perfilBId)) {
            return true;
        }
    }

    return false;
}

bool SoundBridge::eliminarConexion(
    int perfilAId,
    int perfilBId
) noexcept {
    for (std::size_t i = 0; i < cantidadConexiones_; i++) {
        if (conexiones_[i]->conectaPerfiles(perfilAId, perfilBId)) {
            delete conexiones_[i];

            for (std::size_t j = i; j + 1 < cantidadConexiones_; j++) {
                conexiones_[j] = conexiones_[j + 1];
            }

            cantidadConexiones_--;
            conexiones_[cantidadConexiones_] = nullptr;
            return true;
        }
    }

    return false;
}

std::size_t SoundBridge::obtenerCantidadPerfiles() const noexcept {
    return cantidadPerfiles_;
}

std::size_t SoundBridge::obtenerCapacidadPerfiles() const noexcept {
    return capacidadPerfiles_;
}

int SoundBridge::obtenerSiguientePerfilId() const noexcept {
    return siguientePerfilId_;
}

const Perfil* SoundBridge::obtenerPerfilEn(
    std::size_t posicion
) const noexcept {
    if (posicion >= cantidadPerfiles_) {
        return nullptr;
    }

    return perfiles_[posicion];
}

std::size_t SoundBridge::obtenerCantidadConexiones() const noexcept {
    return cantidadConexiones_;
}

std::size_t SoundBridge::obtenerCapacidadConexiones() const noexcept {
    return capacidadConexiones_;
}

const Conexion* SoundBridge::obtenerConexionEn(
    std::size_t posicion
) const noexcept {
    if (posicion >= cantidadConexiones_) {
        return nullptr;
    }

    return conexiones_[posicion];
}

}
