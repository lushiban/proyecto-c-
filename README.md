# SoundBridge

SoundBridge es una red social musical desarrollada en **C++17**. Permite crear perfiles, calcular afinidades, conectar personas y conservar la información en `datos.txt`.

La versión actual incluye dos formas de uso:

- **Frontend web local:** interfaz visual que se abre en el navegador y se comunica con el backend C++.
- **Modo consola:** conserva el menú original para revisar la lógica sin utilizar el navegador.

> **Declaración de uso de IA:** el diseño visual, la estructura HTML, los estilos CSS y la interacción JavaScript del frontend fueron creados con asistencia de inteligencia artificial de OpenAI. El frontend fue integrado y revisado junto con el backend C++ del proyecto. Esta declaración también aparece dentro de la interfaz y en `CONTRIBUTING.md`.

## Funcionalidades

- Crear perfiles de tipo **Oyente**, **Artista**, **Productor** o **FanClub**.
- Ver y buscar perfiles desde tarjetas visuales.
- Calcular las mejores afinidades de un perfil, ordenadas de mayor a menor.
- Conectar dos perfiles y mostrar su porcentaje de afinidad.
- Eliminar un perfil junto con sus conexiones.
- Guardar y volver a cargar perfiles y conexiones mediante `datos.txt`.
- Utilizar una interfaz web responsive o el menú de consola.

## Arquitectura

```text
Navegador
   ↓ solicitudes HTTP locales
ServidorWeb (C++)
   ↓
RedSocialMusical
   ↓
Perfil / clases hijas / Conexion
   ↓
datos.txt
```

El servidor escucha únicamente en `127.0.0.1`, por lo que la aplicación se mantiene en la computadora local. No necesita conexión a internet ni librerías web externas.

## Estructura del proyecto

```text
SoundBridge/
├── frontend/
│   ├── index.html
│   ├── styles.css
│   └── app.js
├── include/
│   ├── AplicacionSoundBridge.h
│   ├── Conexion.h
│   ├── FabricaPerfiles.h
│   ├── Perfil.h
│   ├── PerfilArtista.h
│   ├── PerfilFanClub.h
│   ├── PerfilOyente.h
│   ├── PerfilProductor.h
│   ├── RedSocialMusical.h
│   ├── ServidorWeb.h
│   └── Utilidades.h
├── src/
│   ├── AplicacionSoundBridge.cpp
│   ├── Conexion.cpp
│   ├── FabricaPerfiles.cpp
│   ├── Perfil.cpp
│   ├── PerfilArtista.cpp
│   ├── PerfilFanClub.cpp
│   ├── PerfilOyente.cpp
│   ├── PerfilProductor.cpp
│   ├── RedSocialMusical.cpp
│   ├── ServidorWeb.cpp
│   └── Utilidades.cpp
├── .gitignore
├── CMakeLists.txt
├── CONTRIBUTING.md
├── README.md
└── main.cpp
```

La carpeta `frontend/` **sí es necesaria** para la interfaz gráfica. CMake la copia automáticamente junto al ejecutable después de compilar.

## Requisitos

- Un compilador compatible con **C++17**.
- **CMake 3.16** o superior.
- Un navegador moderno, como Chrome, Edge, Firefox o Safari.

No se necesitan frameworks web, Node.js, bases de datos ni dependencias externas.

## Compilar y ejecutar con CMake

Desde la carpeta raíz `SoundBridge/`:

```bash
cmake -S . -B build
cmake --build build
```

### Linux o macOS

```bash
./build/soundbridge
```

### Windows con MinGW, Ninja u otro generador de una sola configuración

```powershell
.\build\soundbridge.exe
```

### Windows con Visual Studio

```powershell
cmake --build build --config Release
.\build\Release\soundbridge.exe
```

Cuando el programa inicia, abre automáticamente:

```text
http://127.0.0.1:8080
```

Si el navegador no se abre solo, copia esa dirección manualmente.

## Opciones de ejecución

```text
soundbridge                     Inicia la interfaz web
soundbridge --consola           Inicia el menú tradicional
soundbridge --sin-navegador     No abre el navegador automáticamente
soundbridge --puerto 8081       Utiliza otro puerto local
soundbridge --ayuda             Muestra la ayuda
```

Ejemplo si el puerto 8080 ya está ocupado:

```bash
./build/soundbridge --puerto 8081
```

## Compilación directa con g++

### Linux o macOS

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp src/*.cpp -Iinclude -o soundbridge
./soundbridge
```

### Windows con MinGW

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp src/*.cpp -Iinclude -lws2_32 -o soundbridge.exe
.\soundbridge.exe
```

Para la compilación directa, ejecuta el programa desde la raíz del proyecto para que encuentre `frontend/`.

## Punto de entrada

`main.cpp` no contiene toda la lógica. Su responsabilidad es:

1. Interpretar opciones como `--consola` o `--puerto`.
2. Localizar la carpeta del frontend.
3. Crear `ServidorWeb` o `AplicacionSoundBridge`.
4. Delegar la ejecución.

El flujo visual principal es:

```text
main()
  ↓
ServidorWeb::ejecutar()
  ↓
API HTTP local
  ↓
RedSocialMusical
```

## Frontend y backend

### Frontend

Los archivos de `frontend/` forman la interfaz:

- `index.html`: estructura y contenido visual.
- `styles.css`: tema aesthetic pastel, diseño responsive y animaciones.
- `app.js`: formularios, navegación y comunicación con la API local.

### Backend

El backend continúa completamente en C++:

- `ServidorWeb` recibe solicitudes del navegador.
- `RedSocialMusical` aplica las reglas del sistema.
- `Perfil` actúa como clase padre abstracta.
- Las clases hijas implementan su dato especial mediante polimorfismo.
- `Conexion` representa la relación entre dos perfiles.
- `datos.txt` proporciona persistencia local.

## Archivo `datos.txt`

Cada perfil se guarda así:

```text
P|Tipo|ID|Nombre|Edad|GustoPrincipal|GustoSecundario|DatoEspecial
```

Cada conexión se guarda así:

```text
C|IDPrimerPerfil|IDSegundoPerfil
```

El carácter `|` funciona como separador y no puede aparecer dentro de los textos ingresados. Los cambios del frontend se guardan automáticamente; también existe un botón **Guardar ahora**.

## Conceptos de programación aplicados

- Encapsulamiento.
- Herencia y clase padre abstracta.
- Polimorfismo mediante métodos virtuales puros.
- Sobrecarga de operadores.
- Memoria dinámica y redimensionamiento de arreglos.
- Separación entre declaraciones `.h` e implementaciones `.cpp`.
- Persistencia en archivos.
- Servidor HTTP local y comunicación frontend/backend.
- Separación de responsabilidades.

## Verificación realizada

El proyecto fue comprobado mediante:

- Compilación con CMake usando C++17.
- Compilación directa con `g++ -Wall -Wextra -Wpedantic`.
- Validación de sintaxis de `frontend/app.js`.
- Prueba del servidor HTTP y carga de los archivos visuales.
- Revisión con AddressSanitizer y UndefinedBehaviorSanitizer, sin errores detectados.
- Creación de perfiles desde la API.
- Cálculo de afinidades.
- Creación de conexiones.
- Guardado correcto en `datos.txt`.
- Cierre ordenado del servidor.

## Autores

- Eduardo Tapia
- Paula Gallardo

La distribución del trabajo y la declaración detallada del uso de IA están en [`CONTRIBUTING.md`](CONTRIBUTING.md).
