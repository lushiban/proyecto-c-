# Pruebas finales

## Entorno utilizado

- Fecha de cierre: 13 de julio de 2026.
- Estándar: C++17.
- CMake: configuración compatible con 3.20 o superior.
- GCC: 14.2.0.
- Clang: 17.0.0.
- Advertencias: `-Wall -Wextra -Wpedantic -Werror`.

## Resultados automatizados

| Prueba | Resultado esperado | Resultado obtenido | Estado |
| --- | --- | --- | --- |
| Fachada autónoma | Crear solo `ControladorAplicacion` | Se creó y administró el estado interno | Aprobada |
| Modelos para interfaz | DTO completos de cuatro tipos | Datos comunes y específicos correctos | Aprobada |
| Validaciones | Rechazar entradas inválidas sin cambiar estado | Todos los casos fueron rechazados | Aprobada |
| Afinidad | Calcular porcentaje y errores | Resultado de 65 y errores interpretables | Aprobada |
| Conexiones | Crear, impedir duplicados y eliminar | Comportamiento correcto | Aprobada |
| Compatibilidad | Filtrar y ordenar | Orden por afinidad e ID correcto | Aprobada |
| Eliminación en cascada | Borrar conexiones del perfil | Cantidades actualizadas | Aprobada |
| Estadísticas | DTO y categorías obligatorias | Datos correctos en red vacía y poblada | Aprobada |
| Persistencia | Guardar y cargar estado | Perfiles, conexión e ID recuperados | Aprobada |
| Caracteres escapados | Conservar `\\`, `|`, `;` y saltos | Textos recuperados sin cambios | Aprobada |
| Nueva ejecución | Cargar cambios guardados por otra instancia | Tres perfiles recuperados | Aprobada |
| Archivo inválido | No reemplazar estado | Perfil original conservado | Aprobada |
| Archivo inexistente | Iniciar red vacía | Resultado correcto y mensaje claro | Aprobada |

El ejecutable de pruebas agrupa estos casos en nueve bloques:

```text
Resultado: 9 de 9 pruebas aprobadas.
```

## Compilación del núcleo sin presentación

Comando:

```bash
cmake -S . -B build-core \
  -DSOUNDBRIDGE_ENABLE_GRAPHICS=OFF \
  -DSOUNDBRIDGE_BUILD_TESTS=OFF \
  -DCMAKE_CXX_FLAGS="-Werror"
cmake --build build-core --target soundbridge_core --parallel
```

Resultado: `soundbridge_core` compiló correctamente sin `soundbridge_console` ni front-end.

## GCC estricto

```bash
cmake -S . -B build-final \
  -DSOUNDBRIDGE_ENABLE_GRAPHICS=OFF \
  -DSOUNDBRIDGE_BUILD_TESTS=ON \
  -DCMAKE_CXX_FLAGS="-Werror"
cmake --build build-final --parallel
ctest --test-dir build-final --output-on-failure
```

Resultado: 100 % de pruebas aprobadas, sin advertencias.

## Clang estricto

Resultado: 100 % de pruebas aprobadas con Clang 17.0.0 y `-Werror`.

## Sanitizadores

Se ejecutaron AddressSanitizer, LeakSanitizer y UndefinedBehaviorSanitizer.

Resultado: 100 % de pruebas aprobadas, sin reportes de fuga, acceso inválido ni
comportamiento indefinido.

## Consola temporal

Entrada utilizada:

```text
0
```

Resultado:

- Inicio correcto.
- Mensaje de archivo inexistente.
- Menú mostrado.
- Salida normal.
- Código de salida 0.

## Revisión de desacoplamiento

Se buscó `std::cin`, `std::cout`, `std::cerr` e `iostream` en dominio,
aplicación, infraestructura y utilidades. No se encontraron usos directos.

Se buscaron rutas absolutas y marcadores `TODO` o `FIXME` en código compilable.
No se encontraron.

## Soporte gráfico pendiente

Se comprobó `SOUNDBRIDGE_ENABLE_GRAPHICS=ON`. CMake se detuvo intencionalmente con un
mensaje que indica que la tecnología no está seleccionada. Esta verificación
confirma que no se enlaza una dependencia visual inventada.

## Pruebas no ejecutadas

No se ejecutó compilación con MSVC porque ese compilador no está disponible en
el entorno utilizado. La configuración conserva opciones específicas para
MSVC, pero no se marca esa compilación como aprobada.
