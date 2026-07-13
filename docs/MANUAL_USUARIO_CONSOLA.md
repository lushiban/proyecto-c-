# Manual de usuario de la consola

## 1. Propósito

La consola es una demostración temporal del núcleo. Permite utilizar todas las
funciones principales mientras se desarrolla una nueva interfaz visual.

## 2. Inicio

Ejecute:

```bash
./build/red_social_musical
```

El programa intenta cargar `data/datos.txt`. Si no existe, inicia con una red
vacía. Si el archivo es inválido, informa el error y conserva el estado actual.

## 3. Menú

```text
1. Crear perfil
2. Mostrar perfiles
3. Buscar perfil por ID
4. Buscar perfiles compatibles
5. Conectar perfiles
6. Mostrar conexiones
7. Eliminar perfil
8. Eliminar conexión
9. Mostrar gráfica de perfiles por género
10. Mostrar gráfica de perfiles por tipo
11. Mostrar gráfica de conexiones por afinidad
12. Guardar datos
0. Salir
```

## 4. Crear un perfil

Seleccione el tipo e ingrese los datos solicitados.

Datos comunes:

- Nombre no vacío.
- Edad entre 13 y 120.
- Género principal no vacío.
- Al menos un género secundario.
- Géneros secundarios sin duplicados.
- El género principal no puede repetirse como secundario.

Datos específicos:

- Oyente: horas de escucha entre 0 y 168 y plataforma no vacía.
- Artista: nombre artístico e instrumento no vacíos.
- Productor: software no vacío y experiencia entre 0 y 80 años.
- Fan Club: artista, al menos un miembro y ciudad no vacía.

Después de una creación correcta, la consola guarda automáticamente.

## 5. Buscar y listar

La opción 2 muestra todos los perfiles. La opción 3 solicita un ID positivo y
muestra los datos comunes y específicos del perfil encontrado.

## 6. Compatibilidad y conexiones

La opción 4 muestra perfiles con afinidad mínima de 40. Excluye el mismo perfil
y conexiones existentes.

La opción 5 crea una conexión cuando:

- Ambos perfiles existen.
- Los IDs son diferentes.
- La conexión no existe.
- La afinidad es igual o superior a 40.

## 7. Eliminaciones

La eliminación de perfil solicita confirmación y elimina también sus conexiones.
La eliminación de conexión conserva los perfiles.

## 8. Estadísticas

Las opciones 9, 10 y 11 preparan los datos reales. Como no hay un front-end
visual seleccionado, se muestra:

```text
El soporte grafico no esta habilitado.
```

## 9. Guardado y cierre

La opción 12 guarda manualmente. Las creaciones y eliminaciones correctas también
se guardan automáticamente. La opción 0 cierra el programa.
