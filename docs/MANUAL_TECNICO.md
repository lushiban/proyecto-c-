# Manual técnico

## 1. Arquitectura

El proyecto usa capas sencillas:

```text
Presentación: main, MenuConsola, EntradaConsola
Aplicación: ControladorAplicacion, SoundBridge, ServicioEstadisticas
Dominio: Perfil y derivados, Conexion, TipoPerfil
Infraestructura: PersistenciaTexto
Visualización neutral: GraficaDTO e IVisualizadorGraficas
```

## 2. Separación entre interfaz y lógica

`soundbridge_core` contiene dominio, aplicación, persistencia y utilidades. No contiene
`std::cin`, menús ni ventanas.

`soundbridge_console` contiene entrada, salida y el visualizador no disponible. El
ejecutable `soundbridge` enlaza `soundbridge_console`.

Un futuro ejecutable visual debe enlazar `soundbridge_core` y usar
`ControladorAplicacion`.

## 3. Capa pública

`ControladorAplicacion` es la fachada única. En la Fase 10 pasó a poseer
internamente `SoundBridge`. Esto evita que la presentación tenga que crear
o conservar la clase propietaria de punteros.

La copia del controlador está deshabilitada porque representa un estado único.

## 4. Lógica de negocio

`SoundBridge` administra:

- Arreglo dinámico de `Perfil*`.
- Arreglo dinámico de `Conexion*`.
- Creación de IDs.
- Afinidad.
- Compatibilidad.
- Conexiones.
- Eliminación en cascada.

Es la única propietaria de los objetos y libera memoria con `delete` y
`delete[]`.

## 5. Dominio

`Perfil` es abstracta y define los datos comunes. Los tipos derivados agregan
sus campos específicos y sobrescriben `obtenerTipo`, `imprimir` y
`obtenerCamposEspecificos`.

`Conexion` guarda únicamente IDs y afinidad. Normaliza el par guardando primero
el ID menor.

## 6. Afinidad

```text
+60 principales iguales
+15 principal A en secundarios de B
+15 principal B en secundarios de A
+5 por secundario compartido, máximo 10
máximo total 100
```

Una conexión normal necesita al menos 40.

## 7. DTO y resultados

Los DTO son copias por valor y no entregan propiedad del dominio:

- `PerfilDTO`.
- `PerfilCompatibleDTO`.
- `ConexionDTO`.
- `GraficaDTO`.

Los resultados incluyen un booleano y un mensaje. La presentación no necesita
interpretar excepciones de validación de perfiles porque el controlador las
convierte en respuestas.

## 8. Persistencia

`PersistenciaTexto` guarda en formato versionado. Escapa `\\`, `|`, `;`, saltos
de línea y retornos de carro.

Guardado seguro:

1. Crear directorio.
2. Escribir archivo temporal.
3. Verificar flujo.
4. Respaldar archivo anterior.
5. Reemplazar.
6. Restaurar si ocurre un error.

Carga segura:

1. Leer en una red temporal.
2. Validar versión, perfiles, conexiones e IDs.
3. Comparar afinidades guardadas.
4. Intercambiar el estado solo al final.

## 9. Inicialización

```cpp
soundbridge::ControladorAplicacion controlador;
auto carga = controlador.cargarDatos();
```

## 10. Cierre

Se recomienda guardar antes de cerrar. Al destruirse el controlador, su
`SoundBridge` libera todos los perfiles, conexiones y arreglos.

## 11. Compilación

Targets:

- `soundbridge_core`: núcleo sin consola ni front-end.
- `soundbridge_console`: presentación temporal.
- `soundbridge`: ejecutable de consola.
- `soundbridge_tests`: pruebas automatizadas.

El estándar requerido es C++17.

## 12. Pruebas

`tests/PruebasFase10.cpp` verifica:

- Fachada autónoma.
- Cuatro tipos de perfil y DTO.
- Validaciones.
- Afinidad y conexiones.
- Compatibilidad y orden.
- Eliminación en cascada.
- Estadísticas.
- Persistencia y caracteres escapados.
- Recuperación en otra instancia.
- Archivo inválido.
- Inicio sin archivo.

## 13. Conectar un nuevo front-end

1. Crear un nuevo ejecutable fuera de `soundbridge_core`.
2. Enlazar `soundbridge_core`.
3. Crear una instancia de `ControladorAplicacion`.
4. Llamar `cargarDatos()` al iniciar.
5. Conectar botones con los métodos documentados.
6. Mostrar los DTO.
7. Guardar después de cambios correctos.
8. Convertir `GraficaDTO` a componentes visuales.

No deben modificarse el algoritmo de afinidad, la persistencia ni la propiedad
de memoria para implementar la interfaz.
