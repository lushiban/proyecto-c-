cmake_minimum_required(VERSION 3.20)

project(
    SoundBridge
    VERSION 1.0.0
    LANGUAGES CXX
)

option(
    SOUNDBRIDGE_BUILD_TESTS
    "Construir pruebas automatizadas"
    ON
)

option(
    SOUNDBRIDGE_ENABLE_GRAPHICS
    "Activar soporte grafico externo"
    OFF
)

option(
    SOUNDBRIDGE_ENABLE_SANITIZERS
    "Activar sanitizadores"
    OFF
)

set(SOUNDBRIDGE_GRAPHICS_ENABLED_VALUE 0)

if(SOUNDBRIDGE_ENABLE_GRAPHICS)
    include(cmake/GraphicsVendor.cmake)
    soundbridge_preparar_integracion_grafica()
    set(SOUNDBRIDGE_GRAPHICS_ENABLED_VALUE 1)
endif()

add_library(
    soundbridge_core
    src/domain/TipoPerfil.cpp
    src/domain/Perfil.cpp
    src/domain/PerfilOyente.cpp
    src/domain/PerfilArtista.cpp
    src/domain/PerfilProductor.cpp
    src/domain/PerfilFanClub.cpp
    src/domain/Conexion.cpp
    src/application/SoundBridge.cpp
    src/application/ServicioEstadisticas.cpp
    src/application/ControladorAplicacion.cpp
    src/infrastructure/PersistenciaTexto.cpp
    src/utils/TextoUtils.cpp
)

target_include_directories(
    soundbridge_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(soundbridge_core PUBLIC cxx_std_17)

set_target_properties(
    soundbridge_core
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

add_library(
    soundbridge_console
    src/visualization/VisualizadorNoDisponible.cpp
    src/ui/EntradaConsola.cpp
    src/ui/MenuConsola.cpp
)

target_link_libraries(soundbridge_console PUBLIC soundbridge_core)
target_compile_features(soundbridge_console PUBLIC cxx_std_17)

set_target_properties(
    soundbridge_console
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

add_executable(
    soundbridge
    src/main.cpp
)

target_link_libraries(
    soundbridge
    PRIVATE
        soundbridge_console
)

target_compile_features(soundbridge PRIVATE cxx_std_17)

set_target_properties(
    soundbridge
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_compile_definitions(
    soundbridge
    PRIVATE
        SOUNDBRIDGE_ENABLE_GRAPHICS=${SOUNDBRIDGE_GRAPHICS_ENABLED_VALUE}
        SOUNDBRIDGE_VERSION_PROYECTO="${PROJECT_VERSION}"
)

if(MSVC)
    target_compile_options(soundbridge_core PRIVATE /W4)
    target_compile_options(soundbridge_console PRIVATE /W4)
    target_compile_options(soundbridge PRIVATE /W4)
else()
    target_compile_options(soundbridge_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(soundbridge_console PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(soundbridge PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(SOUNDBRIDGE_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
            soundbridge_core
            PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )
        target_compile_options(
            soundbridge_console
            PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )
        target_compile_options(
            soundbridge
            PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )
        target_link_options(
            soundbridge
            PRIVATE
                -fsanitize=address,undefined
        )
    else()
        message(WARNING
            "Los sanitizadores configurados solo estan disponibles con GCC o Clang."
        )
    endif()
endif()

if(SOUNDBRIDGE_BUILD_TESTS)
    enable_testing()

    add_executable(
        soundbridge_tests
        tests/PruebasFase10.cpp
    )

    target_link_libraries(soundbridge_tests PRIVATE soundbridge_core)
    target_compile_features(soundbridge_tests PRIVATE cxx_std_17)

    set_target_properties(
        soundbridge_tests
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    if(MSVC)
        target_compile_options(soundbridge_tests PRIVATE /W4)
    else()
        target_compile_options(soundbridge_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    if(SOUNDBRIDGE_ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
            soundbridge_tests
            PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )
        target_link_options(
            soundbridge_tests
            PRIVATE
                -fsanitize=address,undefined
        )
    endif()

    add_test(NAME soundbridge_pruebas_fase10 COMMAND soundbridge_tests)
endif()
