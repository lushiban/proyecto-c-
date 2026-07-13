# Casos de uso

## CU-01: Iniciar el sistema

**Actor:** Usuario.  
**Objetivo:** recuperar el estado guardado.  
**Condición previa:** aplicación iniciada.  
**Entrada:** ruta predeterminada o personalizada.

**Flujo principal:**

1. La presentación crea `ControladorAplicacion`.
2. Llama `cargarDatos()`.
3. El núcleo valida el archivo completo.
4. Devuelve estado y mensaje.
5. La presentación abre el panel principal.

**Alternativas:** archivo inexistente, formato inválido o acceso denegado.

**Resultado:** red cargada o red vacía segura.  
**Módulo:** `ControladorAplicacion` y `PersistenciaTexto`.

## CU-02: Registrar perfil

**Actor:** Usuario.  
**Objetivo:** crear un perfil musical.  
**Condición previa:** sistema iniciado.  
**Entrada:** datos comunes y específicos.

**Flujo principal:**

1. El usuario selecciona el tipo.
2. Completa el formulario.
3. El front-end hace validaciones básicas.
4. Llama al método `crearPerfil...` correspondiente.
5. El núcleo valida definitivamente y asigna ID.
6. El front-end muestra el mensaje y el ID.
7. Llama `guardarDatos()`.

**Alternativas:** campos vacíos, edad inválida, géneros duplicados o dato
específico fuera de rango.

**Resultado:** perfil agregado o estado sin cambios.

## CU-03: Listar perfiles

**Actor:** Usuario.  
**Objetivo:** consultar todos los perfiles.  
**Entrada:** ninguna.  
**Flujo:** llamar `obtenerPerfiles()` y mostrar cada `PerfilDTO`.  
**Alternativa:** vector vacío.  
**Resultado:** lista de solo lectura.

## CU-04: Buscar perfil

**Actor:** Usuario.  
**Entrada:** ID.  
**Flujo:** llamar `buscarPerfil(id)` y mostrar el DTO.  
**Alternativas:** ID no positivo o inexistente.  
**Resultado:** detalle o mensaje de error.

## CU-05: Calcular afinidad

**Actor:** Usuario.  
**Entrada:** dos IDs diferentes.  
**Flujo:** llamar `calcularAfinidad(a, b)`.  
**Alternativas:** perfiles inexistentes o mismo ID.  
**Resultado:** porcentaje entre 0 y 100.

## CU-06: Buscar perfiles compatibles

**Actor:** Usuario.  
**Condición previa:** perfil existente.  
**Entrada:** ID del perfil base.

**Flujo principal:**

1. Llamar `buscarPerfilesCompatibles(id)`.
2. El núcleo excluye el mismo perfil, conexiones y afinidades menores a 40.
3. Ordena por afinidad e ID.
4. Devuelve DTO.

**Alternativas:** perfil inexistente o lista vacía.  
**Resultado:** candidatos disponibles.

## CU-07: Crear conexión

**Actor:** Usuario.  
**Entrada:** dos IDs.

**Flujo principal:**

1. El usuario selecciona perfiles.
2. La interfaz puede llamar `calcularAfinidad()`.
3. Llama `crearConexion()`.
4. El núcleo valida existencia, diferencia, duplicado y afinidad.
5. La interfaz muestra el resultado y guarda.

**Alternativas:** autoconexión, duplicado, inexistencia o afinidad menor a 40.

**Resultado:** conexión registrada o estado sin cambios.

## CU-08: Listar conexiones

**Actor:** Usuario.  
**Entrada:** ninguna.  
**Flujo:** llamar `obtenerConexiones()` y mostrar `ConexionDTO`.  
**Alternativa:** vector vacío.

## CU-09: Eliminar conexión

**Actor:** Usuario.  
**Entrada:** dos IDs y confirmación visual.  
**Flujo:** llamar `eliminarConexion()`, mostrar resultado y guardar.  
**Alternativas:** conexión inexistente o IDs inválidos.

## CU-10: Eliminar perfil

**Actor:** Usuario.  
**Entrada:** ID y confirmación.

**Flujo principal:**

1. Mostrar advertencia de eliminación en cascada.
2. Llamar `eliminarPerfil(id)`.
3. El núcleo elimina conexiones relacionadas.
4. Guardar.
5. Actualizar listas.

**Alternativas:** ID inválido o inexistente.

## CU-11: Consultar estadísticas

**Actor:** Usuario.  
**Entrada:** tipo de estadística.  
**Flujo:** llamar al método `obtenerGrafica...` y dibujar `GraficaDTO`.  
**Alternativa:** datos vacíos representados con cero.  
**Resultado:** información estadística sin cambiar el estado.

## CU-12: Guardar y cerrar

**Actor:** Usuario.  
**Entrada:** ninguna o ruta personalizada.  
**Flujo:** llamar `guardarDatos()`, mostrar mensaje y cerrar si fue correcto.  
**Alternativa:** error de escritura; permitir reintento.  
**Resultado:** archivo actualizado o estado en memoria conservado.
