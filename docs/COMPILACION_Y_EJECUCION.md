# Compilación y ejecución

## Requisitos

- CMake 3.20 o superior.
- Compilador compatible con C++17.
- GCC, Clang o MSVC.

No se necesita ninguna librería gráfica.

## Configuración normal

```bash
cmake -S . -B build \
  -DSOUNDBRIDGE_ENABLE_GRAPHICS=OFF \
  -DSOUNDBRIDGE_BUILD_TESTS=ON
```

## Compilar todo

```bash
cmake --build build --parallel
```

## Compilar solo el núcleo

```bash
cmake --build build --target soundbridge_core --parallel
```

## Compilar solo las pruebas

```bash
cmake --build build --target soundbridge_tests --parallel
```

## Ejecutar la consola

```bash
./build/soundbridge
```

## Ejecutar pruebas

```bash
ctest --test-dir build --output-on-failure
```

## Advertencias como errores

```bash
cmake -S . -B build-estricto \
  -DSOUNDBRIDGE_ENABLE_GRAPHICS=OFF \
  -DSOUNDBRIDGE_BUILD_TESTS=ON \
  -DCMAKE_CXX_FLAGS="-Werror"
cmake --build build-estricto --parallel
```

El proyecto ya activa `-Wall -Wextra -Wpedantic` con GCC y Clang.

## Sanitizadores

```bash
cmake -S . -B build-sanitized \
  -DSOUNDBRIDGE_ENABLE_GRAPHICS=OFF \
  -DSOUNDBRIDGE_BUILD_TESTS=ON \
  -DSOUNDBRIDGE_ENABLE_SANITIZERS=ON \
  -DCMAKE_CXX_FLAGS="-Werror"
cmake --build build-sanitized --parallel
ctest --test-dir build-sanitized --output-on-failure
```

## Clang

```bash
cmake -S . -B build-clang \
  -DSOUNDBRIDGE_ENABLE_GRAPHICS=OFF \
  -DSOUNDBRIDGE_BUILD_TESTS=ON \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_FLAGS="-Werror"
cmake --build build-clang --parallel
ctest --test-dir build-clang --output-on-failure
```

## Windows

Con Visual Studio, el ejecutable puede quedar en:

```text
build\Debug\soundbridge.exe
```

También puede ejecutarse CTest con:

```bash
ctest --test-dir build -C Debug --output-on-failure
```

## Soporte gráfico

`SOUNDBRIDGE_ENABLE_GRAPHICS=ON` está reservado. Mientras no exista una tecnología
seleccionada, CMake se detiene intencionalmente y explica que debe compilarse con
la opción desactivada.
