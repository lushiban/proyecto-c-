#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

bool textoEsValido(const string& texto) {
    return texto.size() > 0 && texto.find('|') == string::npos;
}

string leerTextoValido(const string& mensaje) {
    string textoIngresado;
    do {
        cout << mensaje;
        getline(cin, textoIngresado);
        if (!textoEsValido(textoIngresado)) {
            cout << "Texto invalido. No puede estar vacio ni usar el simbolo |.\n";
        }
    } while (!textoEsValido(textoIngresado));
    return textoIngresado;
}

int leerEnteroEnRango(const string& mensaje, int valorMinimo, int valorMaximo) {
    int numeroIngresado;
    while (true) {
        cout << mensaje;
        cin >> numeroIngresado;
        if (!cin.fail() && numeroIngresado >= valorMinimo && numeroIngresado <= valorMaximo) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return numeroIngresado;
        }
        cout << "Opcion invalida. Intenta otra vez.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int convertirTextoAEntero(const string& texto, int valorPorDefecto) {
    stringstream convertidor(texto);
    int numeroConvertido;
    if (convertidor >> numeroConvertido) {
        return numeroConvertido;
    }
    return valorPorDefecto;
}

class Perfil {
protected:
    int idPerfil;
    string nombrePerfil;
    int edadPerfil;
    string gustoPrincipal;
    string gustoSecundario;

public:
    Perfil(int idNuevo, string nombreNuevo, int edadNueva, string gustoPrincipalNuevo, string gustoSecundarioNuevo) {
        idPerfil = idNuevo;
        setNombrePerfil(nombreNuevo);
        setEdadPerfil(edadNueva);
        setGustoPrincipal(gustoPrincipalNuevo);
        setGustoSecundario(gustoSecundarioNuevo);
    }

    virtual ~Perfil() { }

    int getIdPerfil() const { return idPerfil; }
    string getNombrePerfil() const { return nombrePerfil; }
    int getEdadPerfil() const { return edadPerfil; }
    string getGustoPrincipal() const { return gustoPrincipal; }
    string getGustoSecundario() const { return gustoSecundario; }

    void setNombrePerfil(const string& nombreNuevo) {
        if (textoEsValido(nombreNuevo)) nombrePerfil = nombreNuevo;
        else nombrePerfil = "Sin nombre";
    }

    void setEdadPerfil(int edadNueva) {
        if (edadNueva >= 13 && edadNueva <= 120) edadPerfil = edadNueva;
        else edadPerfil = 18;
    }

    void setGustoPrincipal(const string& gustoPrincipalNuevo) {
        if (textoEsValido(gustoPrincipalNuevo)) gustoPrincipal = gustoPrincipalNuevo;
        else gustoPrincipal = "pop";
    }

    void setGustoSecundario(const string& gustoSecundarioNuevo) {
        if (textoEsValido(gustoSecundarioNuevo)) gustoSecundario = gustoSecundarioNuevo;
        else gustoSecundario = "k-pop";
    }

    virtual string getTipoPerfil() const = 0;
    virtual string getDatoEspecial() const = 0;
    virtual void setDatoEspecial(const string& datoEspecialNuevo) = 0;
    virtual int calcularPuntosEspeciales(const Perfil& otroPerfil) const = 0;

    int calcularAfinidadCon(const Perfil& otroPerfil) const {
        int puntosAfinidad = 0;

        if (gustoPrincipal == otroPerfil.gustoPrincipal) puntosAfinidad += 45;
        if (gustoPrincipal == otroPerfil.gustoSecundario || gustoSecundario == otroPerfil.gustoPrincipal) puntosAfinidad += 25;
        if (gustoSecundario == otroPerfil.gustoSecundario) puntosAfinidad += 15;

        int diferenciaEdad = edadPerfil - otroPerfil.edadPerfil;
        if (diferenciaEdad < 0) diferenciaEdad = diferenciaEdad * -1;

        if (diferenciaEdad <= 2) puntosAfinidad += 10;
        else if (diferenciaEdad <= 5) puntosAfinidad += 5;

        puntosAfinidad += calcularPuntosEspeciales(otroPerfil);
        if (puntosAfinidad > 100) puntosAfinidad = 100;
        return puntosAfinidad;
    }

