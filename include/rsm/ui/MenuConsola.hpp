#ifndef RSM_UI_MENU_CONSOLA_HPP
#define RSM_UI_MENU_CONSOLA_HPP

namespace rsm {

class ControladorAplicacion;
class IVisualizadorGraficas;
struct PerfilDTO;

class MenuConsola {
private:
    ControladorAplicacion& controlador_;
    IVisualizadorGraficas& visualizador_;

    void mostrarMenuPrincipal() const;
    void mostrarPerfil(const PerfilDTO& perfil) const;
    void crearPerfil();
    void mostrarPerfiles() const;
    void buscarPerfilPorId() const;
    void buscarPerfilesCompatibles() const;
    void conectarPerfiles();
    void mostrarConexiones() const;
    void eliminarPerfil();
    void eliminarConexion();
    void mostrarGraficaPerfilesPorGenero();
    void mostrarGraficaPerfilesPorTipo();
    void mostrarGraficaConexionesPorAfinidad();
    void guardarDatos() const;
    void guardarAutomaticamente() const;

public:
    MenuConsola(
        ControladorAplicacion& controlador,
        IVisualizadorGraficas& visualizador
    );

    void ejecutar();
};

}

#endif
