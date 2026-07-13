#ifndef RSM_UTILS_TEXTO_UTILS_HPP
#define RSM_UTILS_TEXTO_UTILS_HPP

#include <string>
#include <vector>

namespace rsm {

std::string recortarTexto(const std::string& texto);
std::string normalizarTexto(const std::string& texto);
bool textoEstaVacio(const std::string& texto);
bool contieneTexto(
    const std::vector<std::string>& textos,
    const std::string& textoBuscado
);
bool tieneTextosDuplicados(const std::vector<std::string>& textos);

}

#endif
