# Contrato funcional para el futuro front-end

## 1. Punto único de acceso

La interfaz futura debe incluir:

```cpp
#include "soundbridge/application/ControladorAplicacion.hpp"
```

Luego debe crear una sola instancia:

```cpp
soundbridge::ControladorAplicacion controlador;
```

El controlador posee internamente la red. La interfaz no debe crear
`SoundBridge`, acceder a arreglos internos ni llamar directamente a
`PersistenciaTexto`.

## 2. Resultados y modelos de salida

### `ResultadoOperacion`

| Campo | Tipo | Significado |
| --- | --- | --- |
| `correcto` | `bool` | Indica si la operación terminó correctamente. |
| `mensaje` | `std::string` | Texto que puede mostrarse directamente al usuario. |

### `ResultadoCreacionPerfil`

| Campo | Tipo | Significado |
| --- | --- | --- |
| `resultado` | `ResultadoOperacion` | Estado y mensaje. |
| `idPerfil` | `int` | ID asignado cuando la creación fue correcta. |

### `PerfilDTO`

| Campo | Tipo | Uso |
| --- | --- | --- |
| `id` | `int` | Identificador de solo lectura. |
| `tipo` | `std::string` | Oyente, Artista, Productor o Fan Club. |
| `nombre` | `std::string` | Nombre visible. |
| `edad` | `int` | Edad validada. |
| `generoPrincipal` | `std::string` | Género principal. |
| `generosSecundarios` | `std::vector<std::string>` | Géneros secundarios. |
| `camposEspecificos` | `std::vector<CampoDetallePerfilDTO>` | Datos propios del tipo. |

El DTO es una copia. El front-end puede mostrarlo sin modificar el estado
interno.

### `PerfilCompatibleDTO`

Contiene `id`, `nombre`, `tipo` y `afinidad`.

### `ConexionDTO`

Contiene IDs, nombres, tipos y afinidad de los dos perfiles conectados.

### `GraficaDTO`

Contiene título, etiquetas de ejes, tipo de gráfica y puntos. El front-end debe
convertir estos datos a los componentes del framework visual elegido.

---

## 3. Inicializar el sistema

### Propósito

Crear el acceso principal al núcleo.

### Método

```cpp
soundbridge::ControladorAplicacion controlador;
```

### Entradas

Ninguna.

### Resultado exitoso

Se crea una red vacía en memoria.

### Posibles errores

Un error de memoria puede lanzar una excepción durante la construcción. El
punto de entrada de la aplicación debe capturar `std::exception`.

### Cambios realizados

Reserva los arreglos dinámicos internos iniciales.

---

## 4. Cargar datos

### Propósito

Recuperar perfiles, conexiones y siguiente ID.

### Métodos

```cpp
ResultadoOperacion cargarDatos();
ResultadoOperacion cargarDatos(const std::string& rutaArchivo);
```

### Datos de entrada

| Campo | Tipo | Obligatorio | Validación |
| --- | --- | ---: | --- |
| `rutaArchivo` | `std::string` | Solo en la sobrecarga | Debe representar una ruta utilizable. |

### Resultado exitoso

`correcto == true`. Si el archivo no existe, inicia con una red vacía y devuelve
un mensaje informativo.

### Posibles errores

- Archivo inaccesible.
- Versión inválida.
- Registro incompleto.
- IDs duplicados.
- Conexión inválida.
- Afinidad guardada diferente de la calculada.

### Cambios realizados

Reemplaza el estado solo después de validar completamente el archivo. Una carga
fallida conserva el estado anterior.

### Ejemplo

```cpp
soundbridge::ResultadoOperacion carga = controlador.cargarDatos();
mostrarMensaje(carga.correcto, carga.mensaje);
```

---

## 5. Crear perfil Oyente

### Propósito

Registrar un oyente con ID automático.

### Método

```cpp
ResultadoCreacionPerfil crearPerfilOyente(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    int horasEscuchaSemanales,
    const std::string& plataformaPreferida
);
```

### Datos de entrada

