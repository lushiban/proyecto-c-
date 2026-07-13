#include "rsm/infrastructure/PersistenciaTexto.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "rsm/application/RedSocialMusical.hpp"
#include "rsm/domain/Conexion.hpp"
#include "rsm/domain/Perfil.hpp"
#include "rsm/domain/PerfilArtista.hpp"
#include "rsm/domain/PerfilFanClub.hpp"
#include "rsm/domain/PerfilOyente.hpp"
#include "rsm/domain/PerfilProductor.hpp"
#include "rsm/domain/TipoPerfil.hpp"

namespace rsm {
namespace {

const char* RUTA_PREDETERMINADA = "data/datos.txt";
const int VERSION_FORMATO = 1;

struct ConexionPendiente {
    int perfilAId;
    int perfilBId;
    int afinidad;
};

std::string escaparTexto(const std::string& texto) {
    std::string resultado;

    for (char caracter : texto) {
        if (caracter == '\\') {
            resultado += "\\\\";
        } else if (caracter == '|') {
            resultado += "\\|";
        } else if (caracter == ';') {
            resultado += "\\;";
        } else if (caracter == '\n') {
            resultado += "\\n";
        } else if (caracter == '\r') {
            resultado += "\\r";
        } else {
            resultado += caracter;
        }
    }

    return resultado;
}

bool desescaparTexto(
    const std::string& texto,
    std::string& resultado,
    std::string& error
) {
    resultado.clear();

    for (std::size_t i = 0; i < texto.size(); i++) {
        if (texto[i] != '\\') {
            resultado += texto[i];
            continue;
        }

        if (i + 1 >= texto.size()) {
            error = "Se encontro una secuencia de escape incompleta.";
            return false;
        }

        i++;
        char escapado = texto[i];

        if (escapado == '\\') {
            resultado += '\\';
        } else if (escapado == '|') {
            resultado += '|';
        } else if (escapado == ';') {
            resultado += ';';
        } else if (escapado == 'n') {
            resultado += '\n';
        } else if (escapado == 'r') {
            resultado += '\r';
        } else {
            error = "Se encontro una secuencia de escape no valida.";
            return false;
        }
    }

    return true;
}

bool separarEscapado(
    const std::string& texto,
    char separador,
    std::vector<std::string>& partes,
    std::string& error
) {
    partes.clear();
    std::string parteActual;

    for (std::size_t i = 0; i < texto.size(); i++) {
        if (texto[i] == '\\') {
            if (i + 1 >= texto.size()) {
                error = "Se encontro una secuencia de escape incompleta.";
                return false;
            }

            parteActual += texto[i];
            i++;
            parteActual += texto[i];
        } else if (texto[i] == separador) {
            partes.push_back(parteActual);
            parteActual.clear();
        } else {
            parteActual += texto[i];
        }
    }

    partes.push_back(parteActual);
    return true;
}

bool convertirEntero(
    const std::string& textoEscapado,
    int& valor,
    std::string& error
) {
    std::string texto;

    if (!desescaparTexto(textoEscapado, texto, error)) {
        return false;
    }

    try {
        std::size_t posicion = 0;
        long numero = std::stol(texto, &posicion);

        if (posicion != texto.size()
            || numero < std::numeric_limits<int>::min()
            || numero > std::numeric_limits<int>::max()) {
            error = "Se encontro un numero entero no valido.";
            return false;
        }

        valor = static_cast<int>(numero);
        return true;
    } catch (const std::exception&) {
        error = "Se encontro un numero entero no valido.";
        return false;
    }
}

bool convertirBooleano(
    const std::string& textoEscapado,
    bool& valor,
    std::string& error
) {
    int numero = 0;

    if (!convertirEntero(textoEscapado, numero, error)) {
        return false;
    }

    if (numero != 0 && numero != 1) {
        error = "Los valores booleanos deben guardarse como 0 o 1.";
        return false;
    }

    valor = numero == 1;
    return true;
}

bool leerTexto(
    const std::string& textoEscapado,
    std::string& texto,
    std::string& error
) {
    return desescaparTexto(textoEscapado, texto, error);
}

bool leerGenerosSecundarios(
    const std::string& campo,
    std::vector<std::string>& generos,
    std::string& error
) {
    std::vector<std::string> partes;

    if (!separarEscapado(campo, ';', partes, error)) {
        return false;
    }

    generos.clear();

    for (const std::string& parte : partes) {
        std::string genero;

        if (!desescaparTexto(parte, genero, error)) {
            return false;
        }

        generos.push_back(genero);
    }

    return true;
}

std::string unirGenerosSecundarios(
    const std::vector<std::string>& generos
) {
    std::string resultado;

    for (std::size_t i = 0; i < generos.size(); i++) {
        resultado += escaparTexto(generos[i]);

        if (i + 1 < generos.size()) {
            resultado += ';';
        }
    }

    return resultado;
}

const char* tipoPerfilAArchivo(TipoPerfil tipo) {
    switch (tipo) {
        case TipoPerfil::Oyente:
            return "OYENTE";
        case TipoPerfil::Artista:
            return "ARTISTA";
        case TipoPerfil::Productor:
            return "PRODUCTOR";
        case TipoPerfil::FanClub:
            return "FAN_CLUB";
    }

    return "DESCONOCIDO";
}

bool escribirPerfil(std::ofstream& salida, const Perfil& perfil) {
    salida << "PROFILE|"
           << perfil.obtenerId() << '|'
           << tipoPerfilAArchivo(perfil.obtenerTipo()) << '|'
           << escaparTexto(perfil.obtenerNombre()) << '|'
           << perfil.obtenerEdad() << '|'
           << escaparTexto(perfil.obtenerGeneroPrincipal()) << '|'
           << unirGenerosSecundarios(perfil.obtenerGenerosSecundarios());

    std::vector<std::string> camposEspecificos =
        perfil.obtenerCamposEspecificos();

    for (const std::string& campo : camposEspecificos) {
        salida << '|' << escaparTexto(campo);
    }

    salida << '\n';
    return salida.good();
}

Perfil* crearPerfilDesdeCampos(
    const std::vector<std::string>& campos,
    std::string& error
) {
    int id = 0;
    int edad = 0;
    std::string tipo;
    std::string nombre;
    std::string generoPrincipal;
    std::vector<std::string> generosSecundarios;

    if (!convertirEntero(campos[1], id, error)
        || !leerTexto(campos[2], tipo, error)
        || !leerTexto(campos[3], nombre, error)
        || !convertirEntero(campos[4], edad, error)
        || !leerTexto(campos[5], generoPrincipal, error)
        || !leerGenerosSecundarios(
            campos[6],
            generosSecundarios,
            error
        )) {
        return nullptr;
    }

    if (tipo == "OYENTE") {
        if (campos.size() != 9) {
            error = "Un perfil OYENTE debe tener 9 campos.";
            return nullptr;
        }

        int horas = 0;
        std::string plataforma;

        if (!convertirEntero(campos[7], horas, error)
            || !leerTexto(campos[8], plataforma, error)) {
            return nullptr;
        }

        return new PerfilOyente(
            id,
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            horas,
            plataforma
        );
    }

    if (tipo == "ARTISTA") {
        if (campos.size() != 10) {
            error = "Un perfil ARTISTA debe tener 10 campos.";
            return nullptr;
        }

        std::string nombreArtistico;
        std::string instrumento;
        bool buscaColaboraciones = false;

        if (!leerTexto(campos[7], nombreArtistico, error)
            || !leerTexto(campos[8], instrumento, error)
            || !convertirBooleano(
                campos[9],
                buscaColaboraciones,
                error
            )) {
            return nullptr;
        }

        return new PerfilArtista(
            id,
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            nombreArtistico,
            instrumento,
            buscaColaboraciones
        );
    }

    if (tipo == "PRODUCTOR") {
        if (campos.size() != 10) {
            error = "Un perfil PRODUCTOR debe tener 10 campos.";
            return nullptr;
        }

        std::string software;
        int aniosExperiencia = 0;
        bool ofreceServicios = false;

        if (!leerTexto(campos[7], software, error)
            || !convertirEntero(campos[8], aniosExperiencia, error)
            || !convertirBooleano(
                campos[9],
                ofreceServicios,
                error
            )) {
            return nullptr;
        }

        return new PerfilProductor(
            id,
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            software,
            aniosExperiencia,
            ofreceServicios
        );
    }

    if (tipo == "FAN_CLUB") {
        if (campos.size() != 10) {
            error = "Un perfil FAN_CLUB debe tener 10 campos.";
            return nullptr;
        }

        std::string artistaRepresentado;
        int numeroMiembros = 0;
        std::string ciudadBase;

        if (!leerTexto(campos[7], artistaRepresentado, error)
            || !convertirEntero(campos[8], numeroMiembros, error)
            || !leerTexto(campos[9], ciudadBase, error)) {
            return nullptr;
        }

        return new PerfilFanClub(
            id,
            nombre,
            edad,
            generoPrincipal,
            generosSecundarios,
            artistaRepresentado,
            numeroMiembros,
            ciudadBase
        );
    }

    error = "Se encontro un tipo de perfil desconocido.";
    return nullptr;
}

std::filesystem::path crearRutaTemporal(
    const std::filesystem::path& rutaArchivo
) {
    return rutaArchivo.parent_path()
           / (rutaArchivo.stem().string() + ".tmp");
}

}

bool PersistenciaTexto::guardar(
    const RedSocialMusical& red,
    std::string& mensaje
) {
    return guardar(red, RUTA_PREDETERMINADA, mensaje);
}

bool PersistenciaTexto::guardar(
    const RedSocialMusical& red,
    const std::string& rutaArchivo,
    std::string& mensaje
) {
    namespace fs = std::filesystem;

    try {
        fs::path ruta(rutaArchivo);
        fs::path directorio = ruta.parent_path();
        fs::path temporal = crearRutaTemporal(ruta);
        fs::path respaldo = ruta;
        respaldo += ".bak";

        if (!directorio.empty()) {
            std::error_code errorDirectorio;
            fs::create_directories(directorio, errorDirectorio);

            if (errorDirectorio) {
                mensaje = "No se pudo crear el directorio de datos.";
                return false;
            }
        }

        std::error_code errorEliminar;
        fs::remove(temporal, errorEliminar);

        std::ofstream salida(
            temporal,
            std::ios::binary | std::ios::trunc
        );

        if (!salida.is_open()) {
            mensaje = "No se pudo abrir el archivo temporal para guardar.";
            return false;
        }

        salida << "VERSION|" << VERSION_FORMATO << '\n';
        salida << "NEXT_PROFILE_ID|"
               << red.obtenerSiguientePerfilId() << '\n';

        for (std::size_t i = 0; i < red.obtenerCantidadPerfiles(); i++) {
            const Perfil* perfil = red.obtenerPerfilEn(i);

            if (perfil == nullptr || !escribirPerfil(salida, *perfil)) {
                salida.close();
                fs::remove(temporal, errorEliminar);
                mensaje = "No se pudieron escribir todos los perfiles.";
                return false;
            }
        }

        for (std::size_t i = 0; i < red.obtenerCantidadConexiones(); i++) {
            const Conexion* conexion = red.obtenerConexionEn(i);

            if (conexion == nullptr) {
                salida.close();
                fs::remove(temporal, errorEliminar);
                mensaje = "No se pudieron escribir todas las conexiones.";
                return false;
            }

            salida << "CONNECTION|"
                   << conexion->obtenerPerfilAId() << '|'
                   << conexion->obtenerPerfilBId() << '|'
                   << conexion->obtenerPorcentajeAfinidad() << '\n';
        }

        salida.close();

        if (!salida.good()) {
            fs::remove(temporal, errorEliminar);
            mensaje = "Ocurrio un error al cerrar el archivo temporal.";
            return false;
        }

        std::error_code error;
        fs::remove(respaldo, error);
        error.clear();

        bool teniaArchivoAnterior = fs::exists(ruta, error) && !error;

        if (teniaArchivoAnterior) {
            fs::rename(ruta, respaldo, error);

            if (error) {
                fs::remove(temporal, errorEliminar);
                mensaje = "No se pudo preparar el archivo anterior para reemplazarlo.";
                return false;
            }
        }

        error.clear();
        fs::rename(temporal, ruta, error);

        if (error) {
            if (teniaArchivoAnterior) {
                std::error_code errorRestaurar;
                fs::rename(respaldo, ruta, errorRestaurar);
            }

            fs::remove(temporal, errorEliminar);
            mensaje = "No se pudo reemplazar el archivo de datos.";
            return false;
        }

        if (teniaArchivoAnterior) {
            fs::remove(respaldo, error);
        }

        mensaje = "Datos guardados correctamente.";
        return true;
    } catch (const std::exception& error) {
        mensaje = std::string("No se pudieron guardar los datos: ")
                  + error.what();
        return false;
    }
}

bool PersistenciaTexto::cargar(
    RedSocialMusical& red,
    std::string& mensaje
) {
    return cargar(red, RUTA_PREDETERMINADA, mensaje);
}

bool PersistenciaTexto::cargar(
    RedSocialMusical& red,
    const std::string& rutaArchivo,
    std::string& mensaje
) {
    namespace fs = std::filesystem;

    try {
        std::error_code errorArchivo;

        if (!fs::exists(rutaArchivo, errorArchivo)) {
            if (errorArchivo) {
                mensaje = "No se pudo comprobar el archivo de datos.";
                return false;
            }

            RedSocialMusical temporal;
            red.intercambiar(temporal);
            mensaje = "El archivo no existe. Se inicio una red vacia.";
            return true;
        }

        std::ifstream entrada(rutaArchivo, std::ios::binary);

        if (!entrada.is_open()) {
            mensaje = "No se pudo abrir el archivo de datos.";
            return false;
        }

        RedSocialMusical temporal;
        std::vector<ConexionPendiente> conexionesPendientes;
        std::string linea;
        int numeroLinea = 0;
        bool versionLeida = false;
        bool siguienteIdLeido = false;
        int siguienteIdGuardado = 0;
        int idMaximo = 0;

        while (std::getline(entrada, linea)) {
            numeroLinea++;

            if (!linea.empty() && linea.back() == '\r') {
                linea.pop_back();
            }

            if (linea.empty()) {
                continue;
            }

            std::vector<std::string> campos;
            std::string error;

            if (!separarEscapado(linea, '|', campos, error)) {
                mensaje = "Error en la linea "
                          + std::to_string(numeroLinea)
                          + ": " + error;
                return false;
            }

            std::string tipoRegistro;

            if (!leerTexto(campos[0], tipoRegistro, error)) {
                mensaje = "Error en la linea "
                          + std::to_string(numeroLinea)
                          + ": " + error;
                return false;
            }

            if (!versionLeida) {
                int version = 0;

                if (tipoRegistro != "VERSION"
                    || campos.size() != 2
                    || !convertirEntero(campos[1], version, error)
                    || version != VERSION_FORMATO) {
                    mensaje = "La primera linea debe indicar VERSION|1.";
                    return false;
                }

                versionLeida = true;
                continue;
            }

            if (tipoRegistro == "VERSION") {
                mensaje = "El archivo contiene mas de una version.";
                return false;
            }

            if (tipoRegistro == "NEXT_PROFILE_ID") {
                if (siguienteIdLeido
                    || campos.size() != 2
                    || !convertirEntero(
                        campos[1],
                        siguienteIdGuardado,
                        error
                    )
                    || siguienteIdGuardado <= 0) {
                    mensaje = "NEXT_PROFILE_ID no es valido.";
                    return false;
                }

                siguienteIdLeido = true;
                continue;
            }

            if (tipoRegistro == "PROFILE") {
                if (campos.size() < 7) {
                    mensaje = "Un registro PROFILE esta incompleto.";
                    return false;
                }

                int id = 0;

                if (!convertirEntero(campos[1], id, error)) {
                    mensaje = "Error en la linea "
                              + std::to_string(numeroLinea)
                              + ": " + error;
                    return false;
                }

                if (temporal.buscarPerfil(id) != nullptr) {
                    mensaje = "El archivo contiene IDs de perfil duplicados.";
                    return false;
                }

                Perfil* perfil = nullptr;

                try {
                    perfil = crearPerfilDesdeCampos(campos, error);
                } catch (const std::exception& excepcion) {
                    error = excepcion.what();
                }

                if (perfil == nullptr) {
                    mensaje = "Error en la linea "
                              + std::to_string(numeroLinea)
                              + ": " + error;
                    return false;
                }

                temporal.agregarPerfil(perfil);
                idMaximo = std::max(idMaximo, id);
                continue;
            }

            if (tipoRegistro == "CONNECTION") {
                if (campos.size() != 4) {
                    mensaje = "Un registro CONNECTION debe tener 4 campos.";
                    return false;
                }

                ConexionPendiente pendiente{};

                if (!convertirEntero(
                        campos[1],
                        pendiente.perfilAId,
                        error
                    )
                    || !convertirEntero(
                        campos[2],
                        pendiente.perfilBId,
                        error
                    )
                    || !convertirEntero(
                        campos[3],
                        pendiente.afinidad,
                        error
                    )) {
                    mensaje = "Error en la linea "
                              + std::to_string(numeroLinea)
                              + ": " + error;
                    return false;
                }

                conexionesPendientes.push_back(pendiente);
                continue;
            }

            mensaje = "Se encontro un tipo de registro desconocido en la linea "
                      + std::to_string(numeroLinea) + ".";
            return false;
        }

        if (!entrada.eof() && entrada.fail()) {
            mensaje = "Ocurrio un error mientras se leia el archivo.";
            return false;
        }

        if (!versionLeida) {
            mensaje = "El archivo esta vacio o no contiene una version.";
            return false;
        }

        if (!siguienteIdLeido) {
            mensaje = "Falta NEXT_PROFILE_ID en el archivo.";
            return false;
        }

        if (idMaximo == std::numeric_limits<int>::max()) {
            mensaje = "No es posible calcular el siguiente ID disponible.";
            return false;
        }

        for (const ConexionPendiente& pendiente : conexionesPendientes) {
            if (temporal.buscarPerfil(pendiente.perfilAId) == nullptr
                || temporal.buscarPerfil(pendiente.perfilBId) == nullptr) {
                mensaje = "Una conexion hace referencia a un perfil inexistente.";
                return false;
            }

            if (pendiente.perfilAId == pendiente.perfilBId) {
                mensaje = "El archivo contiene una autoconexion.";
                return false;
            }

            if (pendiente.afinidad < 40 || pendiente.afinidad > 100) {
                mensaje = "Una conexion contiene una afinidad no valida.";
                return false;
            }

            if (temporal.existeConexion(
                    pendiente.perfilAId,
                    pendiente.perfilBId
                )) {
                mensaje = "El archivo contiene conexiones duplicadas.";
                return false;
            }

            int afinidadCalculada = temporal.calcularAfinidad(
                pendiente.perfilAId,
                pendiente.perfilBId
            );

            if (afinidadCalculada != pendiente.afinidad) {
                mensaje = "La afinidad guardada no coincide con los perfiles.";
                return false;
            }

            Conexion* conexion = new Conexion(
                pendiente.perfilAId,
                pendiente.perfilBId,
                pendiente.afinidad
            );

            temporal.agregarConexion(conexion);
        }

        temporal.siguientePerfilId_ = std::max(
            siguienteIdGuardado,
            idMaximo + 1
        );

        red.intercambiar(temporal);
        mensaje = "Datos cargados correctamente.";
        return true;
    } catch (const std::exception& error) {
        mensaje = std::string("No se pudieron cargar los datos: ")
                  + error.what();
        return false;
    }
}

}
