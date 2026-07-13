#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "soundbridge/application/ControladorAplicacion.hpp"
#include "soundbridge/visualization/IVisualizadorGraficas.hpp"

namespace {

void comprobar(bool condicion, const std::string& mensaje) {
    if (!condicion) {
        throw std::runtime_error(mensaje);
    }
}

void comprobarMensaje(const soundbridge::ResultadoOperacion& resultado) {
    comprobar(!resultado.mensaje.empty(),
              "Toda operacion debe devolver un mensaje para la interfaz.");
}

class VisualizadorSimulado final : public soundbridge::IVisualizadorGraficas {
public:
    int cantidadLlamadas = 0;
    soundbridge::GraficaDTO ultimaGrafica;

    void mostrar(const soundbridge::GraficaDTO& grafica) override {
        cantidadLlamadas++;
        ultimaGrafica = grafica;
    }
};

struct DatosPrueba {
    soundbridge::ControladorAplicacion controlador;
    int idPaula;
    int idEduardo;
    int idCamila;
    int idFanClub;

    DatosPrueba()
        : idPaula(0),
          idEduardo(0),
          idCamila(0),
          idFanClub(0) {
        soundbridge::ResultadoCreacionPerfil paula = controlador.crearPerfilOyente(
            "Paula",
            20,
            "Rock",
            {"Pop", "Indie"},
            18,
            "Spotify"
        );
        comprobar(paula.resultado.correcto,
                  "El perfil de prueba Paula debe crearse.");
        idPaula = paula.idPerfil;

        soundbridge::ResultadoCreacionPerfil eduardo = controlador.crearPerfilArtista(
            "Eduardo",
            21,
            "Rock",
            {"Blues", "Pop"},
            "Edo T",
            "Guitarra",
            true
        );
        comprobar(eduardo.resultado.correcto,
                  "El perfil de prueba Eduardo debe crearse.");
        idEduardo = eduardo.idPerfil;

        soundbridge::ResultadoCreacionPerfil camila = controlador.crearPerfilProductor(
            "Camila",
            24,
            "Salsa",
            {"Merengue", "Bachata"},
            "Ableton Live",
            4,
            true
        );
        comprobar(camila.resultado.correcto,
                  "El perfil de prueba Camila debe crearse.");
        idCamila = camila.idPerfil;

        soundbridge::ResultadoCreacionPerfil fanClub = controlador.crearPerfilFanClub(
            "Fans de Aurora",
            19,
            "Pop",
            {"Rock", "Indie"},
            "Aurora",
            120,
            "Quito"
        );
        comprobar(fanClub.resultado.correcto,
                  "El fan club de prueba debe crearse.");
        idFanClub = fanClub.idPerfil;
    }
};

void probarFachadaAutonomaYModelos() {
    static_assert(
        std::is_default_constructible<soundbridge::ControladorAplicacion>::value,
        "El front-end debe poder crear el controlador directamente."
    );
    static_assert(
        !std::is_copy_constructible<soundbridge::ControladorAplicacion>::value,
        "El controlador no debe copiar el estado propietario."
    );

    DatosPrueba datos;

    comprobar(datos.controlador.obtenerCantidadPerfiles() == 4,
              "Deben existir cuatro perfiles.");

    std::vector<soundbridge::PerfilDTO> perfiles = datos.controlador.obtenerPerfiles();
    comprobar(perfiles.size() == 4,
              "La lista debe contener cuatro perfiles.");
    comprobar(perfiles[0].id == 1 && perfiles[0].tipo == "Oyente",
              "El primer DTO debe representar al oyente.");
    comprobar(perfiles[0].camposEspecificos.size() == 2,
              "El oyente debe exponer dos campos especificos.");

    soundbridge::ResultadoConsultaPerfil artista =
        datos.controlador.buscarPerfil(datos.idEduardo);
    comprobar(artista.resultado.correcto,
              "El artista creado debe encontrarse.");
    comprobarMensaje(artista.resultado);
    comprobar(artista.perfil.camposEspecificos.size() == 3,
              "El artista debe exponer tres campos especificos.");

    comprobar(datos.controlador.buscarPerfil(datos.idCamila).perfil.tipo
                  == "Productor",
              "El productor debe convertirse a DTO.");
    comprobar(datos.controlador.buscarPerfil(datos.idFanClub).perfil.tipo
                  == "Fan Club",
              "El fan club debe convertirse a DTO.");
}

void probarValidacionesDePerfiles() {
    soundbridge::ControladorAplicacion controlador;

    soundbridge::ResultadoCreacionPerfil nombreVacio = controlador.crearPerfilOyente(
        "   ", 20, "Rock", {"Pop"}, 5, "Spotify"
    );
    comprobar(!nombreVacio.resultado.correcto,
              "Un nombre vacio debe rechazarse.");
    comprobarMensaje(nombreVacio.resultado);

    soundbridge::ResultadoCreacionPerfil edadInvalida = controlador.crearPerfilOyente(
        "Persona", 12, "Rock", {"Pop"}, 5, "Spotify"
    );
    comprobar(!edadInvalida.resultado.correcto,
              "Una edad menor a 13 debe rechazarse.");

    soundbridge::ResultadoCreacionPerfil secundariosVacios =
        controlador.crearPerfilOyente(
            "Persona", 20, "Rock", {}, 5, "Spotify"
        );
    comprobar(!secundariosVacios.resultado.correcto,
              "Debe existir al menos un genero secundario.");

    soundbridge::ResultadoCreacionPerfil generoRepetido =
        controlador.crearPerfilArtista(
            "Artista", 20, "Rock", {" rock "},
            "Nombre", "Guitarra", false
        );
    comprobar(!generoRepetido.resultado.correcto,
              "El genero principal repetido debe rechazarse.");

    soundbridge::ResultadoCreacionPerfil secundariosDuplicados =
        controlador.crearPerfilProductor(
            "Productor", 30, "Rock", {"Pop", " pop "},
            "FL Studio", 3, true
        );
    comprobar(!secundariosDuplicados.resultado.correcto,
              "Los generos secundarios duplicados deben rechazarse.");

    soundbridge::ResultadoCreacionPerfil horasInvalidas =
        controlador.crearPerfilOyente(
            "Oyente", 20, "Rock", {"Pop"}, 169, "Spotify"
        );
    comprobar(!horasInvalidas.resultado.correcto,
              "Las horas mayores a 168 deben rechazarse.");

    soundbridge::ResultadoCreacionPerfil artistaInvalido =
        controlador.crearPerfilArtista(
            "Artista", 20, "Rock", {"Pop"},
            "", "Guitarra", false
        );
    comprobar(!artistaInvalido.resultado.correcto,
              "El nombre artistico vacio debe rechazarse.");

    soundbridge::ResultadoCreacionPerfil productorInvalido =
        controlador.crearPerfilProductor(
            "Productor", 30, "Rock", {"Pop"},
            "Ableton", 81, true
        );
    comprobar(!productorInvalido.resultado.correcto,
              "Mas de 80 anios de experiencia deben rechazarse.");

    soundbridge::ResultadoCreacionPerfil fanClubInvalido =
        controlador.crearPerfilFanClub(
            "Club", 20, "Pop", {"Rock"},
            "Artista", 0, "Quito"
        );
    comprobar(!fanClubInvalido.resultado.correcto,
              "Un fan club sin miembros debe rechazarse.");

    comprobar(controlador.obtenerCantidadPerfiles() == 0,
              "Las validaciones fallidas no deben modificar el estado.");

    soundbridge::ResultadoConsultaPerfil idInvalido = controlador.buscarPerfil(0);
    comprobar(!idInvalido.resultado.correcto,
              "Un ID igual a cero debe rechazarse.");
    comprobarMensaje(idInvalido.resultado);
}

void probarAfinidadYConexiones() {
    DatosPrueba datos;

    soundbridge::ResultadoAfinidad afinidad = datos.controlador.calcularAfinidad(
        datos.idPaula,
        datos.idEduardo
    );
    comprobar(afinidad.resultado.correcto && afinidad.afinidad == 65,
              "La afinidad Rock con un secundario compartido debe ser 65.");
    comprobarMensaje(afinidad.resultado);

    soundbridge::ResultadoAfinidad mismaPersona = datos.controlador.calcularAfinidad(
        datos.idPaula,
        datos.idPaula
    );
    comprobar(!mismaPersona.resultado.correcto,
              "No debe calcularse afinidad con el mismo perfil.");

    soundbridge::ResultadoConexionAplicacion creada = datos.controlador.crearConexion(
        datos.idPaula,
        datos.idEduardo
    );
    comprobar(creada.resultado.correcto && creada.afinidad == 65,
              "La conexion compatible debe crearse.");

    soundbridge::ResultadoConexionAplicacion duplicada =
        datos.controlador.crearConexion(datos.idEduardo, datos.idPaula);
    comprobar(!duplicada.resultado.correcto,
              "La conexion inversa duplicada debe rechazarse.");
    comprobarMensaje(duplicada.resultado);

    soundbridge::ResultadoConexionAplicacion autoconexion =
        datos.controlador.crearConexion(datos.idPaula, datos.idPaula);
    comprobar(!autoconexion.resultado.correcto,
              "La autoconexion debe rechazarse.");

    soundbridge::ResultadoConexionAplicacion inexistente =
        datos.controlador.crearConexion(datos.idPaula, 999);
    comprobar(!inexistente.resultado.correcto,
              "Una conexion con perfil inexistente debe rechazarse.");

    soundbridge::ResultadoConexionAplicacion insuficiente =
        datos.controlador.crearConexion(datos.idPaula, datos.idCamila);
    comprobar(!insuficiente.resultado.correcto,
              "Una afinidad menor a 40 debe rechazarse.");

    std::vector<soundbridge::ConexionDTO> conexiones =
        datos.controlador.obtenerConexiones();
    comprobar(conexiones.size() == 1,
              "Solo debe existir una conexion valida.");
    comprobar(conexiones[0].perfilANombre == "Paula"
                  && conexiones[0].perfilBNombre == "Eduardo"
                  && conexiones[0].afinidad == 65,
              "La conexion debe devolver datos completos para la interfaz.");

    soundbridge::ResultadoOperacion eliminada = datos.controlador.eliminarConexion(
        datos.idPaula,
        datos.idEduardo
    );
    comprobar(eliminada.correcto,
              "La conexion existente debe eliminarse.");
    comprobarMensaje(eliminada);
    comprobar(datos.controlador.obtenerCantidadConexiones() == 0,
              "La lista debe quedar sin conexiones.");

    soundbridge::ResultadoOperacion segundaEliminacion =
        datos.controlador.eliminarConexion(datos.idPaula, datos.idEduardo);
    comprobar(!segundaEliminacion.correcto,
              "Eliminar una conexion inexistente debe devolver error.");
}

void probarCompatibilidadYOrden() {
    soundbridge::ControladorAplicacion controlador;

    int principal = controlador.crearPerfilOyente(
        "Principal", 20, "Rock", {"Pop"}, 10, "Spotify"
    ).idPerfil;
    int primero = controlador.crearPerfilArtista(
        "Primero", 21, "Rock", {"Jazz", "Pop"},
        "Uno", "Guitarra", true
    ).idPerfil;
    int segundo = controlador.crearPerfilProductor(
        "Segundo", 22, "Rock", {"Blues", "Pop"},
        "FL Studio", 2, false
    ).idPerfil;
    controlador.crearPerfilFanClub(
        "Sin afinidad", 23, "Salsa", {"Merengue"},
        "Grupo", 10, "Quito"
    );

    soundbridge::ResultadoCompatibles compatibles =
        controlador.buscarPerfilesCompatibles(principal);
    comprobar(compatibles.resultado.correcto,
              "La consulta de compatibilidad debe ser correcta.");
    comprobarMensaje(compatibles.resultado);
    comprobar(compatibles.perfiles.size() == 2,
              "Deben existir dos perfiles compatibles.");
    comprobar(compatibles.perfiles[0].id == primero
                  && compatibles.perfiles[1].id == segundo,
              "El empate debe ordenarse por ID ascendente.");

    controlador.crearConexion(principal, primero);
    compatibles = controlador.buscarPerfilesCompatibles(principal);
    comprobar(compatibles.perfiles.size() == 1
                  && compatibles.perfiles[0].id == segundo,
              "Una conexion existente debe excluirse de compatibles.");

    soundbridge::ResultadoCompatibles perfilInexistente =
        controlador.buscarPerfilesCompatibles(999);
    comprobar(!perfilInexistente.resultado.correcto,
              "La compatibilidad de un perfil inexistente debe fallar.");
}

void probarEliminacionEnCascada() {
    DatosPrueba datos;
    datos.controlador.crearConexion(datos.idPaula, datos.idEduardo);
    comprobar(datos.controlador.obtenerCantidadConexiones() == 1,
              "Debe existir una conexion antes de eliminar.");

    soundbridge::ResultadoOperacion eliminado =
        datos.controlador.eliminarPerfil(datos.idPaula);
    comprobar(eliminado.correcto, "El perfil debe eliminarse.");
    comprobarMensaje(eliminado);
    comprobar(datos.controlador.obtenerCantidadPerfiles() == 3,
              "La cantidad de perfiles debe disminuir.");
    comprobar(datos.controlador.obtenerCantidadConexiones() == 0,
              "Las conexiones relacionadas deben eliminarse.");
    comprobar(!datos.controlador.buscarPerfil(datos.idPaula).resultado.correcto,
              "El perfil eliminado no debe encontrarse.");
}

void probarEstadisticasYVisualizador() {
    soundbridge::ControladorAplicacion vacio;
    soundbridge::GraficaDTO tiposVacios = vacio.obtenerGraficaPerfilesPorTipo();
    soundbridge::GraficaDTO rangosVacios =
        vacio.obtenerGraficaConexionesPorAfinidad();
    comprobar(tiposVacios.puntos.size() == 4,
              "La grafica vacia por tipo debe conservar cuatro categorias.");
    comprobar(rangosVacios.puntos.size() == 3,
              "La grafica vacia por afinidad debe conservar tres rangos.");

    DatosPrueba datos;
    datos.controlador.crearConexion(datos.idPaula, datos.idEduardo);

    soundbridge::GraficaDTO tipos = datos.controlador.obtenerGraficaPerfilesPorTipo();
    comprobar(tipos.puntos.size() == 4,
              "La grafica por tipo debe conservar cuatro categorias.");
    comprobar(tipos.puntos[0].valor == 1.0
                  && tipos.puntos[1].valor == 1.0
                  && tipos.puntos[2].valor == 1.0
                  && tipos.puntos[3].valor == 1.0,
              "Cada tipo debe tener un perfil.");

    soundbridge::GraficaDTO conexiones =
        datos.controlador.obtenerGraficaConexionesPorAfinidad();
    comprobar(conexiones.puntos[1].valor == 1.0,
              "La conexion de 65 debe estar en el rango 60-79.");

    VisualizadorSimulado visualizador;
    visualizador.mostrar(datos.controlador.obtenerGraficaPerfilesPorGenero());
    comprobar(visualizador.cantidadLlamadas == 1,
              "El visualizador simulado debe recibir una llamada.");
    comprobar(!visualizador.ultimaGrafica.puntos.empty(),
              "El visualizador debe recibir datos reales.");
}

void probarPersistenciaYRecuperacion() {
    namespace fs = std::filesystem;

    fs::path carpeta = fs::temp_directory_path() / "soundbridge_pruebas_fase10";
    fs::remove_all(carpeta);
    fs::create_directories(carpeta);
    fs::path ruta = carpeta / "datos.txt";

    {
        soundbridge::ControladorAplicacion controlador;
        soundbridge::ResultadoCreacionPerfil primero = controlador.crearPerfilOyente(
            "Ana|Luz",
            25,
            "Rock;Alternativo",
            {"Pop\\Indie", "Jazz"},
            12,
            "Plataforma\nLocal"
        );
        soundbridge::ResultadoCreacionPerfil segundo = controlador.crearPerfilArtista(
            "Eduardo",
            21,
            "Rock;Alternativo",
            {"Blues", "Jazz"},
            "Edo T",
            "Guitarra",
            true
        );
        comprobar(primero.resultado.correcto && segundo.resultado.correcto,
                  "Los perfiles con caracteres escapables deben crearse.");
        comprobar(controlador.crearConexion(
                      primero.idPerfil,
                      segundo.idPerfil
                  ).resultado.correcto,
                  "La conexion de persistencia debe crearse.");

        soundbridge::ResultadoOperacion guardado =
            controlador.guardarDatos(ruta.string());
        comprobar(guardado.correcto, "El archivo debe guardarse.");
        comprobarMensaje(guardado);
        comprobar(fs::exists(ruta), "El archivo guardado debe existir.");
    }

    soundbridge::ControladorAplicacion controladorCargado;
    soundbridge::ResultadoOperacion carga =
        controladorCargado.cargarDatos(ruta.string());
    comprobar(carga.correcto, "Los datos guardados deben cargarse.");
    comprobarMensaje(carga);
    comprobar(controladorCargado.obtenerCantidadPerfiles() == 2,
              "La carga debe recuperar dos perfiles.");
    comprobar(controladorCargado.obtenerCantidadConexiones() == 1,
              "La carga debe recuperar una conexion.");

    soundbridge::ResultadoConsultaPerfil recuperado =
        controladorCargado.buscarPerfil(1);
    comprobar(recuperado.resultado.correcto
                  && recuperado.perfil.nombre == "Ana|Luz"
                  && recuperado.perfil.generoPrincipal == "Rock;Alternativo",
              "Los caracteres reservados deben recuperarse sin cambios.");

    soundbridge::ResultadoCreacionPerfil nuevo =
        controladorCargado.crearPerfilOyente(
            "Nuevo", 30, "Jazz", {"Blues"}, 8, "Deezer"
        );
    comprobar(nuevo.resultado.correcto && nuevo.idPerfil == 3,
              "El siguiente ID debe conservarse despues de cargar.");

    soundbridge::ResultadoOperacion segundoGuardado =
        controladorCargado.guardarDatos(ruta.string());
    comprobar(segundoGuardado.correcto,
              "Los cambios posteriores deben guardarse.");

    soundbridge::ControladorAplicacion segundaEjecucion;
    comprobar(segundaEjecucion.cargarDatos(ruta.string()).correcto,
              "Una nueva instancia debe recuperar el archivo actualizado.");
    comprobar(segundaEjecucion.obtenerCantidadPerfiles() == 3,
              "La nueva ejecucion debe recuperar el ultimo cambio.");

    fs::remove_all(carpeta);
}

void probarArchivoInvalidoNoReemplazaEstado() {
    namespace fs = std::filesystem;

    fs::path carpeta = fs::temp_directory_path()
                       / "soundbridge_prueba_invalida_fase10";
    fs::remove_all(carpeta);
    fs::create_directories(carpeta);
    fs::path ruta = carpeta / "datos.txt";

    std::ofstream salida(ruta);
    salida << "VERSION|1\n";
    salida << "NEXT_PROFILE_ID|2\n";
    salida << "PROFILE|1|OYENTE|Dato incompleto\n";
    salida.close();

    soundbridge::ControladorAplicacion controlador;
    controlador.crearPerfilOyente(
        "Existente", 20, "Rock", {"Pop"}, 5, "Spotify"
    );

    soundbridge::ResultadoOperacion carga = controlador.cargarDatos(ruta.string());
    comprobar(!carga.correcto, "El archivo incompleto debe rechazarse.");
    comprobarMensaje(carga);
    comprobar(controlador.obtenerCantidadPerfiles() == 1,
              "Una carga fallida no debe reemplazar el estado actual.");
    comprobar(controlador.buscarPerfil(1).perfil.nombre == "Existente",
              "El perfil original debe conservarse.");

    fs::remove_all(carpeta);
}

void probarInicioSinArchivo() {
    namespace fs = std::filesystem;

    fs::path carpeta = fs::temp_directory_path() / "soundbridge_sin_archivo_fase10";
    fs::remove_all(carpeta);
    fs::path ruta = carpeta / "datos.txt";

    soundbridge::ControladorAplicacion controlador;
    soundbridge::ResultadoOperacion carga = controlador.cargarDatos(ruta.string());
    comprobar(carga.correcto,
              "La falta del archivo inicial debe manejarse correctamente.");
    comprobarMensaje(carga);
    comprobar(controlador.obtenerCantidadPerfiles() == 0,
              "El sistema debe iniciar con una red vacia.");

    fs::remove_all(carpeta);
}

}