    string crearLineaArchivo() const {
        return "P|" + getTipoPerfil() + "|" + to_string(idPerfil) + "|" + nombrePerfil + "|" + to_string(edadPerfil) + "|" + gustoPrincipal + "|" + gustoSecundario + "|" + getDatoEspecial();
    }

    bool operator==(const Perfil& otroPerfil) const {
        return idPerfil == otroPerfil.idPerfil;
    }

    friend ostream& operator<<(ostream& salida, const Perfil& perfil) {
        salida << "ID: " << perfil.idPerfil << " | ";
        salida << "Nombre: " << perfil.nombrePerfil << " | ";
        salida << "Edad: " << perfil.edadPerfil << " | ";
        salida << "Tipo: " << perfil.getTipoPerfil() << " | ";
        salida << "Gustos: " << perfil.gustoPrincipal << ", " << perfil.gustoSecundario << " | ";
        salida << "Dato especial: " << perfil.getDatoEspecial();
        return salida;
    }
};

class PerfilOyente : public Perfil {
private:
    string cancionFavorita;

public:
    PerfilOyente(int idNuevo, string nombreNuevo, int edadNueva, string gustoPrincipalNuevo, string gustoSecundarioNuevo, string cancionFavoritaNueva)
        : Perfil(idNuevo, nombreNuevo, edadNueva, gustoPrincipalNuevo, gustoSecundarioNuevo) {
        setDatoEspecial(cancionFavoritaNueva);
    }

    string getTipoPerfil() const override { return "Oyente"; }
    string getDatoEspecial() const override { return cancionFavorita; }

    void setDatoEspecial(const string& datoEspecialNuevo) override {
        if (textoEsValido(datoEspecialNuevo)) cancionFavorita = datoEspecialNuevo;
        else cancionFavorita = "Cancion favorita no definida";
    }

    int calcularPuntosEspeciales(const Perfil& otroPerfil) const override {
        if (cancionFavorita == otroPerfil.getDatoEspecial()) return 10;
        return 0;
    }
};

class PerfilArtista : public Perfil {
private:
    string instrumentoFavorito;

public:
    PerfilArtista(int idNuevo, string nombreNuevo, int edadNueva, string gustoPrincipalNuevo, string gustoSecundarioNuevo, string instrumentoFavoritoNuevo)
        : Perfil(idNuevo, nombreNuevo, edadNueva, gustoPrincipalNuevo, gustoSecundarioNuevo) {
        setDatoEspecial(instrumentoFavoritoNuevo);
    }

    string getTipoPerfil() const override { return "Artista"; }
    string getDatoEspecial() const override { return instrumentoFavorito; }

    void setDatoEspecial(const string& datoEspecialNuevo) override {
        if (textoEsValido(datoEspecialNuevo)) instrumentoFavorito = datoEspecialNuevo;
        else instrumentoFavorito = "Instrumento no definido";
    }

    int calcularPuntosEspeciales(const Perfil& otroPerfil) const override {
        if (instrumentoFavorito == otroPerfil.getDatoEspecial()) return 10;
        return 0;
    }
};

class PerfilProductor : public Perfil {
private:
    string programaFavorito;

public:
    PerfilProductor(int idNuevo, string nombreNuevo, int edadNueva, string gustoPrincipalNuevo, string gustoSecundarioNuevo, string programaFavoritoNuevo)
        : Perfil(idNuevo, nombreNuevo, edadNueva, gustoPrincipalNuevo, gustoSecundarioNuevo) {
        setDatoEspecial(programaFavoritoNuevo);
    }

    string getTipoPerfil() const override { return "Productor"; }
    string getDatoEspecial() const override { return programaFavorito; }

    void setDatoEspecial(const string& datoEspecialNuevo) override {
        if (textoEsValido(datoEspecialNuevo)) programaFavorito = datoEspecialNuevo;
        else programaFavorito = "Programa no definido";
    }

