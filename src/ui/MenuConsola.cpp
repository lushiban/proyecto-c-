#include "soundbridge/ui/MenuConsola.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "soundbridge/application/ControladorAplicacion.hpp"
#include "soundbridge/ui/EntradaConsola.hpp"
#include "soundbridge/visualization/IVisualizadorGraficas.hpp"

namespace soundbridge {

MenuConsola::MenuConsola(
    ControladorAplicacion& controlador,
    IVisualizadorGraficas& visualizador
)
    : controlador_(controlador),
      visualizador_(visualizador) {
}

void MenuConsola::mostrarMenuPrincipal() const {
    std::cout << "\n========== MENU PRINCIPAL ==========" << std::endl;
    std::cout << "1. Crear perfil" << std::endl;
    std::cout << "2. Mostrar perfiles" << std::endl;
    std::cout << "3. Buscar perfil por ID" << std::endl;
    std::cout << "4. Buscar perfiles compatibles" << std::endl;
    std::cout << "5. Conectar perfiles" << std::endl;
    std::cout << "6. Mostrar conexiones" << std::endl;
    std::cout << "7. Eliminar perfil" << std::endl;
    std::cout << "8. Eliminar conexion" << std::endl;
    std::cout << "9. Mostrar grafica de perfiles por genero" << std::endl;
    std::cout << "10. Mostrar grafica de perfiles por tipo" << std::endl;
    std::cout << "11. Mostrar grafica de conexiones por afinidad" << std::endl;
    std::cout << "12. Guardar datos" << std::endl;
    std::cout << "0. Salir" << std::endl;
}

void MenuConsola::mostrarPerfil(const PerfilDTO& perfil) const {
    std::cout << "ID: " << perfil.id << std::endl;
    std::cout << "Tipo: " << perfil.tipo << std::endl;
    std::cout << "Nombre: " << perfil.nombre << std::endl;
    std::cout << "Edad: " << perfil.edad << std::endl;
    std::cout << "Genero principal: " << perfil.generoPrincipal << std::endl;
    std::cout << "Generos secundarios: ";

    for (std::size_t i = 0; i < perfil.generosSecundarios.size(); i++) {
        std::cout << perfil.generosSecundarios[i];

        if (i + 1 < perfil.generosSecundarios.size()) {
            std::cout << ", ";
        }
    }

    std::cout << std::endl;

    for (const CampoDetallePerfilDTO& campo : perfil.camposEspecificos) {
        std::cout << campo.nombre << ": " << campo.valor << std::endl;
    }
}

void MenuConsola::ejecutar() {
    int opcion = -1;

    while (opcion != 0) {
        mostrarMenuPrincipal();
        opcion = EntradaConsola::leerEnteroEnRango(
            "Ingrese una opcion: ",
            0,
            12
        );

        try {
            switch (opcion) {
                case 1:
                    crearPerfil();
                    break;
                case 2:
                    mostrarPerfiles();
                    break;
                case 3:
                    buscarPerfilPorId();
                    break;
                case 4:
                    buscarPerfilesCompatibles();
                    break;
                case 5:
                    conectarPerfiles();
                    break;
                case 6:
                    mostrarConexiones();
                    break;
                case 7:
                    eliminarPerfil();
                    break;
                case 8:
                    eliminarConexion();
                    break;
                case 9:
                    mostrarGraficaPerfilesPorGenero();
                    break;
                case 10:
                    mostrarGraficaPerfilesPorTipo();
                    break;
                case 11:
                    mostrarGraficaConexionesPorAfinidad();
                    break;
                case 12:
                    guardarDatos();
                    break;
                case 0:
                    std::cout << "Saliendo del programa." << std::endl;
                    break;
            }
        } catch (const std::exception& error) {
            std::cout << "No se pudo completar la operacion: "
                      << error.what() << std::endl;
        }
    }
}

void MenuConsola::crearPerfil() {
    std::cout << "\n--- CREAR PERFIL ---" << std::endl;
    std::cout << "1. Oyente" << std::endl;
    std::cout << "2. Artista" << std::endl;
    std::cout << "3. Productor" << std::endl;
    std::cout << "4. Fan Club" << std::endl;
    std::cout << "0. Cancelar" << std::endl;

    int tipo = EntradaConsola::leerEnteroEnRango(
        "Seleccione el tipo de perfil: ",
        0,
        4
    );

    if (tipo == 0) {
        std::cout << "Creacion cancelada." << std::endl;
        return;
    }

    std::string nombre = EntradaConsola::leerTextoNoVacio("Nombre: ");
    int edad = EntradaConsola::leerEnteroEnRango("Edad: ", 13, 120);
    std::string generoPrincipal = EntradaConsola::leerTextoNoVacio(
        "Genero musical principal: "
    );
    std::vector<std::string> generosSecundarios =
        EntradaConsola::leerGenerosSecundarios(generoPrincipal);

    ResultadoCreacionPerfil respuesta{};

    if (tipo == 1) {
        int horas = EntradaConsola::leerEnteroEnRango(
            "Horas de escucha semanales: ",
            0,
            168
        );
        std::string plataforma = EntradaConsola::leerTextoNoVacio(
            "Plataforma preferida: "
        );

        respuesta = controlador_.crearPerfilOyente(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            horas,
            plataforma
        );
    } else if (tipo == 2) {
        std::string nombreArtistico = EntradaConsola::leerTextoNoVacio(
            "Nombre artistico: "
        );
        std::string instrumento = EntradaConsola::leerTextoNoVacio(
            "Instrumento principal: "
        );
        bool buscaColaboraciones = EntradaConsola::leerConfirmacion(
            "Busca colaboraciones? (s/n): "
        );

        respuesta = controlador_.crearPerfilArtista(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            nombreArtistico,
            instrumento,
            buscaColaboraciones
        );
    } else if (tipo == 3) {
        std::string software = EntradaConsola::leerTextoNoVacio(
            "Software principal: "
        );
        int aniosExperiencia = EntradaConsola::leerEnteroEnRango(
            "Anios de experiencia: ",
            0,
            80
        );
        bool ofreceServicios = EntradaConsola::leerConfirmacion(
            "Ofrece servicios? (s/n): "
        );

        respuesta = controlador_.crearPerfilProductor(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            software,
            aniosExperiencia,
            ofreceServicios
        );
    } else {
        std::string artista = EntradaConsola::leerTextoNoVacio(
            "Artista representado: "
        );
        int numeroMiembros = EntradaConsola::leerEnteroMinimo(
            "Numero de miembros: ",
            1
        );
        std::string ciudad = EntradaConsola::leerTextoNoVacio(
            "Ciudad base: "
        );

        respuesta = controlador_.crearPerfilFanClub(
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            artista,
            numeroMiembros,
            ciudad
        );
    }

    std::cout << respuesta.resultado.mensaje << std::endl;

    if (respuesta.resultado.correcto) {
        std::cout << "ID asignado: " << respuesta.idPerfil << std::endl;
        guardarAutomaticamente();
    }
}

void MenuConsola::mostrarPerfiles() const {
    std::cout << "\n--- PERFILES REGISTRADOS ---" << std::endl;
    std::vector<PerfilDTO> perfiles = controlador_.obtenerPerfiles();

    if (perfiles.empty()) {
        std::cout << "No existen perfiles registrados." << std::endl;
        return;
    }

    for (const PerfilDTO& perfil : perfiles) {
        std::cout << "----------------------------------------" << std::endl;
        mostrarPerfil(perfil);
    }
}

void MenuConsola::buscarPerfilPorId() const {
    int id = EntradaConsola::leerEnteroMinimo(
        "Ingrese el ID del perfil: ",
        1
    );
    ResultadoConsultaPerfil respuesta = controlador_.buscarPerfil(id);

    if (!respuesta.resultado.correcto) {
        std::cout << respuesta.resultado.mensaje << std::endl;
        return;
    }

    std::cout << "\n--- PERFIL ENCONTRADO ---" << std::endl;
    mostrarPerfil(respuesta.perfil);
}

void MenuConsola::buscarPerfilesCompatibles() const {
    if (controlador_.obtenerCantidadPerfiles() == 0) {
        std::cout << "No existen perfiles registrados." << std::endl;
        return;
    }

    int id = EntradaConsola::leerEnteroMinimo(
        "Ingrese el ID del perfil: ",
        1
    );
    ResultadoCompatibles respuesta =
        controlador_.buscarPerfilesCompatibles(id);

    if (!respuesta.resultado.correcto || respuesta.perfiles.empty()) {
        std::cout << respuesta.resultado.mensaje << std::endl;
        return;
    }

    std::cout << "\n--- PERFILES COMPATIBLES ---" << std::endl;

    for (const PerfilCompatibleDTO& perfil : respuesta.perfiles) {
        std::cout << "ID: " << perfil.id
                  << " | Nombre: " << perfil.nombre
                  << " | Tipo: " << perfil.tipo
                  << " | Afinidad: " << perfil.afinidad << "%"
                  << std::endl;
    }
}

void MenuConsola::conectarPerfiles() {
    if (controlador_.obtenerCantidadPerfiles() < 2) {
        std::cout << "Se necesitan al menos dos perfiles para crear una conexion."
                  << std::endl;
        return;
    }

    int perfilAId = EntradaConsola::leerEnteroMinimo(
        "ID del primer perfil: ",
        1
    );
    int perfilBId = EntradaConsola::leerEnteroMinimo(
        "ID del segundo perfil: ",
        1
    );

    ResultadoConexionAplicacion respuesta =
        controlador_.crearConexion(perfilAId, perfilBId);
    std::cout << respuesta.resultado.mensaje << std::endl;

    if (respuesta.resultado.correcto) {
        std::cout << "Afinidad registrada: "
                  << respuesta.afinidad << "%" << std::endl;
        guardarAutomaticamente();
    }
}

void MenuConsola::mostrarConexiones() const {
    std::cout << "\n--- CONEXIONES REGISTRADAS ---" << std::endl;
    std::vector<ConexionDTO> conexiones = controlador_.obtenerConexiones();

    if (conexiones.empty()) {
        std::cout << "No existen conexiones registradas." << std::endl;
        return;
    }

    for (const ConexionDTO& conexion : conexiones) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Perfil 1: " << conexion.perfilAId << " - "
                  << conexion.perfilANombre << " ("
                  << conexion.perfilATipo << ")" << std::endl;
        std::cout << "Perfil 2: " << conexion.perfilBId << " - "
                  << conexion.perfilBNombre << " ("
                  << conexion.perfilBTipo << ")" << std::endl;
        std::cout << "Afinidad: " << conexion.afinidad << "%" << std::endl;
    }
}

