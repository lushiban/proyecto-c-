#include "soundbridge/utils/TextoUtils.hpp"

#include <cctype>

namespace soundbridge {

std::string recortarTexto(const std::string& texto) {
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

std::string normalizarTexto(const std::string& texto) {
    std::string resultado = recortarTexto(texto);

    for (char& caracter : resultado) {
        caracter = static_cast<char>(
            std::tolower(static_cast<unsigned char>(caracter))
        );
    }

    return resultado;
}

bool textoEstaVacio(const std::string& texto) {
    return recortarTexto(texto).empty();
}

bool contieneTexto(
    const std::vector<std::string>& textos,
    const std::string& textoBuscado
) {
    std::string textoNormalizado = normalizarTexto(textoBuscado);

    for (const std::string& texto : textos) {
        if (normalizarTexto(texto) == textoNormalizado) {
            return true;
        }
    }

    return false;
}

bool tieneTextosDuplicados(const std::vector<std::string>& textos) {
    for (std::size_t i = 0; i < textos.size(); i++) {
        for (std::size_t j = i + 1; j < textos.size(); j++) {
            if (normalizarTexto(textos[i]) == normalizarTexto(textos[j])) {
                return true;
            }
        }
    }

    return false;
}

}