    int calcularPuntosEspeciales(const Perfil& otroPerfil) const override {
        if (programaFavorito == otroPerfil.getDatoEspecial()) return 10;
        return 0;
    }
};

class PerfilFanClub : public Perfil {
private:
    string artistaFavorito;

public:
    PerfilFanClub(int idNuevo, string nombreNuevo, int edadNueva, string gustoPrincipalNuevo, string gustoSecundarioNuevo, string artistaFavoritoNuevo)
        : Perfil(idNuevo, nombreNuevo, edadNueva, gustoPrincipalNuevo, gustoSecundarioNuevo) {
        setDatoEspecial(artistaFavoritoNuevo);
    }

    string getTipoPerfil() const override { return "FanClub"; }
    string getDatoEspecial() const override { return artistaFavorito; }

    void setDatoEspecial(const string& datoEspecialNuevo) override {
        if (textoEsValido(datoEspecialNuevo)) artistaFavorito = datoEspecialNuevo;
        else artistaFavorito = "Artista no definido";
    }

    int calcularPuntosEspeciales(const Perfil& otroPerfil) const override {
        if (artistaFavorito == otroPerfil.getDatoEspecial()) return 10;
        return 0;
    }
};

class Conexion {
private:
    int idPrimerPerfil;
    int idSegundoPerfil;

public:
    Conexion() {
        idPrimerPerfil = 0;
        idSegundoPerfil = 0;
    }

    Conexion(int idPrimerPerfilNuevo, int idSegundoPerfilNuevo) {
        idPrimerPerfil = idPrimerPerfilNuevo;
        idSegundoPerfil = idSegundoPerfilNuevo;
    }

    int getIdPrimerPerfil() const { return idPrimerPerfil; }
    int getIdSegundoPerfil() const { return idSegundoPerfil; }
    bool contienePerfil(int idBuscado) const { return idPrimerPerfil == idBuscado || idSegundoPerfil == idBuscado; }

    string crearLineaArchivo() const {
        return "C|" + to_string(idPrimerPerfil) + "|" + to_string(idSegundoPerfil);
    }

    bool operator==(const Conexion& otraConexion) const {
        bool mismoOrden = idPrimerPerfil == otraConexion.idPrimerPerfil && idSegundoPerfil == otraConexion.idSegundoPerfil;
        bool ordenContrario = idPrimerPerfil == otraConexion.idSegundoPerfil && idSegundoPerfil == otraConexion.idPrimerPerfil;
        return mismoOrden || ordenContrario;
    }

    friend ostream& operator<<(ostream& salida, const Conexion& conexion) {
        salida << conexion.idPrimerPerfil << " <-> " << conexion.idSegundoPerfil;
        return salida;
    }
};

Perfil* crearPerfilDesdeTipo(const string& tipoPerfil, int idPerfil, const string& nombrePerfil, int edadPerfil, const string& gustoPrincipal, const string& gustoSecundario, const string& datoEspecial) {
    if (tipoPerfil == "Oyente") return new PerfilOyente(idPerfil, nombrePerfil, edadPerfil, gustoPrincipal, gustoSecundario, datoEspecial);
    if (tipoPerfil == "Artista") return new PerfilArtista(idPerfil, nombrePerfil, edadPerfil, gustoPrincipal, gustoSecundario, datoEspecial);
    if (tipoPerfil == "Productor") return new PerfilProductor(idPerfil, nombrePerfil, edadPerfil, gustoPrincipal, gustoSecundario, datoEspecial);
    if (tipoPerfil == "FanClub") return new PerfilFanClub(idPerfil, nombrePerfil, edadPerfil, gustoPrincipal, gustoSecundario, datoEspecial);
    return nullptr;
}

class RedSocialMusical {
private:
    Perfil** listaPerfiles;
    int cantidadPerfiles;
    int capacidadPerfiles;
    Conexion* listaConexiones;
    int cantidadConexiones;
    int capacidadConexiones;
    int siguienteIdPerfil;
    string nombreArchivoDatos;

