# Matriz de funcionalidades

| Funcionalidad | Clase responsable | Método público | Entrada | Salida | Pantalla futura | Estado |
| --- | --- | --- | --- | --- | --- | --- |
| Inicializar núcleo | ControladorAplicacion | Constructor | Ninguna | Instancia lista | Inicio | Completa y probada |
| Cargar datos | ControladorAplicacion | `cargarDatos` | Ruta opcional | ResultadoOperacion | Inicio | Completa y probada |
| Crear oyente | ControladorAplicacion | `crearPerfilOyente` | Datos comunes, horas, plataforma | ResultadoCreacionPerfil | Formulario | Completa y probada |
| Crear artista | ControladorAplicacion | `crearPerfilArtista` | Datos comunes, nombre artístico, instrumento, bandera | ResultadoCreacionPerfil | Formulario | Completa y probada |
| Crear productor | ControladorAplicacion | `crearPerfilProductor` | Datos comunes, software, experiencia, bandera | ResultadoCreacionPerfil | Formulario | Completa y probada |
| Crear fan club | ControladorAplicacion | `crearPerfilFanClub` | Datos comunes, artista, miembros, ciudad | ResultadoCreacionPerfil | Formulario | Completa y probada |
| Listar perfiles | ControladorAplicacion | `obtenerPerfiles` | Ninguna | vector de PerfilDTO | Lista de perfiles | Completa y probada |
| Buscar perfil | ControladorAplicacion | `buscarPerfil` | ID | ResultadoConsultaPerfil | Detalle | Completa y probada |
| Buscar compatibles | ControladorAplicacion | `buscarPerfilesCompatibles` | ID | ResultadoCompatibles | Compatibles | Completa y probada |
| Calcular afinidad | ControladorAplicacion | `calcularAfinidad` | Dos IDs | ResultadoAfinidad | Crear conexión | Completa y probada |
| Crear conexión | ControladorAplicacion | `crearConexion` | Dos IDs | ResultadoConexionAplicacion | Crear conexión | Completa y probada |
| Listar conexiones | ControladorAplicacion | `obtenerConexiones` | Ninguna | vector de ConexionDTO | Lista de conexiones | Completa y probada |
| Eliminar perfil | ControladorAplicacion | `eliminarPerfil` | ID | ResultadoOperacion | Confirmación | Completa y probada |
| Eliminar conexión | ControladorAplicacion | `eliminarConexion` | Dos IDs | ResultadoOperacion | Confirmación | Completa y probada |
| Guardar datos | ControladorAplicacion | `guardarDatos` | Ruta opcional | ResultadoOperacion | Guardado | Completa y probada |
| Contar perfiles | ControladorAplicacion | `obtenerCantidadPerfiles` | Ninguna | size_t | Panel principal | Completa y probada |
| Contar conexiones | ControladorAplicacion | `obtenerCantidadConexiones` | Ninguna | size_t | Panel principal | Completa y probada |
| Estadística por género | ControladorAplicacion | `obtenerGraficaPerfilesPorGenero` | Ninguna | GraficaDTO | Estadísticas | Completa y probada |
| Estadística por tipo | ControladorAplicacion | `obtenerGraficaPerfilesPorTipo` | Ninguna | GraficaDTO | Estadísticas | Completa y probada |
| Estadística por afinidad | ControladorAplicacion | `obtenerGraficaConexionesPorAfinidad` | Ninguna | GraficaDTO | Estadísticas | Completa y probada |
| Editar perfil | No pertenece al alcance | No existe | No aplicable | No aplicable | No definida | No aplicable |
| Interfaz visual | Front-end futuro | Usará los métodos anteriores | Eventos y formularios | Presentación visual | Todas | Limitación documentada |
