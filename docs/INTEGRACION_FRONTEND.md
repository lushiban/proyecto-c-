# Integración del futuro front-end

## Estado

```text
Implementación pendiente y preparada para una fase posterior.
```

No existe una dependencia gráfica integrada. `external/` y
`cmake/GraphicsVendor.cmake` se conservan como puntos de preparación, pero no
forman parte de la compilación normal.

## Componente que debe utilizarse

Toda interfaz nueva debe trabajar con:

```cpp
soundbridge::ControladorAplicacion
```

El controlador posee la red y devuelve DTO por valor. No deben usarse
`SoundBridge`, `PersistenciaTexto`, `Perfil*` ni `Conexion*` desde botones,
formularios o ventanas.

## Flujo de inicio

1. Crear `ControladorAplicacion`.
2. Llamar `cargarDatos()`.
3. Mostrar el mensaje devuelto.
4. Obtener perfiles y conexiones.
5. Abrir la pantalla principal.

## Flujo de una acción

1. El front-end lee campos visuales.
2. Convierte los textos a tipos C++.
3. Llama al método del controlador.
4. Revisa `correcto`.
5. Muestra `mensaje`.
6. Si hubo cambio, llama `guardarDatos()`.
7. Actualiza las listas visibles.

## Estadísticas

El front-end recibe `GraficaDTO`. Debe transformar cada `PuntoGrafica` a la
serie o componente de la librería elegida. El cálculo no debe repetirse en la
interfaz.

## Archivos relacionados con el antiguo intento gráfico

- `cmake/GraphicsVendor.cmake`: reserva el punto de configuración.
- `external/.gitkeep`: reserva la carpeta de dependencia futura.
- `IVisualizadorGraficas`: contrato opcional para un visualizador.
- `VisualizadorNoDisponible`: implementación temporal de consola.

Ninguno introduce una dependencia visual dentro de `soundbridge_core`.

## Pasos cuando se elija una tecnología

1. Crear un target separado para la aplicación visual.
2. Enlazarlo con `soundbridge_core`.
3. Crear ventanas y formularios.
4. Conectar eventos con `ControladorAplicacion`.
5. Implementar la presentación de `GraficaDTO`.
6. Completar `GraphicsVendor.cmake` solo con la dependencia real.
7. Mantener `SOUNDBRIDGE_ENABLE_GRAPHICS=OFF` como opción válida.