    void aumentarCapacidadPerfilesSiHaceFalta() {
        if (cantidadPerfiles < capacidadPerfiles) return;

        int nuevaCapacidadPerfiles = capacidadPerfiles * 2;
        Perfil** nuevaListaPerfiles = new Perfil*[nuevaCapacidadPerfiles];

        for (int indicePerfil = 0; indicePerfil < nuevaCapacidadPerfiles; indicePerfil++) nuevaListaPerfiles[indicePerfil] = nullptr;
        for (int indicePerfil = 0; indicePerfil < cantidadPerfiles; indicePerfil++) nuevaListaPerfiles[indicePerfil] = listaPerfiles[indicePerfil];

        Perfil** listaAnteriorPerfiles = listaPerfiles;
        listaPerfiles = nuevaListaPerfiles;
        delete[] listaAnteriorPerfiles;
        listaAnteriorPerfiles = nullptr;
        capacidadPerfiles = nuevaCapacidadPerfiles;
    }

    void aumentarCapacidadConexionesSiHaceFalta() {
        if (cantidadConexiones < capacidadConexiones) return;

        int nuevaCapacidadConexiones = capacidadConexiones * 2;
        Conexion* nuevaListaConexiones = new Conexion[nuevaCapacidadConexiones];

        for (int indiceConexion = 0; indiceConexion < cantidadConexiones; indiceConexion++) nuevaListaConexiones[indiceConexion] = listaConexiones[indiceConexion];

        Conexion* listaAnteriorConexiones = listaConexiones;
        listaConexiones = nuevaListaConexiones;
        delete[] listaAnteriorConexiones;
        listaAnteriorConexiones = nullptr;
        capacidadConexiones = nuevaCapacidadConexiones;
    }

    int buscarIndicePerfilPorId(int idBuscado) const {
        for (int indicePerfil = 0; indicePerfil < cantidadPerfiles; indicePerfil++) {
            if (listaPerfiles[indicePerfil]->getIdPerfil() == idBuscado) return indicePerfil;
        }
        return -1;
    }

    Perfil* buscarPerfilPorId(int idBuscado) const {
        int indicePerfil = buscarIndicePerfilPorId(idBuscado);
        if (indicePerfil == -1) return nullptr;
        return listaPerfiles[indicePerfil];
    }

    bool existeConexion(int idPrimerPerfil, int idSegundoPerfil) const {
        Conexion conexionBuscada(idPrimerPerfil, idSegundoPerfil);
        for (int indiceConexion = 0; indiceConexion < cantidadConexiones; indiceConexion++) {
            if (listaConexiones[indiceConexion] == conexionBuscada) return true;
        }
        return false;
    }

    void agregarPerfilYaCreado(Perfil* perfilNuevo) {
        if (perfilNuevo == nullptr) return;
        aumentarCapacidadPerfilesSiHaceFalta();
        listaPerfiles[cantidadPerfiles] = perfilNuevo;
        cantidadPerfiles++;
        if (perfilNuevo->getIdPerfil() >= siguienteIdPerfil) siguienteIdPerfil = perfilNuevo->getIdPerfil() + 1;
    }

    void agregarConexionYaCreada(int idPrimerPerfil, int idSegundoPerfil) {
        if (buscarPerfilPorId(idPrimerPerfil) == nullptr || buscarPerfilPorId(idSegundoPerfil) == nullptr) return;
        if (idPrimerPerfil == idSegundoPerfil || existeConexion(idPrimerPerfil, idSegundoPerfil)) return;
        aumentarCapacidadConexionesSiHaceFalta();
        listaConexiones[cantidadConexiones] = Conexion(idPrimerPerfil, idSegundoPerfil);
        cantidadConexiones++;
    }