| Campo | Tipo | Obligatorio | Validación |
| --- | --- | ---: | --- |
| nombre | string | Sí | No vacío. |
| edad | int | Sí | Entre 13 y 120. |
| generoPrincipal | string | Sí | No vacío. |
| generosSecundarios | vector<string> | Sí | Al menos uno, sin vacíos ni duplicados. |
| horasEscuchaSemanales | int | Sí | Entre 0 y 168. |
| plataformaPreferida | string | Sí | No vacía. |

### Resultado exitoso

`resultado.correcto == true` e `idPerfil` contiene el ID asignado.

### Posibles errores

Cualquier validación anterior. El mensaje viene listo para mostrarse.

### Cambios realizados

Agrega un perfil en memoria. No guarda automáticamente el archivo.

### Ejemplo

```cpp
auto respuesta = controlador.crearPerfilOyente(
    nombre,
    edad,
    generoPrincipal,
    generosSecundarios,
    horas,
    plataforma
);
```

---

## 6. Crear perfil Artista

### Método

```cpp
ResultadoCreacionPerfil crearPerfilArtista(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    const std::string& nombreArtistico,
    const std::string& instrumentoPrincipal,
    bool buscaColaboraciones
);
```

### Validaciones específicas

| Campo | Tipo | Validación |
| --- | --- | --- |
| nombreArtistico | string | No vacío. |
| instrumentoPrincipal | string | No vacío. |
| buscaColaboraciones | bool | `true` o `false`. |

Aplica también todas las validaciones comunes de perfil.

### Resultado y cambios

Devuelve ID y mensaje. Agrega el artista en memoria y requiere llamar a
`guardarDatos()` para persistir.

---

## 7. Crear perfil Productor

### Método

```cpp
ResultadoCreacionPerfil crearPerfilProductor(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    const std::string& softwarePrincipal,
    int aniosExperiencia,
    bool ofreceServicios
);
```

### Validaciones específicas

| Campo | Tipo | Validación |
| --- | --- | --- |
| softwarePrincipal | string | No vacío. |
| aniosExperiencia | int | Entre 0 y 80. |
| ofreceServicios | bool | `true` o `false`. |

### Resultado y cambios

Devuelve ID y mensaje. Agrega el productor en memoria.

---

## 8. Crear perfil Fan Club

### Método

```cpp
ResultadoCreacionPerfil crearPerfilFanClub(
    const std::string& nombre,
    int edad,
    const std::string& generoPrincipal,
    const std::vector<std::string>& generosSecundarios,
    const std::string& artistaRepresentado,
    int numeroMiembros,
    const std::string& ciudadBase
);
```

### Validaciones específicas

| Campo | Tipo | Validación |
| --- | --- | --- |
| artistaRepresentado | string | No vacío. |
| numeroMiembros | int | Igual o mayor a 1. |
| ciudadBase | string | No vacía. |

### Resultado y cambios

Devuelve ID y mensaje. Agrega el fan club en memoria.

---

## 9. Obtener todos los perfiles

### Método

```cpp
std::vector<PerfilDTO> obtenerPerfiles() const;
```

### Entrada

Ninguna.

### Salida

Vector por valor. Un vector vacío significa que no existen perfiles.

### Cambios realizados

Ninguno.

### Ejemplo

```cpp
std::vector<soundbridge::PerfilDTO> perfiles = controlador.obtenerPerfiles();
```

---

## 10. Buscar perfil

### Método

```cpp
ResultadoConsultaPerfil buscarPerfil(int id) const;
```

### Entrada

| Campo | Tipo | Validación |
| --- | --- | --- |
| id | int | Mayor que cero. |

### Resultado exitoso

`resultado.correcto == true` y `perfil` contiene el DTO.

### Posibles errores

- ID no positivo.
- Perfil inexistente.

### Cambios realizados

Ninguno.

---

## 11. Buscar perfiles compatibles

### Método

```cpp
ResultadoCompatibles buscarPerfilesCompatibles(int id) const;
```

### Reglas

- El perfil consultado debe existir.
- Se excluye el mismo perfil.
- Se excluyen conexiones existentes.
- Se excluyen afinidades menores a 40.
- Se ordena por afinidad descendente y por ID ascendente en empate.

### Salida

