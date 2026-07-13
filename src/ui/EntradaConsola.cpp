#include "rsm/ui/EntradaConsola.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

#include "rsm/utils/TextoUtils.hpp"

namespace rsm {
namespace {

bool convertirEntero(const std::string& texto, int& valor) {
    std::istringstream flujo(texto);

    if (!(flujo >> valor)) {
        return false;
    }

    flujo >> std::ws;
    return flujo.eof();
}

}

std::string EntradaConsola::leerTexto(const std::string& mensaje) {
    std::cout << mensaje;

    std::string texto;

    if (!std::getline(std::cin, texto)) {
        throw std::runtime_error("La entrada fue finalizada.");
    }

    return texto;
}

std::string EntradaConsola::leerTextoNoVacio(const std::string& mensaje) {
    while (true) {
        std::string texto = leerTexto(mensaje);

        if (!textoEstaVacio(texto)) {
            return recortarTexto(texto);
        }

        std::cout << "El texto no puede estar vacio." << std::endl;
    }
}

int EntradaConsola::leerEnteroEnRango(
    const std::string& mensaje,
    int minimo,
    int maximo
) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        int valor = 0;

        if (!convertirEntero(texto, valor)) {
            std::cout << "Debe ingresar un numero entero valido." << std::endl;
            continue;
        }

        if (valor < minimo || valor > maximo) {
            std::cout << "El valor debe estar entre "
                      << minimo << " y " << maximo << "." << std::endl;
            continue;
        }

        return valor;
    }
}

int EntradaConsola::leerEnteroMinimo(
    const std::string& mensaje,
    int minimo
) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        int valor = 0;

        if (!convertirEntero(texto, valor)) {
            std::cout << "Debe ingresar un numero entero valido." << std::endl;
            continue;
        }

        if (valor < minimo) {
            std::cout << "El valor debe ser igual o mayor a "
                      << minimo << "." << std::endl;
            continue;
        }

        return valor;
    }
}

bool EntradaConsola::leerConfirmacion(const std::string& mensaje) {
    while (true) {
        std::string respuesta = normalizarTexto(leerTexto(mensaje));

        if (respuesta == "s" || respuesta == "si") {
            return true;
        }

        if (respuesta == "n" || respuesta == "no") {
            return false;
        }

        std::cout << "Ingrese s para si o n para no." << std::endl;
    }
}

std::vector<std::string> EntradaConsola::leerGenerosSecundarios(
    const std::string& generoPrincipal
) {
    int cantidad = leerEnteroMinimo(
        "Cantidad de generos secundarios: ",
        1
    );

    std::vector<std::string> generos;

    for (int i = 0; i < cantidad; i++) {
        bool agregado = false;

        while (!agregado) {
            std::string mensaje = "Genero secundario "
                                  + std::to_string(i + 1)
                                  + ": ";
            std::string genero = leerTextoNoVacio(mensaje);

            if (normalizarTexto(genero)
                == normalizarTexto(generoPrincipal)) {
                std::cout << "El genero principal no puede repetirse "
                          << "como secundario." << std::endl;
            } else if (contieneTexto(generos, genero)) {
                std::cout << "El genero secundario ya fue ingresado."
                          << std::endl;
            } else {
                generos.push_back(genero);
                agregado = true;
            }
        }
    }

    return generos;
}

}
