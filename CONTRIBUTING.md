# Contribuciones de SoundBridge

Este documento registra la distribución del trabajo del proyecto y el uso de herramientas de inteligencia artificial.

## Declaración del uso de inteligencia artificial

El **frontend web** fue creado con asistencia de inteligencia artificial de OpenAI. La IA ayudó a generar:

- la estructura inicial de `frontend/index.html`;
- el tema visual y el diseño responsive de `frontend/styles.css`;
- la interacción y las solicitudes HTTP de `frontend/app.js`;
- comentarios y documentación relacionados con la interfaz.

El resultado fue integrado con el backend C++, compilado y probado como una aplicación local. Esta declaración se mantiene visible en el frontend, en `README.md`, en este archivo y en comentarios de los archivos visuales.

## Distribución del trabajo

La división se realizó considerando cantidad de módulos y dificultad. Paula se encargó de más componentes directos y repetitivos; Eduardo asumió los módulos con mayor complejidad técnica e integración.

### Paula Gallardo

- Clase padre abstracta `Perfil`.
- Constructores, getters, setters y validaciones de los datos comunes.
- Cálculo base de afinidad.
- Clases hijas `PerfilOyente`, `PerfilArtista`, `PerfilProductor` y `PerfilFanClub`.
- Implementación de los datos especiales de cada clase hija.
- `FabricaPerfiles` para seleccionar la clase hija correspondiente.
- Revisión de nombres, etiquetas y mensajes mostrados en los formularios del frontend asistido por IA.
- Revisión visual del tema aesthetic y comprobación de creación de perfiles.
- Documentación de clases padre, clases hijas, herencia y polimorfismo.

### Eduardo Tapia

- Clase `Conexion` y detección de conexiones repetidas en cualquier orden.
- Clase `RedSocialMusical`.
- Administración de arreglos dinámicos y liberación de memoria.
- Eliminación de perfiles y de sus conexiones asociadas.
- Guardado y carga mediante `datos.txt`.
- Clase `AplicacionSoundBridge` y modo consola.
- Clase `ServidorWeb` y API HTTP local.
- Integración del frontend asistido por IA con el backend C++.
- Configuración multiplataforma de sockets y CMake.
- Pruebas de compilación, persistencia, conexiones y servidor.

## Trabajo compartido

- Selección del nombre **SoundBridge**.
- Revisión de los requisitos de la actividad.
- Revisión final de la documentación.
- Pruebas manuales de la interfaz.
- Verificación de que la declaración del uso de IA sea visible y clara.

## Reglas mantenidas en el proyecto

- Las declaraciones de clases están en archivos `.h`.
- Las implementaciones están en archivos `.cpp`.
- `main.cpp` delega la lógica en clases especializadas.
- Los comentarios se usan en herencia, polimorfismo, memoria, sockets y persistencia, sin comentar cada línea.
- La carpeta `frontend/` contiene únicamente la presentación y la interacción del navegador.
- Las reglas del sistema se mantienen en el backend C++.
- El frontend no utiliza servicios de internet ni dependencias externas.
- `datos.txt`, `build/` y los ejecutables no deben subirse al repositorio.

## Comprobaciones antes de entregar

- [x] El proyecto compila con C++17 y CMake.
- [x] El proyecto compila directamente con `g++` en el entorno de verificación.
- [x] No se encontraron advertencias con `-Wall -Wextra -Wpedantic`.
- [x] El frontend carga desde el servidor C++.
- [x] Se pueden crear perfiles y conexiones.
- [x] Se pueden calcular afinidades.
- [x] Los datos se guardan en `datos.txt`.
- [x] El frontend declara de forma visible que fue realizado con asistencia de IA.