    void borrarConexionesDePerfil(int idPerfilEliminado) {
        int indiceConexion = 0;
        while (indiceConexion < cantidadConexiones) {
            if (listaConexiones[indiceConexion].contienePerfil(idPerfilEliminado)) {
                for (int indiceMover = indiceConexion; indiceMover < cantidadConexiones - 1; indiceMover++) listaConexiones[indiceMover] = listaConexiones[indiceMover + 1];
                cantidadConexiones--;
            } else {
                indiceConexion++;
            }
        }
    }

public:
    RedSocialMusical(string nombreArchivoDatosNuevo) {
        cantidadPerfiles = 0;
        capacidadPerfiles = 4;
        listaPerfiles = new Perfil*[capacidadPerfiles];
        for (int indicePerfil = 0; indicePerfil < capacidadPerfiles; indicePerfil++) listaPerfiles[indicePerfil] = nullptr;

        cantidadConexiones = 0;
        capacidadConexiones = 4;
        listaConexiones = new Conexion[capacidadConexiones];

        siguienteIdPerfil = 1;
        nombreArchivoDatos = nombreArchivoDatosNuevo;
    }

    ~RedSocialMusical() {
        vaciarDatos();
        delete[] listaPerfiles;
        listaPerfiles = nullptr;
        delete[] listaConexiones;
        listaConexiones = nullptr;
    }

    void vaciarDatos() {
        for (int indicePerfil = 0; indicePerfil < cantidadPerfiles; indicePerfil++) {
            delete listaPerfiles[indicePerfil];
            listaPerfiles[indicePerfil] = nullptr;
        }
        cantidadPerfiles = 0;
        cantidadConexiones = 0;
        siguienteIdPerfil = 1;
    }

    void crearPerfilDesdeMenu() {
        cout << "\nTipos de perfil:\n";
        cout << "1. Oyente\n2. Artista\n3. Productor\n4. FanClub\n";

        int opcionTipoPerfil = leerEnteroEnRango("Elige el tipo: ", 1, 4);
        string nombrePerfil = leerTextoValido("Nombre: ");
        int edadPerfil = leerEnteroEnRango("Edad: ", 13, 120);
        string gustoPrincipal = leerTextoValido("Gusto principal: ");
        string gustoSecundario = leerTextoValido("Gusto secundario: ");
        string tipoPerfil;
        string datoEspecial;

        if (opcionTipoPerfil == 1) {
            tipoPerfil = "Oyente";
            datoEspecial = leerTextoValido("Cancion favorita: ");
        } else if (opcionTipoPerfil == 2) {
            tipoPerfil = "Artista";
            datoEspecial = leerTextoValido("Instrumento favorito: ");
        } else if (opcionTipoPerfil == 3) {
            tipoPerfil = "Productor";
            datoEspecial = leerTextoValido("Programa favorito: ");
        } else {
            tipoPerfil = "FanClub";
            datoEspecial = leerTextoValido("Artista o grupo favorito: ");
        }

        Perfil* perfilNuevo = crearPerfilDesdeTipo(tipoPerfil, siguienteIdPerfil, nombrePerfil, edadPerfil, gustoPrincipal, gustoSecundario, datoEspecial);
        agregarPerfilYaCreado(perfilNuevo);
        cout << "Perfil creado con ID " << perfilNuevo->getIdPerfil() << ".\n";
    }

    void mostrarPerfiles() const {
        if (cantidadPerfiles == 0) {
            cout << "\nNo hay perfiles registrados.\n";
            return;
        }
        cout << "\nPerfiles registrados:\n";
        for (int indicePerfil = 0; indicePerfil < cantidadPerfiles; indicePerfil++) cout << *listaPerfiles[indicePerfil] << "\n";
    }

    void mostrarMejoresCoincidencias() const {
        if (cantidadPerfiles < 2) {
            cout << "\nNecesitas al menos dos perfiles.\n";
            return;
        }

        int idPerfilBase = leerEnteroEnRango("ID del perfil que busca coincidencias: ", 1, 1000000);
        Perfil* perfilBase = buscarPerfilPorId(idPerfilBase);

        if (perfilBase == nullptr) {
            cout << "No existe un perfil con ese ID.\n";
            return;
        }

        cout << "\nCoincidencias para " << perfilBase->getNombrePerfil() << ":\n";
        for (int indicePerfil = 0; indicePerfil < cantidadPerfiles; indicePerfil++) {
            Perfil* perfilCandidato = listaPerfiles[indicePerfil];
            if (perfilCandidato->getIdPerfil() != idPerfilBase) {
                int porcentajeAfinidad = perfilBase->calcularAfinidadCon(*perfilCandidato);
                cout << porcentajeAfinidad << "% -> " << *perfilCandidato << "\n";
            }
        }
    }

