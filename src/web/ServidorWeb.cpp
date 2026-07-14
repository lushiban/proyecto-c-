#include "soundbridge/web/ServidorWeb.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
using SocketServidor = SOCKET;
constexpr SocketServidor SOCKET_INVALIDO = INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using SocketServidor = int;
constexpr SocketServidor SOCKET_INVALIDO = -1;
#endif

namespace soundbridge {
namespace {

struct SolicitudHttp {
    std::string metodo;
    std::string ruta;
    std::string consulta;
    std::map<std::string, std::string> cabeceras;
    std::string cuerpo;
};

struct RespuestaHttp {
    int estado = 200;
    std::string tipoContenido = "application/json; charset=utf-8";
    std::string cuerpo;
};

void cerrarSocket(SocketServidor socket) {
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

std::string minusculas(std::string texto) {
    std::transform(
        texto.begin(),
        texto.end(),
        texto.begin(),
        [](unsigned char caracter) {
            return static_cast<char>(std::tolower(caracter));
        }
    );
    return texto;
}

std::string recortar(const std::string& texto) {
    std::size_t inicio = 0;
    while (inicio < texto.size()
           && std::isspace(static_cast<unsigned char>(texto[inicio]))) {
        inicio++;
    }

    std::size_t final = texto.size();
    while (final > inicio
           && std::isspace(static_cast<unsigned char>(texto[final - 1]))) {
        final--;
    }

    return texto.substr(inicio, final - inicio);
}

std::string escaparJson(const std::string& texto) {
    std::ostringstream salida;
    for (unsigned char caracter : texto) {
        switch (caracter) {
            case '"': salida << "\\\""; break;
            case '\\': salida << "\\\\"; break;
            case '\b': salida << "\\b"; break;
            case '\f': salida << "\\f"; break;
            case '\n': salida << "\\n"; break;
            case '\r': salida << "\\r"; break;
            case '\t': salida << "\\t"; break;
            default:
                if (caracter < 0x20) {
                    const char* digitos = "0123456789abcdef";
                    salida << "\\u00"
                           << digitos[(caracter >> 4) & 0x0F]
                           << digitos[caracter & 0x0F];
                } else {
                    salida << static_cast<char>(caracter);
                }
        }
    }
    return salida.str();
}

std::string jsonTexto(const std::string& texto) {
    return "\"" + escaparJson(texto) + "\"";
}

std::string tipoParaFrontend(const std::string& tipo) {
    return tipo == "Fan Club" ? "FanClub" : tipo;
}

std::string unir(const std::vector<std::string>& valores, const std::string& separador) {
    std::ostringstream salida;
    for (std::size_t i = 0; i < valores.size(); i++) {
        if (i > 0) {
            salida << separador;
        }
        salida << valores[i];
    }
    return salida.str();
}

std::string valorCampo(
    const PerfilDTO& perfil,
    const std::string& nombre,
    const std::string& alternativa = ""
) {
    for (const CampoDetallePerfilDTO& campo : perfil.camposEspecificos) {
        if (campo.nombre == nombre) {
            return campo.valor;
        }
    }
    return alternativa;
}

std::string detallePrincipal(const PerfilDTO& perfil) {
    if (perfil.tipo == "Oyente") {
        return valorCampo(perfil, "Plataforma preferida", "Sin plataforma");
    }
    if (perfil.tipo == "Artista") {
        return valorCampo(perfil, "Instrumento principal", "Sin instrumento");
    }
    if (perfil.tipo == "Productor") {
        return valorCampo(perfil, "Software principal", "Sin software");
    }
    if (perfil.tipo == "Fan Club") {
        return valorCampo(perfil, "Artista representado", "Sin artista");
    }
    return "Sin detalle";
}

std::string jsonCamposEspecificos(const PerfilDTO& perfil) {
    std::ostringstream salida;
    salida << '[';
    for (std::size_t i = 0; i < perfil.camposEspecificos.size(); i++) {
        if (i > 0) salida << ',';
        salida << "{\"nombre\":" << jsonTexto(perfil.camposEspecificos[i].nombre)
               << ",\"valor\":" << jsonTexto(perfil.camposEspecificos[i].valor)
               << '}';
    }
    salida << ']';
    return salida.str();
}

std::string jsonPerfil(const PerfilDTO& perfil) {
    const std::string secundarios = unir(perfil.generosSecundarios, ", ");
    std::ostringstream salida;
    salida << '{'
           << "\"id\":" << perfil.id << ','
           << "\"tipo\":" << jsonTexto(tipoParaFrontend(perfil.tipo)) << ','
           << "\"nombre\":" << jsonTexto(perfil.nombre) << ','
           << "\"edad\":" << perfil.edad << ','
           << "\"gustoPrincipal\":" << jsonTexto(perfil.generoPrincipal) << ','
           << "\"gustoSecundario\":" << jsonTexto(secundarios) << ','
           << "\"generosSecundarios\":";

    salida << '[';
    for (std::size_t i = 0; i < perfil.generosSecundarios.size(); i++) {
        if (i > 0) salida << ',';
        salida << jsonTexto(perfil.generosSecundarios[i]);
    }
    salida << "],";

    salida << "\"datoEspecial\":" << jsonTexto(detallePrincipal(perfil)) << ','
           << "\"camposEspecificos\":" << jsonCamposEspecificos(perfil)
           << '}';
    return salida.str();
}

std::string jsonConexion(const ConexionDTO& conexion) {
    std::ostringstream salida;
    salida << '{'
           << "\"idPrimerPerfil\":" << conexion.perfilAId << ','
           << "\"nombrePrimerPerfil\":" << jsonTexto(conexion.perfilANombre) << ','
           << "\"tipoPrimerPerfil\":" << jsonTexto(tipoParaFrontend(conexion.perfilATipo)) << ','
           << "\"idSegundoPerfil\":" << conexion.perfilBId << ','
           << "\"nombreSegundoPerfil\":" << jsonTexto(conexion.perfilBNombre) << ','
           << "\"tipoSegundoPerfil\":" << jsonTexto(tipoParaFrontend(conexion.perfilBTipo)) << ','
           << "\"afinidad\":" << conexion.afinidad
           << '}';
    return salida.str();
}

RespuestaHttp respuestaJson(
    int estado,
    bool correcto,
    const std::string& mensaje,
    const std::string& datosAdicionales = ""
) {
    std::ostringstream cuerpo;
    cuerpo << "{\"ok\":" << (correcto ? "true" : "false")
           << ",\"mensaje\":" << jsonTexto(mensaje);
    if (!datosAdicionales.empty()) {
        cuerpo << ',' << datosAdicionales;
    }
    cuerpo << '}';

    RespuestaHttp respuesta;
    respuesta.estado = estado;
    respuesta.cuerpo = cuerpo.str();
    return respuesta;
}

std::string decodificarUrl(const std::string& texto) {
    std::string resultado;
    resultado.reserve(texto.size());

    for (std::size_t i = 0; i < texto.size(); i++) {
        if (texto[i] == '+') {
            resultado.push_back(' ');
        } else if (texto[i] == '%' && i + 2 < texto.size()) {
            auto valorHexadecimal = [](char caracter) -> int {
                if (caracter >= '0' && caracter <= '9') return caracter - '0';
                if (caracter >= 'a' && caracter <= 'f') return 10 + caracter - 'a';
                if (caracter >= 'A' && caracter <= 'F') return 10 + caracter - 'A';
                return -1;
            };

            const int alto = valorHexadecimal(texto[i + 1]);
            const int bajo = valorHexadecimal(texto[i + 2]);
            if (alto >= 0 && bajo >= 0) {
                resultado.push_back(static_cast<char>((alto << 4) | bajo));
                i += 2;
            } else {
                resultado.push_back(texto[i]);
            }
        } else {
            resultado.push_back(texto[i]);
        }
    }

    return resultado;
}

std::map<std::string, std::string> analizarParametros(const std::string& texto) {
    std::map<std::string, std::string> parametros;
    std::size_t inicio = 0;

    while (inicio <= texto.size()) {
        const std::size_t ampersand = texto.find('&', inicio);
        const std::string parte = texto.substr(
            inicio,
            ampersand == std::string::npos ? std::string::npos : ampersand - inicio
        );
        const std::size_t igual = parte.find('=');
        const std::string clave = decodificarUrl(parte.substr(0, igual));
        const std::string valor = igual == std::string::npos
            ? ""
            : decodificarUrl(parte.substr(igual + 1));

        if (!clave.empty()) {
            parametros[clave] = valor;
        }

        if (ampersand == std::string::npos) break;
        inicio = ampersand + 1;
    }

    return parametros;
}

bool convertirEntero(const std::string& texto, int& valor) {
    try {
        std::size_t usados = 0;
        const int convertido = std::stoi(texto, &usados);
        if (usados != texto.size()) return false;
        valor = convertido;
        return true;
    } catch (...) {
        return false;
    }
}

bool convertirBooleano(const std::string& texto) {
    const std::string valor = minusculas(recortar(texto));
    return valor == "1" || valor == "true" || valor == "si" || valor == "sí"
        || valor == "on" || valor == "yes";
}

std::vector<std::string> dividirGeneros(const std::string& texto) {
    std::vector<std::string> generos;
    std::size_t inicio = 0;

    while (inicio <= texto.size()) {
        const std::size_t coma = texto.find(',', inicio);
        const std::string genero = recortar(texto.substr(
            inicio,
            coma == std::string::npos ? std::string::npos : coma - inicio
        ));
        if (!genero.empty()) {
            generos.push_back(genero);
        }
        if (coma == std::string::npos) break;
        inicio = coma + 1;
    }

    return generos;
}

std::string textoEstado(int estado) {
    switch (estado) {
        case 200: return "OK";
        case 201: return "Created";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        default: return "OK";
    }
}

bool enviarTodo(SocketServidor socket, const std::string& contenido) {
    std::size_t enviados = 0;
    while (enviados < contenido.size()) {
#ifdef _WIN32
        const int cantidad = send(
            socket,
            contenido.data() + enviados,
            static_cast<int>(contenido.size() - enviados),
            0
        );
#else
        const ssize_t cantidad = send(
            socket,
            contenido.data() + enviados,
            contenido.size() - enviados,
            MSG_NOSIGNAL
        );
#endif
        if (cantidad <= 0) return false;
        enviados += static_cast<std::size_t>(cantidad);
    }
    return true;
}

void enviarRespuesta(SocketServidor socket, const RespuestaHttp& respuesta) {
    std::ostringstream cabeceras;
    cabeceras << "HTTP/1.1 " << respuesta.estado << ' '
              << textoEstado(respuesta.estado) << "\r\n"
              << "Content-Type: " << respuesta.tipoContenido << "\r\n"
              << "Content-Length: " << respuesta.cuerpo.size() << "\r\n"
              << "Cache-Control: no-store\r\n"
              << "X-Content-Type-Options: nosniff\r\n"
              << "Connection: close\r\n\r\n";

    enviarTodo(socket, cabeceras.str() + respuesta.cuerpo);
}

bool recibirSolicitud(SocketServidor socket, SolicitudHttp& solicitud) {
    std::string datos;
    char buffer[4096];
    std::size_t finalCabeceras = std::string::npos;

    while (finalCabeceras == std::string::npos && datos.size() < 1024 * 1024) {
#ifdef _WIN32
        const int recibidos = recv(socket, buffer, static_cast<int>(sizeof(buffer)), 0);
#else
        const ssize_t recibidos = recv(socket, buffer, sizeof(buffer), 0);
#endif
        if (recibidos <= 0) return false;
        datos.append(buffer, static_cast<std::size_t>(recibidos));
        finalCabeceras = datos.find("\r\n\r\n");
    }

    if (finalCabeceras == std::string::npos) return false;

    const std::string bloqueCabeceras = datos.substr(0, finalCabeceras);
    std::istringstream flujo(bloqueCabeceras);
    std::string lineaInicial;
    if (!std::getline(flujo, lineaInicial)) return false;
    if (!lineaInicial.empty() && lineaInicial.back() == '\r') lineaInicial.pop_back();

    std::istringstream inicial(lineaInicial);
    std::string objetivo;
    std::string version;
    if (!(inicial >> solicitud.metodo >> objetivo >> version)) return false;

    const std::size_t interrogacion = objetivo.find('?');
    solicitud.ruta = decodificarUrl(objetivo.substr(0, interrogacion));
    solicitud.consulta = interrogacion == std::string::npos
        ? ""
        : objetivo.substr(interrogacion + 1);

    std::string linea;
    while (std::getline(flujo, linea)) {
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        const std::size_t dosPuntos = linea.find(':');
        if (dosPuntos == std::string::npos) continue;
        solicitud.cabeceras[minusculas(recortar(linea.substr(0, dosPuntos)))] =
            recortar(linea.substr(dosPuntos + 1));
    }

    std::size_t longitudCuerpo = 0;
    const auto encontrado = solicitud.cabeceras.find("content-length");
    if (encontrado != solicitud.cabeceras.end()) {
        try {
            longitudCuerpo = static_cast<std::size_t>(std::stoul(encontrado->second));
        } catch (...) {
            return false;
        }
    }

    const std::size_t inicioCuerpo = finalCabeceras + 4;
    while (datos.size() - inicioCuerpo < longitudCuerpo && datos.size() < 2 * 1024 * 1024) {
#ifdef _WIN32
        const int recibidos = recv(socket, buffer, static_cast<int>(sizeof(buffer)), 0);
#else
        const ssize_t recibidos = recv(socket, buffer, sizeof(buffer), 0);
#endif
        if (recibidos <= 0) return false;
        datos.append(buffer, static_cast<std::size_t>(recibidos));
    }

    if (datos.size() - inicioCuerpo < longitudCuerpo) return false;
    solicitud.cuerpo = datos.substr(inicioCuerpo, longitudCuerpo);
    return true;
}

std::string tipoMime(const std::filesystem::path& ruta) {
    const std::string extension = minusculas(ruta.extension().string());
    if (extension == ".html") return "text/html; charset=utf-8";
    if (extension == ".css") return "text/css; charset=utf-8";
    if (extension == ".js") return "application/javascript; charset=utf-8";
    if (extension == ".svg") return "image/svg+xml";
    if (extension == ".png") return "image/png";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    return "application/octet-stream";
}

RespuestaHttp servirArchivo(
    const std::string& carpetaFrontend,
    const std::string& rutaSolicitada
) {
    std::string archivo = rutaSolicitada;
    if (archivo == "/") archivo = "/index.html";

    if (archivo.find("..") != std::string::npos) {
        return respuestaJson(404, false, "Archivo no encontrado.");
    }

    const std::filesystem::path ruta =
        std::filesystem::path(carpetaFrontend) / archivo.substr(1);
    std::ifstream entrada(ruta, std::ios::binary);
    if (!entrada) {
        return respuestaJson(404, false, "Archivo no encontrado.");
    }

    std::ostringstream contenido;
    contenido << entrada.rdbuf();

    RespuestaHttp respuesta;
    respuesta.tipoContenido = tipoMime(ruta);
    respuesta.cuerpo = contenido.str();
    return respuesta;
}

void abrirEnNavegador(int puerto) {
    const std::string url = "http://127.0.0.1:" + std::to_string(puerto);
#ifdef _WIN32
    const std::string comando = "start \"\" \"" + url + "\"";
#elif __APPLE__
    const std::string comando = "open \"" + url + "\"";
#else
    const std::string comando = "xdg-open \"" + url + "\" >/dev/null 2>&1 &";
#endif
    std::system(comando.c_str());
}

RespuestaHttp crearEstado(ControladorAplicacion& controlador) {
    const std::vector<PerfilDTO> perfiles = controlador.obtenerPerfiles();
    const std::vector<ConexionDTO> conexiones = controlador.obtenerConexiones();

    int oyentes = 0;
    int artistas = 0;
    int productores = 0;
    int fanClub = 0;

    for (const PerfilDTO& perfil : perfiles) {
        if (perfil.tipo == "Oyente") oyentes++;
        else if (perfil.tipo == "Artista") artistas++;
        else if (perfil.tipo == "Productor") productores++;
        else if (perfil.tipo == "Fan Club") fanClub++;
    }

    std::ostringstream datos;
    datos << "\"datos\":{\"perfiles\":[";
    for (std::size_t i = 0; i < perfiles.size(); i++) {
        if (i > 0) datos << ',';
        datos << jsonPerfil(perfiles[i]);
    }

    datos << "],\"conexiones\":[";
    for (std::size_t i = 0; i < conexiones.size(); i++) {
        if (i > 0) datos << ',';
        datos << jsonConexion(conexiones[i]);
    }

    datos << "],\"estadisticas\":{"
          << "\"perfiles\":" << perfiles.size() << ','
          << "\"conexiones\":" << conexiones.size() << ','
          << "\"oyentes\":" << oyentes << ','
          << "\"artistas\":" << artistas << ','
          << "\"productores\":" << productores << ','
          << "\"fanClub\":" << fanClub
          << "}}";

    return respuestaJson(200, true, "Estado obtenido.", datos.str());
}

RespuestaHttp crearAfinidades(
    ControladorAplicacion& controlador,
    const std::map<std::string, std::string>& parametros
) {
    const auto encontrado = parametros.find("id");
    int id = 0;
    if (encontrado == parametros.end() || !convertirEntero(encontrado->second, id)) {
        return respuestaJson(400, false, "El ID del perfil no es valido.");
    }

    const ResultadoConsultaPerfil base = controlador.buscarPerfil(id);
    if (!base.resultado.correcto) {
        return respuestaJson(404, false, base.resultado.mensaje);
    }

    const ResultadoCompatibles compatibles = controlador.buscarPerfilesCompatibles(id);
    if (!compatibles.resultado.correcto) {
        return respuestaJson(400, false, compatibles.resultado.mensaje);
    }

    std::ostringstream datos;
    datos << "\"datos\":{\"perfilBase\":{"
          << "\"id\":" << base.perfil.id << ','
          << "\"nombre\":" << jsonTexto(base.perfil.nombre)
          << "},\"coincidencias\":[";

    for (std::size_t i = 0; i < compatibles.perfiles.size(); i++) {
        if (i > 0) datos << ',';
        const PerfilCompatibleDTO& compatible = compatibles.perfiles[i];
        const ResultadoConsultaPerfil detalle = controlador.buscarPerfil(compatible.id);
        const std::string genero = detalle.resultado.correcto
            ? detalle.perfil.generoPrincipal
            : "";

        datos << '{'
              << "\"id\":" << compatible.id << ','
              << "\"nombre\":" << jsonTexto(compatible.nombre) << ','
              << "\"tipo\":" << jsonTexto(tipoParaFrontend(compatible.tipo)) << ','
              << "\"afinidad\":" << compatible.afinidad << ','
              << "\"gustoPrincipal\":" << jsonTexto(genero)
              << '}';
    }

    datos << "]}";
    return respuestaJson(200, true, compatibles.resultado.mensaje, datos.str());
}

RespuestaHttp crearPerfil(
    ControladorAplicacion& controlador,
    const std::string& rutaDatos,
    const std::map<std::string, std::string>& parametros
) {
    const auto obtener = [&parametros](const std::string& clave) -> std::string {
        const auto encontrado = parametros.find(clave);
        return encontrado == parametros.end() ? "" : encontrado->second;
    };

    const std::string tipo = obtener("tipo");
    const std::string nombre = obtener("nombre");
    const std::string generoPrincipal = obtener("gustoPrincipal");
    std::string textoSecundarios = obtener("generosSecundarios");
    if (textoSecundarios.empty()) textoSecundarios = obtener("gustoSecundario");

    int edad = 0;
    if (!convertirEntero(obtener("edad"), edad)) {
        return respuestaJson(400, false, "La edad no es valida.");
    }

    const std::vector<std::string> secundarios = dividirGeneros(textoSecundarios);
    ResultadoCreacionPerfil resultado{};

    if (tipo == "Oyente") {
        int horas = 0;
        if (!convertirEntero(obtener("horasEscuchaSemanales"), horas)) {
            return respuestaJson(400, false, "Las horas de escucha no son validas.");
        }
        resultado = controlador.crearPerfilOyente(
            nombre,
            edad,
            generoPrincipal,
            secundarios,
            horas,
            obtener("plataformaPreferida")
        );
    } else if (tipo == "Artista") {
        resultado = controlador.crearPerfilArtista(
            nombre,
            edad,
            generoPrincipal,
            secundarios,
            obtener("nombreArtistico"),
            obtener("instrumentoPrincipal"),
            convertirBooleano(obtener("buscaColaboraciones"))
        );
    } else if (tipo == "Productor") {
        int anios = 0;
        if (!convertirEntero(obtener("aniosExperiencia"), anios)) {
            return respuestaJson(400, false, "Los anios de experiencia no son validos.");
        }
        resultado = controlador.crearPerfilProductor(
            nombre,
            edad,
            generoPrincipal,
            secundarios,
            obtener("softwarePrincipal"),
            anios,
            convertirBooleano(obtener("ofreceServicios"))
        );
    } else if (tipo == "FanClub") {
        int miembros = 0;
        if (!convertirEntero(obtener("numeroMiembros"), miembros)) {
            return respuestaJson(400, false, "El numero de miembros no es valido.");
        }
        resultado = controlador.crearPerfilFanClub(
            nombre,
            edad,
            generoPrincipal,
            secundarios,
            obtener("artistaRepresentado"),
            miembros,
            obtener("ciudadBase")
        );
    } else {
        return respuestaJson(400, false, "El tipo de perfil no es valido.");
    }

    if (!resultado.resultado.correcto) {
        return respuestaJson(400, false, resultado.resultado.mensaje);
    }

    const ResultadoOperacion guardado = controlador.guardarDatos(rutaDatos);
    if (!guardado.correcto) {
        return respuestaJson(500, false, guardado.mensaje);
    }

    return respuestaJson(
        201,
        true,
        resultado.resultado.mensaje,
        "\"idPerfil\":" + std::to_string(resultado.idPerfil)
    );
}

RespuestaHttp procesarApi(
    ControladorAplicacion& controlador,
    const std::string& rutaDatos,
    bool& ejecutando,
    const SolicitudHttp& solicitud
) {
    if (solicitud.metodo == "GET" && solicitud.ruta == "/api/estado") {
        return crearEstado(controlador);
    }

    if (solicitud.metodo == "GET" && solicitud.ruta == "/api/afinidades") {
        return crearAfinidades(controlador, analizarParametros(solicitud.consulta));
    }

    if (solicitud.metodo == "POST" && solicitud.ruta == "/api/perfiles") {
        return crearPerfil(
            controlador,
            rutaDatos,
            analizarParametros(solicitud.cuerpo)
        );
    }

    const std::string prefijoPerfil = "/api/perfiles/";
    if (solicitud.metodo == "DELETE"
        && solicitud.ruta.rfind(prefijoPerfil, 0) == 0) {
        int id = 0;
        if (!convertirEntero(solicitud.ruta.substr(prefijoPerfil.size()), id)) {
            return respuestaJson(400, false, "El ID del perfil no es valido.");
        }

        const ResultadoOperacion resultado = controlador.eliminarPerfil(id);
        if (!resultado.correcto) {
            return respuestaJson(400, false, resultado.mensaje);
        }

        const ResultadoOperacion guardado = controlador.guardarDatos(rutaDatos);
        if (!guardado.correcto) {
            return respuestaJson(500, false, guardado.mensaje);
        }
        return respuestaJson(200, true, resultado.mensaje);
    }

    if (solicitud.metodo == "POST" && solicitud.ruta == "/api/conexiones") {
        const std::map<std::string, std::string> parametros =
            analizarParametros(solicitud.cuerpo);
        int primero = 0;
        int segundo = 0;
        const auto a = parametros.find("idPrimerPerfil");
        const auto b = parametros.find("idSegundoPerfil");
        if (a == parametros.end() || b == parametros.end()
            || !convertirEntero(a->second, primero)
            || !convertirEntero(b->second, segundo)) {
            return respuestaJson(400, false, "Los IDs de la conexion no son validos.");
        }

        const ResultadoConexionAplicacion resultado =
            controlador.crearConexion(primero, segundo);
        if (!resultado.resultado.correcto) {
            return respuestaJson(400, false, resultado.resultado.mensaje);
        }

        const ResultadoOperacion guardado = controlador.guardarDatos(rutaDatos);
        if (!guardado.correcto) {
            return respuestaJson(500, false, guardado.mensaje);
        }

        return respuestaJson(
            201,
            true,
            resultado.resultado.mensaje,
            "\"afinidad\":" + std::to_string(resultado.afinidad)
        );
    }

    if (solicitud.metodo == "POST" && solicitud.ruta == "/api/guardar") {
        const ResultadoOperacion resultado = controlador.guardarDatos(rutaDatos);
        return respuestaJson(
            resultado.correcto ? 200 : 500,
            resultado.correcto,
            resultado.mensaje
        );
    }

    if (solicitud.metodo == "POST" && solicitud.ruta == "/api/cargar") {
        const ResultadoOperacion resultado = controlador.cargarDatos(rutaDatos);
        return respuestaJson(
            resultado.correcto ? 200 : 400,
            resultado.correcto,
            resultado.mensaje
        );
    }

    if (solicitud.metodo == "POST" && solicitud.ruta == "/api/cerrar") {
        const ResultadoOperacion resultado = controlador.guardarDatos(rutaDatos);
        if (!resultado.correcto) {
            return respuestaJson(500, false, resultado.mensaje);
        }
        ejecutando = false;
        return respuestaJson(
            200,
            true,
            "Datos guardados. SoundBridge se cerrara correctamente."
        );
    }

    return respuestaJson(404, false, "Ruta de la API no encontrada.");
}

}

ServidorWeb::ServidorWeb(
    ControladorAplicacion& controlador,
    std::string carpetaFrontend,
    std::string rutaDatos,
    int puerto
)
    : controlador_(controlador),
      carpetaFrontend_(std::move(carpetaFrontend)),
      rutaDatos_(std::move(rutaDatos)),
      puerto_(puerto),
      ejecutando_(false) {
}

int ServidorWeb::ejecutar(bool abrirNavegador) {
    if (!std::filesystem::exists(
            std::filesystem::path(carpetaFrontend_) / "index.html"
        )) {
        std::cerr << "No se encontro frontend/index.html en: "
                  << carpetaFrontend_ << "\n";
        return 1;
    }

#ifdef _WIN32
    WSADATA datosWinsock;
    if (WSAStartup(MAKEWORD(2, 2), &datosWinsock) != 0) {
        std::cerr << "No se pudo iniciar Winsock.\n";
        return 1;
    }
#endif

    const SocketServidor servidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servidor == SOCKET_INVALIDO) {
        std::cerr << "No se pudo crear el socket del servidor.\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    int reutilizar = 1;
#ifdef _WIN32
    setsockopt(
        servidor,
        SOL_SOCKET,
        SO_REUSEADDR,
        reinterpret_cast<const char*>(&reutilizar),
        sizeof(reutilizar)
    );
#else
    setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &reutilizar, sizeof(reutilizar));
#endif

