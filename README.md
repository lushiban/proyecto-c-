# SoundBridge

**SoundBridge** es una aplicación de consola escrita en C++ que administra perfiles musicales, calcula afinidad entre usuarios y permite guardar conexiones. El proyecto fue reorganizado a partir de un único archivo para separar responsabilidades, facilitar su lectura y permitir que cada módulo se compile de forma independiente.

## Funcionalidades

- Crear perfiles de tipo **Oyente**, **Artista**, **Productor** o **FanClub**.
- Mostrar los perfiles registrados.
- Calcular el porcentaje de afinidad entre perfiles.
- Conectar dos perfiles y consultar las conexiones existentes.
- Eliminar perfiles junto con sus conexiones.
- Guardar y cargar la información desde `datos.txt`.

## Requisitos

- Compilador compatible con **C++17**.
- CMake 3.16 o superior para el método recomendado de compilación.

## Estructura del repositorio

```text
SoundBridge/
├── docs/
│   └── DISECCION_CODIGO.md
├── ejemplos/
│   └── datos_ejemplo.txt
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
│   └── Utilidades.cpp
├── .gitignore
├── CMakeLists.txt
├── CONTRIBUTING.md
├── LICENSE
├── PRUEBAS.md
├── README.md
└── main.cpp
```

Las declaraciones de clases y funciones están en archivos `.h`. Sus implementaciones están en archivos `.cpp`. Así, cada archivo tiene una responsabilidad principal.

## Punto de entrada y flujo general

`main.cpp` contiene la función `main()`, pero no concentra la lógica del programa. Solo crea un objeto `AplicacionSoundBridge` y llama a `ejecutar()`.

```text
Sistema operativo
        ↓
      main()
        ↓
AplicacionSoundBridge::ejecutar()
        ↓
  ciclo principal del menú
        ↓
   RedSocialMusical
        ↓
Perfiles, conexiones y archivos
```

El recorrido detallado de las funciones se encuentra en [`docs/DISECCION_CODIGO.md`](docs/DISECCION_CODIGO.md).

## Compilación con CMake

Desde la carpeta raíz del proyecto:

```bash
cmake -S . -B build
cmake --build build
```

En Linux o macOS, el ejecutable normalmente se inicia así:

```bash
./build/soundbridge
```

En Windows con un generador de CMake de configuración única, normalmente se usa:

```powershell
.\build\soundbridge.exe
```

Con Visual Studio, el ejecutable puede quedar dentro de `build/Debug/` o `build/Release/`, según la configuración seleccionada.

## Compilación directa con g++

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp src/*.cpp -Iinclude -o soundbridge
```

Ejecución:

```bash
./soundbridge
```

En Windows:

```powershell
.\soundbridge.exe
```

## Archivo de datos

El programa crea `datos.txt` en la carpeta desde la que se ejecuta. Cada perfil se guarda con este formato:

```text
P|Tipo|ID|Nombre|Edad|GustoPrincipal|GustoSecundario|DatoEspecial
```

Cada conexión se guarda así:

```text
C|IDPrimerPerfil|IDSegundoPerfil
```

El carácter `|` se utiliza como separador, por eso no se permite dentro de los textos ingresados por el usuario. El archivo `ejemplos/datos_ejemplo.txt` puede copiarse como `datos.txt` para realizar una prueba rápida.

## Conceptos de programación aplicados

- **Encapsulamiento:** los atributos se mantienen privados o protegidos y se accede a ellos mediante métodos.
- **Herencia:** `PerfilOyente`, `PerfilArtista`, `PerfilProductor` y `PerfilFanClub` heredan de `Perfil`.
- **Polimorfismo:** la clase padre define métodos virtuales puros y cada clase hija implementa su comportamiento.
- **Sobrecarga de operadores:** `Perfil` y `Conexion` implementan comparaciones y salida con `operator<<`.
- **Memoria dinámica:** `RedSocialMusical` administra arreglos redimensionables y libera los perfiles creados con `new`.
- **Persistencia:** los perfiles y las conexiones se guardan en un archivo de texto.
- **Separación de responsabilidades:** entrada, aplicación, modelo, conexiones y almacenamiento se encuentran en módulos diferentes.

## Autores

- Eduardo Tapia
- Paula Gallardo

La distribución detallada del trabajo y el proceso recomendado para contribuir están en [`CONTRIBUTING.md`](CONTRIBUTING.md).

## Licencia

Este proyecto se distribuye bajo la licencia MIT incluida en [`LICENSE`](LICENSE).
