# Especificación del futuro front-end

## Estado general

Este documento define vistas y acciones, no un diseño visual ni un framework.

```text
Implementación pendiente y preparada para una fase posterior.
```

## Pantalla 1: Inicio y carga

### Objetivo

Inicializar el controlador y cargar la información guardada.

### Información mostrada

- Nombre del proyecto.
- Estado de carga.
- Mensaje de error o inicio vacío.

### Acciones

- Continuar al menú principal.
- Reintentar una carga, si la interfaz permite seleccionar una ruta.

### Operaciones del núcleo

```cpp
ControladorAplicacion controlador;
controlador.cargarDatos();
```

### Mensajes posibles

- Datos cargados correctamente.
- El archivo no existe. Se inició una red vacía.
- Error de formato o acceso.

### Pantalla siguiente

Panel principal.

---

## Pantalla 2: Panel principal

### Objetivo

Servir como punto de navegación.

### Información mostrada

- Cantidad de perfiles.
- Cantidad de conexiones.
- Accesos a perfiles, conexiones y estadísticas.

### Operaciones

```cpp
obtenerCantidadPerfiles();
obtenerCantidadConexiones();
```

### Acciones

- Gestionar perfiles.
- Gestionar conexiones.
- Abrir estadísticas.
- Guardar.
- Salir.

---

## Pantalla 3: Lista de perfiles

### Objetivo

Mostrar todos los perfiles registrados.

### Información mostrada

- ID.
- Tipo.
- Nombre.
- Edad.
- Género principal.

### Operación

```cpp
std::vector<PerfilDTO> obtenerPerfiles();
```

### Acciones

- Crear perfil.
- Ver detalle.
- Buscar por ID.
- Buscar compatibles.
- Eliminar.
- Volver.

### Estado vacío

`No existen perfiles registrados.`

---

## Pantalla 4: Selección del tipo de perfil

### Objetivo

Elegir Oyente, Artista, Productor o Fan Club.

### Acciones

- Continuar al formulario correspondiente.
- Cancelar.

### Pantalla siguiente

Formulario de perfil.

---

## Pantalla 5: Formulario de perfil

### Objetivo

Registrar un perfil.

### Campos comunes

- Nombre.
- Edad.
- Género principal.
- Lista de géneros secundarios.

### Campos de Oyente

- Horas de escucha semanales.
- Plataforma preferida.

### Campos de Artista

- Nombre artístico.
- Instrumento principal.
- Busca colaboraciones.

### Campos de Productor

- Software principal.
- Años de experiencia.
- Ofrece servicios.

### Campos de Fan Club

- Artista representado.
- Número de miembros.
- Ciudad base.

### Validaciones

El front-end puede validar formato y campos requeridos, pero el núcleo siempre
realiza la validación definitiva.

### Operaciones

Uno de los cuatro métodos `crearPerfil...`.

### Resultado exitoso

Mostrar mensaje e ID. Guardar y regresar a la lista.

### Resultado inválido

Mantener los campos y mostrar `resultado.mensaje`.

---

## Pantalla 6: Detalle o búsqueda de perfil

### Objetivo

Mostrar todos los datos de un perfil.

### Entrada

ID positivo.

### Operación

```cpp
ResultadoConsultaPerfil buscarPerfil(int id);
```

### Información mostrada

Todos los campos de `PerfilDTO`, incluidos `camposEspecificos`.

### Acciones

- Buscar compatibles.
- Iniciar conexión.
- Eliminar perfil.
- Volver.

---

## Pantalla 7: Perfiles compatibles

### Objetivo

Mostrar candidatos compatibles de un perfil.

### Operación

```cpp
ResultadoCompatibles buscarPerfilesCompatibles(int id);
```

### Información mostrada

- ID.
- Nombre.
- Tipo.
- Afinidad.

### Acciones

- Seleccionar candidato.
- Crear conexión.
- Volver al detalle.

### Estado vacío

No existen perfiles compatibles disponibles.

---

## Pantalla 8: Crear conexión

### Objetivo

Conectar dos perfiles.

### Campos

- ID del primer perfil.
- ID del segundo perfil.

### Acciones

- Calcular afinidad opcionalmente.
- Confirmar conexión.
- Cancelar.

### Operaciones

```cpp
calcularAfinidad(perfilAId, perfilBId);
crearConexion(perfilAId, perfilBId);
```

### Mensajes posibles

- Conexión creada.
- Perfil inexistente.
- Mismo perfil.
- Conexión duplicada.
- Afinidad insuficiente.

---

## Pantalla 9: Lista de conexiones

### Objetivo

Mostrar todas las conexiones.

### Operación

```cpp
std::vector<ConexionDTO> obtenerConexiones();
```

### Información mostrada

IDs, nombres, tipos y porcentaje de afinidad.

### Acciones

- Eliminar conexión.
- Ver perfiles relacionados.
- Volver.

---

## Pantalla 10: Confirmación de eliminación de perfil

### Objetivo

Evitar una eliminación accidental.

### Advertencia

También se eliminarán las conexiones relacionadas.

### Operación

```cpp
ResultadoOperacion eliminarPerfil(int id);
```

### Resultado exitoso

Guardar, actualizar listas y regresar a perfiles.

---

## Pantalla 11: Confirmación de eliminación de conexión

### Operación

```cpp
ResultadoOperacion eliminarConexion(int perfilAId, int perfilBId);
```

### Resultado exitoso

Guardar y regresar a conexiones.

---

## Pantalla 12: Estadísticas

### Objetivo

Mostrar una de las tres estadísticas.

### Opciones

- Perfiles por género.
- Perfiles por tipo.
- Conexiones por afinidad.

### Operaciones

```cpp
obtenerGraficaPerfilesPorGenero();
obtenerGraficaPerfilesPorTipo();
obtenerGraficaConexionesPorAfinidad();
```

### Información mostrada

Título, etiquetas y puntos de `GraficaDTO`.

### Condición especial

La interfaz decide la apariencia. No recalcula los datos.

---

## Pantalla 13: Guardado y mensajes

### Objetivo

Mostrar el resultado del guardado manual o automático.

### Operación

```cpp
ResultadoOperacion guardarDatos();
```

### Mensajes

- Datos guardados correctamente.
- Error al crear directorio, escribir o reemplazar archivo.

### Regla

Si una operación en memoria fue correcta pero el guardado falló, la interfaz
debe informar ambas situaciones y permitir reintentar.
