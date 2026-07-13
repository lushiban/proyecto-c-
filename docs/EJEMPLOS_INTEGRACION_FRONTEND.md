# Ejemplos de integración del futuro front-end

Los ejemplos usan nombres reales del núcleo. Las funciones `mostrarExito`,
`mostrarError`, `actualizarTabla` y `dibujarGrafica` representan código que debe
implementar el framework visual.

## 1. Inicialización y carga

```cpp
#include "soundbridge/application/ControladorAplicacion.hpp"

soundbridge::ControladorAplicacion controlador;
soundbridge::ResultadoOperacion carga = controlador.cargarDatos();

if (carga.correcto) {
    mostrarExito(carga.mensaje);
} else {
    mostrarError(carga.mensaje);
}
```

El controlador y `cargarDatos()` pertenecen al núcleo. Las funciones para mostrar
mensajes pertenecen al front-end.

## 2. Actualizar una lista de perfiles

```cpp
std::vector<soundbridge::PerfilDTO> perfiles = controlador.obtenerPerfiles();

if (perfiles.empty()) {
    mostrarEstadoVacio("No existen perfiles registrados.");
} else {
    actualizarTabla(perfiles);
}
```

## 3. Registrar un oyente desde un botón

```cpp
auto respuesta = controlador.crearPerfilOyente(
    nombreIngresado,
    edadIngresada,
    generoPrincipalIngresado,
    generosSecundariosIngresados,
    horasIngresadas,
    plataformaIngresada
);

if (!respuesta.resultado.correcto) {
    mostrarError(respuesta.resultado.mensaje);
    return;
}

mostrarExito(
    respuesta.resultado.mensaje
    + " ID: " + std::to_string(respuesta.idPerfil)
);

soundbridge::ResultadoOperacion guardado = controlador.guardarDatos();
mostrarMensajeGuardado(guardado.correcto, guardado.mensaje);
```

## 4. Registrar otros tipos

```cpp
auto artista = controlador.crearPerfilArtista(
    nombre,
    edad,
    generoPrincipal,
    generosSecundarios,
    nombreArtistico,
    instrumento,
    buscaColaboraciones
);

auto productor = controlador.crearPerfilProductor(
    nombre,
    edad,
    generoPrincipal,
    generosSecundarios,
    software,
    aniosExperiencia,
    ofreceServicios
);

auto fanClub = controlador.crearPerfilFanClub(
    nombre,
    edad,
    generoPrincipal,
    generosSecundarios,
    artistaRepresentado,
    numeroMiembros,
    ciudadBase
);
```

## 5. Buscar un perfil

```cpp
soundbridge::ResultadoConsultaPerfil respuesta = controlador.buscarPerfil(idBuscado);

if (respuesta.resultado.correcto) {
    mostrarDetalle(respuesta.perfil);
} else {
    mostrarError(respuesta.resultado.mensaje);
}
```

## 6. Buscar compatibles

```cpp
soundbridge::ResultadoCompatibles respuesta =
    controlador.buscarPerfilesCompatibles(idPerfil);

if (!respuesta.resultado.correcto) {
    mostrarError(respuesta.resultado.mensaje);
    return;
}

mostrarCompatibles(respuesta.perfiles);
```

## 7. Calcular afinidad antes de conectar

```cpp
soundbridge::ResultadoAfinidad afinidad =
    controlador.calcularAfinidad(perfilAId, perfilBId);

if (afinidad.resultado.correcto) {
    mostrarPorcentaje(afinidad.afinidad);
} else {
    mostrarError(afinidad.resultado.mensaje);
}
```

## 8. Crear conexión

```cpp
soundbridge::ResultadoConexionAplicacion respuesta =
    controlador.crearConexion(perfilAId, perfilBId);

if (respuesta.resultado.correcto) {
    mostrarExito(respuesta.resultado.mensaje);
    mostrarPorcentaje(respuesta.afinidad);
    controlador.guardarDatos();
} else {
    mostrarError(respuesta.resultado.mensaje);
}
```

## 9. Eliminar perfil

```cpp
if (usuarioConfirmo) {
    soundbridge::ResultadoOperacion resultado = controlador.eliminarPerfil(idPerfil);

    if (resultado.correcto) {
        controlador.guardarDatos();
        actualizarTabla(controlador.obtenerPerfiles());
    }

    mostrarMensaje(resultado.correcto, resultado.mensaje);
}
```

## 10. Eliminar conexión

```cpp
soundbridge::ResultadoOperacion resultado =
    controlador.eliminarConexion(perfilAId, perfilBId);

if (resultado.correcto) {
    controlador.guardarDatos();
    actualizarConexiones(controlador.obtenerConexiones());
} else {
    mostrarError(resultado.mensaje);
}
```

## 11. Mostrar una gráfica

```cpp
soundbridge::GraficaDTO grafica = controlador.obtenerGraficaPerfilesPorTipo();
dibujarGrafica(grafica);
```

`dibujarGrafica` debe recorrer `grafica.puntos` y convertir cada etiqueta y valor
a los tipos de la librería visual.

## 12. Cierre

```cpp
soundbridge::ResultadoOperacion guardado = controlador.guardarDatos();

if (guardado.correcto) {
    cerrarVentanaPrincipal();
} else {
    mostrarError(guardado.mensaje);
}
```

No existe un método especial de cierre porque los destructores liberan la
memoria automáticamente cuando termina la instancia del controlador.
