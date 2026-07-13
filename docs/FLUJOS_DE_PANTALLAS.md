# Flujos de pantallas

## Mapa general

```text
Inicio y carga
      |
      v
Panel principal
      |
      +----> Perfiles
      |        +----> Seleccionar tipo
      |        |        +----> Formulario de creación
      |        +----> Detalle / búsqueda
      |        |        +----> Compatibles
      |        |        +----> Crear conexión
      |        |        +----> Eliminar perfil
      |        +----> Lista de perfiles
      |
      +----> Conexiones
      |        +----> Crear conexión
      |        +----> Eliminar conexión
      |
      +----> Estadísticas
      |        +----> Por género
      |        +----> Por tipo
      |        +----> Por afinidad
      |
      +----> Guardar
      +----> Salir
```

## Flujo 1: Inicio

1. Punto de inicio: apertura de la aplicación.
2. Acción: crear `ControladorAplicacion`.
3. Datos: ninguno.
4. Método: `cargarDatos()`.
5. Éxito: mostrar mensaje y abrir panel principal.
6. Error: mostrar mensaje; el estado anterior no se reemplaza.
7. Regreso: panel principal o cierre controlado.

## Flujo 2: Registrar perfil

1. Inicio: lista de perfiles.
2. Acción: seleccionar Crear.
3. Datos: comunes y específicos del tipo.
4. Método: `crearPerfilOyente`, `crearPerfilArtista`,
   `crearPerfilProductor` o `crearPerfilFanClub`.
5. Éxito: mostrar ID, llamar `guardarDatos()` y actualizar lista.
6. Error: mostrar mensaje y conservar formulario.
7. Regreso: lista de perfiles.

## Flujo 3: Consultar perfil

1. Inicio: lista o buscador.
2. Acción: seleccionar o ingresar ID.
3. Dato: ID positivo.
4. Método: `buscarPerfil(id)`.
5. Éxito: abrir detalle.
6. Error: mostrar mensaje.
7. Regreso: lista o buscador.

## Flujo 4: Buscar compatibles

1. Inicio: detalle de perfil.
2. Acción: seleccionar Compatibles.
3. Dato: ID del perfil actual.
4. Método: `buscarPerfilesCompatibles(id)`.
5. Éxito: mostrar lista ordenada.
6. Sin resultados: mostrar estado vacío.
7. Regreso: detalle del perfil.

## Flujo 5: Crear conexión

1. Inicio: compatibles o gestión de conexiones.
2. Acción: seleccionar dos perfiles.
3. Datos: dos IDs.
4. Método opcional: `calcularAfinidad()`.
5. Método definitivo: `crearConexion()`.
6. Éxito: guardar y mostrar la conexión.
7. Error: mostrar el mensaje sin cambiar de pantalla.

## Flujo 6: Eliminar perfil

1. Inicio: detalle o lista.
2. Acción: eliminar.
3. Datos: ID y confirmación visual.
4. Método: `eliminarPerfil(id)`.
5. Éxito: guardar; actualizar perfiles y conexiones.
6. Error: mostrar mensaje.
7. Regreso: lista de perfiles.

## Flujo 7: Eliminar conexión

1. Inicio: lista de conexiones.
2. Acción: eliminar una fila.
3. Datos: dos IDs y confirmación.
4. Método: `eliminarConexion(a, b)`.
5. Éxito: guardar y actualizar lista.
6. Error: mostrar mensaje.
7. Regreso: lista de conexiones.

## Flujo 8: Estadística

1. Inicio: panel principal.
2. Acción: seleccionar tipo de estadística.
3. Datos: ninguno.
4. Método: uno de los tres `obtenerGrafica...`.
5. Éxito: convertir `GraficaDTO` al componente visual.
6. Estado vacío: mostrar categorías con valor cero cuando corresponda.
7. Regreso: panel de estadísticas.

## Flujo 9: Cierre

1. Inicio: cualquier pantalla principal.
2. Acción: cerrar aplicación.
3. Método recomendado: `guardarDatos()`.
4. Éxito: cerrar.
5. Error: permitir reintentar, cancelar el cierre o cerrar con advertencia.