    void conectarPerfilesDesdeMenu() {
        if (cantidadPerfiles < 2) {
            cout << "\nNecesitas al menos dos perfiles para conectar.\n";
            return;
        }

        int idPrimerPerfil = leerEnteroEnRango("ID del primer perfil: ", 1, 1000000);
        int idSegundoPerfil = leerEnteroEnRango("ID del segundo perfil: ", 1, 1000000);

        if (idPrimerPerfil == idSegundoPerfil) {
            cout << "Un perfil no puede conectarse consigo mismo.\n";
            return;
        }

        Perfil* primerPerfil = buscarPerfilPorId(idPrimerPerfil);
        Perfil* segundoPerfil = buscarPerfilPorId(idSegundoPerfil);

        if (primerPerfil == nullptr || segundoPerfil == nullptr) {
            cout << "Uno de los perfiles no existe.\n";
            return;
        }

        if (existeConexion(idPrimerPerfil, idSegundoPerfil)) {
            cout << "Esos perfiles ya estan conectados.\n";
            return;
        }

        agregarConexionYaCreada(idPrimerPerfil, idSegundoPerfil);
        int porcentajeAfinidad = primerPerfil->calcularAfinidadCon(*segundoPerfil);
        cout << primerPerfil->getNombrePerfil() << " y " << segundoPerfil->getNombrePerfil() << " ahora estan conectados.\n";
        cout << "Afinidad calculada: " << porcentajeAfinidad << "%\n";
    }

    void mostrarConexiones() const {
        if (cantidadConexiones == 0) {
            cout << "\nNo hay conexiones guardadas.\n";
            return;
        }

        cout << "\nConexiones guardadas:\n";
        for (int indiceConexion = 0; indiceConexion < cantidadConexiones; indiceConexion++) {
            int idPrimerPerfil = listaConexiones[indiceConexion].getIdPrimerPerfil();
            int idSegundoPerfil = listaConexiones[indiceConexion].getIdSegundoPerfil();
            Perfil* primerPerfil = buscarPerfilPorId(idPrimerPerfil);
            Perfil* segundoPerfil = buscarPerfilPorId(idSegundoPerfil);
            if (primerPerfil != nullptr && segundoPerfil != nullptr) cout << primerPerfil->getNombrePerfil() << " <-> " << segundoPerfil->getNombrePerfil() << "\n";
        }
    }

    void eliminarPerfilDesdeMenu() {
        if (cantidadPerfiles == 0) {
            cout << "\nNo hay perfiles para eliminar.\n";
            return;
        }

        int idPerfilEliminado = leerEnteroEnRango("ID del perfil que deseas eliminar: ", 1, 1000000);
        int indicePerfilEliminado = buscarIndicePerfilPorId(idPerfilEliminado);

        if (indicePerfilEliminado == -1) {
            cout << "No existe un perfil con ese ID.\n";
            return;
        }

        delete listaPerfiles[indicePerfilEliminado];
        listaPerfiles[indicePerfilEliminado] = nullptr;

        for (int indiceMover = indicePerfilEliminado; indiceMover < cantidadPerfiles - 1; indiceMover++) listaPerfiles[indiceMover] = listaPerfiles[indiceMover + 1];
        cantidadPerfiles--;
        listaPerfiles[cantidadPerfiles] = nullptr;
        borrarConexionesDePerfil(idPerfilEliminado);
        cout << "Perfil eliminado.\n";
    }

    void guardarDatosEnArchivo() const {
        ofstream archivoSalida(nombreArchivoDatos);

        if (!archivoSalida.is_open()) {
            cout << "No se pudo abrir el archivo para guardar.\n";
            return;
        }

        for (int indicePerfil = 0; indicePerfil < cantidadPerfiles; indicePerfil++) archivoSalida << listaPerfiles[indicePerfil]->crearLineaArchivo() << "\n";
        for (int indiceConexion = 0; indiceConexion < cantidadConexiones; indiceConexion++) archivoSalida << listaConexiones[indiceConexion].crearLineaArchivo() << "\n";

        archivoSalida.close();
        cout << "Datos guardados en " << nombreArchivoDatos << ".\n";
    }