`ResultadoCompatibles` con mensaje y vector. Una lista vacía con resultado
correcto significa que no hay candidatos disponibles.

### Cambios realizados

Ninguno.

---

## 12. Calcular afinidad

### Método

```cpp
ResultadoAfinidad calcularAfinidad(int perfilAId, int perfilBId) const;
```

### Entradas

Dos IDs positivos y diferentes.

### Resultado exitoso

`afinidad` entre 0 y 100.

### Posibles errores

- ID no positivo.
- Mismo perfil.
- Uno o ambos perfiles inexistentes.

### Cambios realizados

Ninguno.

---

## 13. Crear conexión

### Método

```cpp
ResultadoConexionAplicacion crearConexion(
    int perfilAId,
    int perfilBId
);
```

### Validaciones

- IDs positivos.
- Perfiles existentes.
- IDs diferentes.
- Conexión no duplicada.
- Afinidad mínima de 40.

### Resultado exitoso

`resultado.correcto == true` y `afinidad` contiene el porcentaje registrado.

### Posibles errores

- Perfil inexistente.
- Autoconexión.
- Duplicado.
- Afinidad insuficiente.

### Cambios realizados

Agrega una conexión en memoria.

---

## 14. Obtener conexiones

### Método

```cpp
std::vector<ConexionDTO> obtenerConexiones() const;
```

### Salida

Vector con IDs, nombres, tipos y afinidad. Un vector vacío significa que no hay
conexiones.

### Cambios realizados

Ninguno.

---

## 15. Eliminar perfil

### Método

```cpp
ResultadoOperacion eliminarPerfil(int id);
```

### Validaciones

ID positivo y perfil existente.

### Resultado exitoso

Mensaje de eliminación correcta.

### Cambios realizados

Elimina el perfil y todas sus conexiones relacionadas.

### Posibles errores

- ID no positivo.
- Perfil inexistente.

---

## 16. Eliminar conexión

### Método

```cpp
ResultadoOperacion eliminarConexion(int perfilAId, int perfilBId);
```

### Validaciones

IDs positivos, diferentes y conexión existente.

### Cambios realizados

Elimina solamente la conexión.

---

## 17. Guardar datos

### Métodos

```cpp
ResultadoOperacion guardarDatos() const;
ResultadoOperacion guardarDatos(const std::string& rutaArchivo) const;
```

### Resultado exitoso

El archivo queda actualizado mediante escritura temporal y reemplazo seguro.

### Posibles errores

- No se puede crear el directorio.
- No se puede abrir el temporal.
- Fallo de escritura o reemplazo.

### Cambios realizados

Actualiza el archivo; no modifica la red en memoria.

---

## 18. Obtener estadísticas

### Métodos

```cpp
GraficaDTO obtenerGraficaPerfilesPorGenero() const;
GraficaDTO obtenerGraficaPerfilesPorTipo() const;
GraficaDTO obtenerGraficaConexionesPorAfinidad() const;
```

### Salida

Cada método devuelve un `GraficaDTO` listo para convertir a barras o gráfica
circular.

### Reglas

- Tipos: siempre aparecen las cuatro categorías.
- Afinidad: siempre aparecen `40-59`, `60-79` y `80-100`.
- Géneros: se agrupan ignorando mayúsculas y espacios externos.

### Cambios realizados

Ninguno.

---

## 19. Consultar cantidades

### Métodos

```cpp
std::size_t obtenerCantidadPerfiles() const noexcept;
std::size_t obtenerCantidadConexiones() const noexcept;
```

Se usan para decidir estados vacíos, habilitar botones o mostrar contadores.

---

## 20. Regla de guardado para el front-end

Las operaciones que modifican memoria no guardan por sí solas. La interfaz debe
seguir este flujo:

```cpp
auto respuesta = controlador.eliminarPerfil(id);

if (respuesta.correcto) {
    auto guardado = controlador.guardarDatos();
    mostrarMensaje(guardado.mensaje);
} else {
    mostrarError(respuesta.mensaje);
}
```

Esto permite que cada tecnología decida si guarda inmediatamente, al cerrar o
mediante un botón. La consola actual guarda automáticamente después de cambios
correctos.
