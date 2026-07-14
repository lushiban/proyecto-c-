cmake_minimum_required(VERSION 3.16)

project(SoundBridge VERSION 2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(soundbridge_core
    src/application/ControladorAplicacion.cpp
    src/application/ServicioEstadisticas.cpp
    src/application/SoundBridge.cpp
    src/domain/Conexion.cpp
    src/domain/Perfil.cpp
    src/domain/PerfilArtista.cpp
    src/domain/PerfilFanClub.cpp
    src/domain/PerfilOyente.cpp
    src/domain/PerfilProductor.cpp
    src/domain/TipoPerfil.cpp
    src/infrastructure/PersistenciaTexto.cpp
    src/ui/EntradaConsola.cpp
    src/ui/MenuConsola.cpp
    src/utils/TextoUtils.cpp
    src/visualization/VisualizadorNoDisponible.cpp
)

target_include_directories(soundbridge_core PUBLIC include)
target_compile_definitions(
    soundbridge_core
    PUBLIC SOUNDBRIDGE_VERSION_PROYECTO="${PROJECT_VERSION}"
)

option(SOUNDBRIDGE_BUILD_TESTS "Compilar pruebas del nucleo" ON)

add_executable(soundbridge
    main.cpp
    src/web/ServidorWeb.cpp
)

target_link_libraries(soundbridge PRIVATE soundbridge_core)

if(WIN32)
    target_link_libraries(soundbridge PRIVATE ws2_32)
endif()

if(MSVC)
    target_compile_options(soundbridge_core PRIVATE /W4 /permissive-)
    target_compile_options(soundbridge PRIVATE /W4 /permissive-)
else()
    target_compile_options(soundbridge_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(soundbridge PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(TARGET soundbridge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/frontend"
            "$<TARGET_FILE_DIR:soundbridge>/frontend"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/data"
            "$<TARGET_FILE_DIR:soundbridge>/data"
    COMMENT "Copiando frontend y datos de SoundBridge"
)

if(SOUNDBRIDGE_BUILD_TESTS)
    enable_testing()
    add_executable(soundbridge_tests tests/PruebasFase10.cpp)
    target_link_libraries(soundbridge_tests PRIVATE soundbridge_core)

    if(MSVC)
        target_compile_options(soundbridge_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(soundbridge_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME soundbridge_core_tests COMMAND soundbridge_tests)
endif()