    void cargarDatosDesdeArchivo() {
        ifstream archivoEntrada(nombreArchivoDatos);

        if (!archivoEntrada.is_open()) {
            cout << "No existe archivo de datos. Se iniciara una red vacia.\n";
            return;
        }

        vaciarDatos();
        string lineaArchivo;

        while (getline(archivoEntrada, lineaArchivo)) {
            stringstream separador(lineaArchivo);
            string etiquetaLinea;
            getline(separador, etiquetaLinea, '|');

            if (etiquetaLinea == "P") {
                string tipoPerfil, textoIdPerfil, nombrePerfil, textoEdadPerfil, gustoPrincipal, gustoSecundario, datoEspecial;
                getline(separador, tipoPerfil, '|');
                getline(separador, textoIdPerfil, '|');
                getline(separador, nombrePerfil, '|');
                getline(separador, textoEdadPerfil, '|');
                getline(separador, gustoPrincipal, '|');
                getline(separador, gustoSecundario, '|');
                getline(separador, datoEspecial, '|');

                int idPerfil = convertirTextoAEntero(textoIdPerfil, 0);
                int edadPerfil = convertirTextoAEntero(textoEdadPerfil, 18);
                Perfil* perfilCargado = crearPerfilDesdeTipo(tipoPerfil, idPerfil, nombrePerfil, edadPerfil, gustoPrincipal, gustoSecundario, datoEspecial);
                agregarPerfilYaCreado(perfilCargado);
            } else if (etiquetaLinea == "C") {
                string textoIdPrimerPerfil, textoIdSegundoPerfil;
                getline(separador, textoIdPrimerPerfil, '|');
                getline(separador, textoIdSegundoPerfil, '|');

                int idPrimerPerfil = convertirTextoAEntero(textoIdPrimerPerfil, 0);
                int idSegundoPerfil = convertirTextoAEntero(textoIdSegundoPerfil, 0);
                agregarConexionYaCreada(idPrimerPerfil, idSegundoPerfil);
            }
        }

        archivoEntrada.close();
        cout << "Datos cargados desde " << nombreArchivoDatos << ".\n";
    }
};

void mostrarMenuPrincipal() {
    cout << "\n==== EQUALS MUSIC ====\n";
    cout << "1. Crear perfil\n";
    cout << "2. Ver perfiles\n";
    cout << "3. Ver mejores coincidencias\n";
    cout << "4. Conectar dos perfiles\n";
    cout << "5. Ver conexiones\n";
    cout << "6. Eliminar perfil\n";
    cout << "7. Guardar datos\n";
    cout << "8. Cargar datos\n";
    cout << "0. Salir\n";
}

int main() {
    RedSocialMusical redSocialMusical("datos.txt");
    redSocialMusical.cargarDatosDesdeArchivo();
    int opcionMenu = -1;

    do {
        mostrarMenuPrincipal();
        opcionMenu = leerEnteroEnRango("Elige una opcion: ", 0, 8);

        if (opcionMenu == 1) redSocialMusical.crearPerfilDesdeMenu();
        else if (opcionMenu == 2) redSocialMusical.mostrarPerfiles();
        else if (opcionMenu == 3) redSocialMusical.mostrarMejoresCoincidencias();
        else if (opcionMenu == 4) redSocialMusical.conectarPerfilesDesdeMenu();
        else if (opcionMenu == 5) redSocialMusical.mostrarConexiones();
        else if (opcionMenu == 6) redSocialMusical.eliminarPerfilDesdeMenu();
        else if (opcionMenu == 7) redSocialMusical.guardarDatosEnArchivo();
        else if (opcionMenu == 8) redSocialMusical.cargarDatosDesdeArchivo();
    } while (opcionMenu != 0);

    redSocialMusical.guardarDatosEnArchivo();
    cout << "Gracias por usar Equals Music.\n";
    return 0;
}
