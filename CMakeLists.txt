cmake_minimum_required(VERSION 3.16)

project(SoundBridge VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(soundbridge
    main.cpp
    src/AplicacionSoundBridge.cpp
    src/Conexion.cpp
    src/FabricaPerfiles.cpp
    src/Perfil.cpp
    src/PerfilArtista.cpp
    src/PerfilFanClub.cpp
    src/PerfilOyente.cpp
    src/PerfilProductor.cpp
    src/RedSocialMusical.cpp
    src/ServidorWeb.cpp
    src/Utilidades.cpp
)

target_include_directories(soundbridge PRIVATE include)

if(WIN32)
    # Winsock es parte de Windows y permite ejecutar el servidor HTTP local.
    target_link_libraries(soundbridge PRIVATE ws2_32)
endif()

if(MSVC)
    target_compile_options(soundbridge PRIVATE /W4 /permissive-)
else()
    target_compile_options(soundbridge PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copia los archivos visuales junto al ejecutable para que funcionen desde build/.
add_custom_command(TARGET soundbridge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/frontend"
            "$<TARGET_FILE_DIR:soundbridge>/frontend"
    COMMENT "Copiando frontend de SoundBridge"
)