int main() {
    std::vector<std::pair<std::string, std::function<void()>>> pruebas = {
        {"Fachada autonoma y modelos para la interfaz",
         probarFachadaAutonomaYModelos},
        {"Validaciones completas de perfiles", probarValidacionesDePerfiles},
        {"Afinidad, conexiones y resultados", probarAfinidadYConexiones},
        {"Compatibilidad y orden", probarCompatibilidadYOrden},
        {"Eliminacion en cascada", probarEliminacionEnCascada},
        {"Estadisticas y visualizador simulado",
         probarEstadisticasYVisualizador},
        {"Persistencia y recuperacion en nueva ejecucion",
         probarPersistenciaYRecuperacion},
        {"Archivo invalido conserva el estado",
         probarArchivoInvalidoNoReemplazaEstado},
        {"Inicio correcto sin archivo", probarInicioSinArchivo}
    };

    int aprobadas = 0;

    for (const auto& prueba : pruebas) {
        try {
            prueba.second();
            aprobadas++;
            std::cout << "[APROBADA] " << prueba.first << std::endl;
        } catch (const std::exception& error) {
            std::cout << "[FALLIDA] " << prueba.first
                      << ": " << error.what() << std::endl;
        }
    }

    std::cout << "Resultado: " << aprobadas << " de "
              << pruebas.size() << " pruebas aprobadas." << std::endl;

    return aprobadas == static_cast<int>(pruebas.size()) ? 0 : 1;
}