void MenuConsola::eliminarPerfil() {
    if (controlador_.obtenerCantidadPerfiles() == 0) {
        std::cout << "No existen perfiles registrados." << std::endl;
        return;
    }

    int id = EntradaConsola::leerEnteroMinimo(
        "Ingrese el ID del perfil que desea eliminar: ",
        1
    );
    ResultadoConsultaPerfil consulta = controlador_.buscarPerfil(id);

    if (!consulta.resultado.correcto) {
        std::cout << consulta.resultado.mensaje << std::endl;
        return;
    }

    std::cout << "Perfil seleccionado: " << consulta.perfil.nombre
              << " (ID " << consulta.perfil.id << ")" << std::endl;
    std::cout << "Tambien se eliminaran sus conexiones relacionadas."
              << std::endl;

    bool confirmar = EntradaConsola::leerConfirmacion(
        "Confirma la eliminacion? (s/n): "
    );

    if (!confirmar) {
        std::cout << "Eliminacion cancelada." << std::endl;
        return;
    }

    ResultadoOperacion resultado = controlador_.eliminarPerfil(id);
    std::cout << resultado.mensaje << std::endl;

    if (resultado.correcto) {
        guardarAutomaticamente();
    }
}

void MenuConsola::eliminarConexion() {
    if (controlador_.obtenerCantidadConexiones() == 0) {
        std::cout << "No existen conexiones registradas." << std::endl;
        return;
    }

    int perfilAId = EntradaConsola::leerEnteroMinimo(
        "ID del primer perfil: ",
        1
    );
    int perfilBId = EntradaConsola::leerEnteroMinimo(
        "ID del segundo perfil: ",
        1
    );

    bool confirmar = EntradaConsola::leerConfirmacion(
        "Confirma la eliminacion de la conexion? (s/n): "
    );

    if (!confirmar) {
        std::cout << "Eliminacion cancelada." << std::endl;
        return;
    }

    ResultadoOperacion resultado = controlador_.eliminarConexion(
        perfilAId,
        perfilBId
    );
    std::cout << resultado.mensaje << std::endl;

    if (resultado.correcto) {
        guardarAutomaticamente();
    }
}

void MenuConsola::mostrarGraficaPerfilesPorGenero() {
    visualizador_.mostrar(controlador_.obtenerGraficaPerfilesPorGenero());
}

void MenuConsola::mostrarGraficaPerfilesPorTipo() {
    visualizador_.mostrar(controlador_.obtenerGraficaPerfilesPorTipo());
}

void MenuConsola::mostrarGraficaConexionesPorAfinidad() {
    visualizador_.mostrar(controlador_.obtenerGraficaConexionesPorAfinidad());
}

void MenuConsola::guardarDatos() const {
    ResultadoOperacion resultado = controlador_.guardarDatos();
    std::cout << resultado.mensaje << std::endl;
}

void MenuConsola::guardarAutomaticamente() const {
    ResultadoOperacion resultado = controlador_.guardarDatos();

    if (resultado.correcto) {
        std::cout << "Cambios guardados automaticamente." << std::endl;
    } else {
        std::cout << "El cambio se realizo, pero no pudo guardarse: "
                  << resultado.mensaje << std::endl;
    }
}

}