    sockaddr_in direccion{};
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(static_cast<unsigned short>(puerto_));
    direccion.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(
            servidor,
            reinterpret_cast<sockaddr*>(&direccion),
            sizeof(direccion)
        ) < 0) {
        std::cerr << "No se pudo usar el puerto " << puerto_
                  << ". Prueba con --puerto 8081.\n";
        cerrarSocket(servidor);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (listen(servidor, 10) < 0) {
        std::cerr << "No se pudo iniciar la escucha del servidor.\n";
        cerrarSocket(servidor);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    ejecutando_ = true;
    std::cout << "Backend y frontend conectados.\n";
    std::cout << "Abre http://127.0.0.1:" << puerto_ << "\n";
    std::cout << "Presiona Ctrl+C o usa el boton Cerrar SoundBridge.\n";

    if (abrirNavegador) {
        abrirEnNavegador(puerto_);
    }

    while (ejecutando_) {
        sockaddr_in clienteDireccion{};
#ifdef _WIN32
        int longitud = sizeof(clienteDireccion);
#else
        socklen_t longitud = sizeof(clienteDireccion);
#endif
        const SocketServidor cliente = accept(
            servidor,
            reinterpret_cast<sockaddr*>(&clienteDireccion),
            &longitud
        );

        if (cliente == SOCKET_INVALIDO) {
            if (ejecutando_) {
                std::cerr << "No se pudo aceptar una conexion.\n";
            }
            continue;
        }

        SolicitudHttp solicitud;
        if (!recibirSolicitud(cliente, solicitud)) {
            enviarRespuesta(
                cliente,
                respuestaJson(400, false, "Solicitud HTTP invalida.")
            );
            cerrarSocket(cliente);
            continue;
        }

        try {
            RespuestaHttp respuesta;
            if (solicitud.ruta.rfind("/api/", 0) == 0) {
                respuesta = procesarApi(
                    controlador_,
                    rutaDatos_,
                    ejecutando_,
                    solicitud
                );
            } else if (solicitud.metodo == "GET") {
                respuesta = servirArchivo(carpetaFrontend_, solicitud.ruta);
            } else {
                respuesta = respuestaJson(
                    405,
                    false,
                    "Metodo HTTP no permitido para esta ruta."
                );
            }
            enviarRespuesta(cliente, respuesta);
        } catch (const std::exception& error) {
            enviarRespuesta(
                cliente,
                respuestaJson(500, false, error.what())
            );
        }

        cerrarSocket(cliente);
    }

    cerrarSocket(servidor);
#ifdef _WIN32
    WSACleanup();
#endif
    std::cout << "Servidor SoundBridge cerrado.\n";
    return 0;
}

}
