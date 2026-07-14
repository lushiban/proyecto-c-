/*
 * Frontend de SoundBridge.
 * Diseño e implementación inicial generados con asistencia de IA (OpenAI),
 * conectados al backend C++ mediante la API HTTP local de ServidorWeb.
 */

'use strict';

const estado = {
    perfiles: [],
    conexiones: [],
    estadisticas: {
        perfiles: 0,
        conexiones: 0,
        oyentes: 0,
        artistas: 0,
        productores: 0,
        fanClub: 0
    }
};

let perfilAfinidadActual = null;

const titulosSeccion = {
    inicio: ['TU ESPACIO MUSICAL', 'Hola, bienvenida/o a SoundBridge'],
    perfiles: ['COMUNIDAD', 'Personas que comparten tu ritmo'],
    conexiones: ['PUENTES MUSICALES', 'Conecta dos frecuencias'],
    afinidades: ['COMPATIBILIDAD', 'Descubre mejores coincidencias'],
    datos: ['CONFIGURACIÓN', 'Datos y aplicación']
};

const configuracionTipos = {
    Oyente: {
        detalle: 'Plataforma preferida',
        icono: '♪'
    },
    Artista: {
        detalle: 'Instrumento principal',
        icono: '♬'
    },
    Productor: {
        detalle: 'Software principal',
        icono: '◉'
    },
    FanClub: {
        detalle: 'Artista representado',
        icono: '♥'
    }
};

const elementos = {
    pageEyebrow: document.querySelector('#pageEyebrow'),
    pageTitle: document.querySelector('#pageTitle'),
    serverStatus: document.querySelector('#serverStatus'),
    navProfileCount: document.querySelector('#navProfileCount'),
    navConnectionCount: document.querySelector('#navConnectionCount'),
    statProfiles: document.querySelector('#statProfiles'),
    statConnections: document.querySelector('#statConnections'),
    statArtists: document.querySelector('#statArtists'),
    statFanClubs: document.querySelector('#statFanClubs'),
    recentProfiles: document.querySelector('#recentProfiles'),
    profileGrid: document.querySelector('#profileGrid'),
    profileSearch: document.querySelector('#profileSearch'),
    firstProfileSelect: document.querySelector('#firstProfileSelect'),
    secondProfileSelect: document.querySelector('#secondProfileSelect'),
    connectionForm: document.querySelector('#connectionForm'),
    connectButton: document.querySelector('#connectButton'),
    connectionList: document.querySelector('#connectionList'),
    connectionBubble: document.querySelector('#connectionBubble'),
    affinityProfileSelect: document.querySelector('#affinityProfileSelect'),
    affinitySummary: document.querySelector('#affinitySummary'),
    affinityList: document.querySelector('#affinityList'),
    profileDialog: document.querySelector('#profileDialog'),
    profileForm: document.querySelector('#profileForm'),
    profileType: document.querySelector('#profileType'),
    specialFields: document.querySelector('#specialFields'),
    toastRegion: document.querySelector('#toastRegion'),
    shutdownScreen: document.querySelector('#shutdownScreen')
};

function escaparHtml(valor) {
    return String(valor ?? '')
        .replaceAll('&', '&amp;')
        .replaceAll('<', '&lt;')
        .replaceAll('>', '&gt;')
        .replaceAll('"', '&quot;')
        .replaceAll("'", '&#039;');
}

function obtenerIniciales(nombre) {
    const palabras = String(nombre ?? '')
        .trim()
        .split(/\s+/)
        .filter(Boolean);

    if (palabras.length === 0) return 'SB';
    if (palabras.length === 1) return palabras[0].slice(0, 2).toUpperCase();
    return `${palabras[0][0]}${palabras[palabras.length - 1][0]}`.toUpperCase();
}

function nombreTipo(tipo) {
    return tipo === 'FanClub' ? 'Fan club' : tipo;
}

function parametrosDeFormulario(formulario) {
    const parametros = new URLSearchParams();
    for (const [clave, valor] of new FormData(formulario).entries()) {
        parametros.append(clave, String(valor));
    }
    return parametros;
}

function marcarServidor(conectado) {
    elementos.serverStatus.classList.toggle('offline', !conectado);
    elementos.serverStatus.innerHTML = conectado
        ? '<span class="status-dot" aria-hidden="true"></span>Backend conectado'
        : '<span class="status-dot" aria-hidden="true"></span>Backend desconectado';
}

async function solicitar(ruta, opciones = {}) {
    try {
        const respuesta = await fetch(ruta, opciones);
        const texto = await respuesta.text();
        let datos = {};

        try {
            datos = texto ? JSON.parse(texto) : {};
        } catch {
            throw new Error('El servidor devolvió una respuesta que no se pudo interpretar.');
        }

        if (!respuesta.ok || datos.ok === false) {
            throw new Error(datos.mensaje || `Error HTTP ${respuesta.status}`);
        }

        marcarServidor(true);
        return datos;
    } catch (error) {
        if (error instanceof TypeError) {
            marcarServidor(false);
            throw new Error('No se pudo conectar con el backend C++. Verifica que SoundBridge siga abierto.');
        }
        throw error;
    }
}

function mostrarToast(mensaje, tipo = 'success') {
    const toast = document.createElement('div');
    toast.className = `toast ${tipo === 'error' ? 'error' : ''}`;
    toast.innerHTML = `
        <span class="toast-icon" aria-hidden="true">${tipo === 'error' ? '!' : '✓'}</span>
        <p>${escaparHtml(mensaje)}</p>
        <button type="button" aria-label="Cerrar notificación">×</button>
    `;

    const eliminar = () => toast.remove();
    toast.querySelector('button').addEventListener('click', eliminar);
    elementos.toastRegion.appendChild(toast);
    window.setTimeout(eliminar, 4700);
}

function cambiarSeccion(nombreSeccion) {
    const nombreValido = titulosSeccion[nombreSeccion] ? nombreSeccion : 'inicio';

    document.querySelectorAll('.page').forEach((pagina) => {
        pagina.classList.toggle('active', pagina.id === `section-${nombreValido}`);
    });

    document.querySelectorAll('.nav-item').forEach((boton) => {
        boton.classList.toggle('active', boton.dataset.section === nombreValido);
    });

    const [eyebrow, titulo] = titulosSeccion[nombreValido];
    elementos.pageEyebrow.textContent = eyebrow;
    elementos.pageTitle.textContent = titulo;
    history.replaceState(null, '', `#${nombreValido}`);
    window.scrollTo({ top: 0, behavior: 'smooth' });
}

function renderizarEstadisticas() {
    const estadisticas = estado.estadisticas;
    elementos.navProfileCount.textContent = estadisticas.perfiles;
    elementos.navConnectionCount.textContent = estadisticas.conexiones;
    elementos.statProfiles.textContent = estadisticas.perfiles;
    elementos.statConnections.textContent = estadisticas.conexiones;
    elementos.statArtists.textContent = estadisticas.artistas;
    elementos.statFanClubs.textContent = estadisticas.fanClub;
    elementos.connectionBubble.textContent = estadisticas.conexiones;
}

function contenidoVacio(titulo, mensaje, mostrarBoton = false) {
    return `
        <div class="empty-state">
            <div>
                <span class="empty-state-icon" aria-hidden="true">♫</span>
                <h3>${escaparHtml(titulo)}</h3>
                <p>${escaparHtml(mensaje)}</p>
                ${mostrarBoton ? '<button class="button button-primary" type="button" data-create-profile>Crear perfil</button>' : ''}
            </div>
        </div>
    `;
}

function renderizarPerfilesRecientes() {
    if (estado.perfiles.length === 0) {
        elementos.recentProfiles.innerHTML = `
            <div class="mini-profile">
                <span class="avatar">♫</span>
                <div><strong>Aún no hay perfiles</strong><small>Crea el primero para comenzar.</small></div>
                <span class="type-chip">nuevo</span>
            </div>
        `;
        return;
    }

    elementos.recentProfiles.innerHTML = estado.perfiles
        .slice(-4)
        .reverse()
        .map((perfil) => `
            <div class="mini-profile">
                <span class="avatar ${escaparHtml(perfil.tipo)}">${escaparHtml(obtenerIniciales(perfil.nombre))}</span>
                <div>
                    <strong>${escaparHtml(perfil.nombre)}</strong>
                    <small>${escaparHtml(perfil.gustoPrincipal)} · ${perfil.edad} años</small>
                </div>
                <span class="type-chip ${escaparHtml(perfil.tipo)}">${escaparHtml(nombreTipo(perfil.tipo))}</span>
            </div>
        `)
        .join('');
}

function tarjetaPerfil(perfil) {
    const configuracion = configuracionTipos[perfil.tipo] ?? configuracionTipos.Oyente;

    return `
        <article class="profile-card ${escaparHtml(perfil.tipo)}">
            <div class="profile-card-body">
                <div class="profile-card-top">
                    <span class="avatar ${escaparHtml(perfil.tipo)}">${escaparHtml(obtenerIniciales(perfil.nombre))}</span>
                    <button class="profile-menu-button" type="button" data-delete-profile="${perfil.id}" title="Eliminar perfil" aria-label="Eliminar a ${escaparHtml(perfil.nombre)}">×</button>
                </div>
                <h3 title="${escaparHtml(perfil.nombre)}">${escaparHtml(perfil.nombre)}</h3>
                <div class="profile-meta">
                    <span class="type-chip ${escaparHtml(perfil.tipo)}">${escaparHtml(nombreTipo(perfil.tipo))}</span>
                    <span>ID ${perfil.id}</span>
                    <span>·</span>
                    <span>${perfil.edad} años</span>
                </div>
                <div class="profile-genres">
                    <span class="genre-chip">${escaparHtml(perfil.gustoPrincipal)}</span>
                    <span class="genre-chip">${escaparHtml(perfil.gustoSecundario)}</span>
                </div>
                <div class="special-detail">
                    <span aria-hidden="true">${configuracion.icono}</span>
                    <div>
                        <small>${escaparHtml(configuracion.detalle)}</small>
                        <strong title="${escaparHtml(perfil.datoEspecial)}">${escaparHtml(perfil.datoEspecial)}</strong>
                    </div>
                </div>
                <div class="profile-card-actions">
                    <button class="button button-secondary" type="button" data-match-profile="${perfil.id}">Ver afinidades</button>
                </div>
            </div>
        </article>
    `;
}

function renderizarPerfiles() {
    const busqueda = elementos.profileSearch.value.trim().toLocaleLowerCase('es');
    const perfilesFiltrados = estado.perfiles.filter((perfil) => {
        const texto = [
            perfil.nombre,
            perfil.tipo,
            perfil.gustoPrincipal,
            perfil.gustoSecundario,
            perfil.datoEspecial
        ].join(' ').toLocaleLowerCase('es');
        return texto.includes(busqueda);
    });

    if (estado.perfiles.length === 0) {
        elementos.profileGrid.innerHTML = contenidoVacio(
            'Tu comunidad está esperando',
            'Crea el primer perfil para comenzar a calcular afinidades y conexiones.',
            true
        );
        return;
    }

    if (perfilesFiltrados.length === 0) {
        elementos.profileGrid.innerHTML = contenidoVacio(
            'No encontramos coincidencias',
            'Prueba con otro nombre, tipo de perfil o género musical.'
        );
        return;
    }

    elementos.profileGrid.innerHTML = perfilesFiltrados.map(tarjetaPerfil).join('');
}

function opcionesPerfiles(incluirPlaceholder = true) {
    const placeholder = incluirPlaceholder
        ? '<option value="">Selecciona un perfil</option>'
        : '';
    return placeholder + estado.perfiles.map((perfil) => `
        <option value="${perfil.id}">${escaparHtml(perfil.nombre)} · ${escaparHtml(nombreTipo(perfil.tipo))}</option>
    `).join('');
}

function renderizarSelectores() {
    const valorPrimero = elementos.firstProfileSelect.value;
    const valorSegundo = elementos.secondProfileSelect.value;
    const valorAfinidad = elementos.affinityProfileSelect.value;

    elementos.firstProfileSelect.innerHTML = opcionesPerfiles();
    elementos.secondProfileSelect.innerHTML = opcionesPerfiles();
    elementos.affinityProfileSelect.innerHTML = opcionesPerfiles();

    const ids = estado.perfiles.map((perfil) => String(perfil.id));

    if (ids.includes(valorPrimero)) {
        elementos.firstProfileSelect.value = valorPrimero;
    } else if (ids.length > 0) {
        elementos.firstProfileSelect.value = ids[0];
    }

    if (ids.includes(valorSegundo) && valorSegundo !== elementos.firstProfileSelect.value) {
        elementos.secondProfileSelect.value = valorSegundo;
    } else if (ids.length > 1) {
        elementos.secondProfileSelect.value = ids.find((id) => id !== elementos.firstProfileSelect.value) ?? '';
    }

    if (ids.includes(valorAfinidad)) {
        elementos.affinityProfileSelect.value = valorAfinidad;
    } else if (perfilAfinidadActual && ids.includes(String(perfilAfinidadActual))) {
        elementos.affinityProfileSelect.value = String(perfilAfinidadActual);
    }

    elementos.connectButton.disabled = estado.perfiles.length < 2;
}

function renderizarConexiones() {
    if (estado.conexiones.length === 0) {
        elementos.connectionList.innerHTML = contenidoVacio(
            'Todavía no hay conexiones',
            'Selecciona dos perfiles para crear el primer puente musical.'
        );
        return;
    }

    const perfilesPorId = new Map(estado.perfiles.map((perfil) => [perfil.id, perfil]));

    elementos.connectionList.innerHTML = estado.conexiones
        .slice()
        .reverse()
        .map((conexion) => {
            const primero = perfilesPorId.get(conexion.idPrimerPerfil);
            const segundo = perfilesPorId.get(conexion.idSegundoPerfil);
            return `
                <div class="connection-row">
                    <div class="connection-person">
                        <span class="avatar ${escaparHtml(primero?.tipo ?? '')}">${escaparHtml(obtenerIniciales(conexion.nombrePrimerPerfil))}</span>
                        <strong title="${escaparHtml(conexion.nombrePrimerPerfil)}">${escaparHtml(conexion.nombrePrimerPerfil)}</strong>
                    </div>
                    <span class="connection-heart" aria-hidden="true">♡</span>
                    <div class="connection-person">
                        <span class="avatar ${escaparHtml(segundo?.tipo ?? '')}">${escaparHtml(obtenerIniciales(conexion.nombreSegundoPerfil))}</span>
                        <strong title="${escaparHtml(conexion.nombreSegundoPerfil)}">${escaparHtml(conexion.nombreSegundoPerfil)}</strong>
                    </div>
                    <span class="affinity-pill">${conexion.afinidad}%</span>
                </div>
            `;
        })
        .join('');
}

function renderizarTodo() {
    renderizarEstadisticas();
    renderizarPerfilesRecientes();
    renderizarPerfiles();
    renderizarSelectores();
    renderizarConexiones();

    if (perfilAfinidadActual && !estado.perfiles.some((perfil) => perfil.id === perfilAfinidadActual)) {
        perfilAfinidadActual = null;
        mostrarAfinidadInicial();
    }
}

async function cargarEstado(mostrarError = true) {
    try {
        const respuesta = await solicitar('/api/estado');
        const datos = respuesta.datos ?? {};
        estado.perfiles = Array.isArray(datos.perfiles) ? datos.perfiles : [];
        estado.conexiones = Array.isArray(datos.conexiones) ? datos.conexiones : [];
        estado.estadisticas = {
            ...estado.estadisticas,
            ...(datos.estadisticas ?? {})
        };
        renderizarTodo();

        if (perfilAfinidadActual &&
            estado.perfiles.some((perfil) => perfil.id === perfilAfinidadActual)) {
            await cargarAfinidades(perfilAfinidadActual);
        }

        return true;
    } catch (error) {
        if (mostrarError) mostrarToast(error.message, 'error');
        return false;
    }
}

function abrirDialogoPerfil() {
    if (typeof elementos.profileDialog.showModal === 'function') {
        elementos.profileDialog.showModal();
    } else {
        elementos.profileDialog.setAttribute('open', '');
    }
    window.setTimeout(() => elementos.profileForm.querySelector('[name="nombre"]')?.focus(), 80);
}

function cerrarDialogoPerfil() {
    if (typeof elementos.profileDialog.close === 'function') {
        elementos.profileDialog.close();
    } else {
        elementos.profileDialog.removeAttribute('open');
    }
}

function actualizarCamposEspeciales() {
    const tipo = elementos.profileType.value;

    if (tipo === 'Oyente') {
        elementos.specialFields.innerHTML = `
            <label>
                Horas de escucha semanales
                <input type="number" name="horasEscuchaSemanales" min="0" max="168" value="10" required>
            </label>
            <label>
                Plataforma preferida
                <input type="text" name="plataformaPreferida" maxlength="80" placeholder="Ej. Spotify" required>
            </label>
        `;
    } else if (tipo === 'Artista') {
        elementos.specialFields.innerHTML = `
            <label>
                Nombre artístico
                <input type="text" name="nombreArtistico" maxlength="80" placeholder="Ej. Luna Azul" required>
            </label>
            <label>
                Instrumento principal
                <input type="text" name="instrumentoPrincipal" maxlength="80" placeholder="Ej. Guitarra" required>
            </label>
            <label>
                ¿Busca colaboraciones?
                <select name="buscaColaboraciones" required>
                    <option value="true">Sí</option>
                    <option value="false">No</option>
                </select>
            </label>
        `;
    } else if (tipo === 'Productor') {
        elementos.specialFields.innerHTML = `
            <label>
                Software principal
                <input type="text" name="softwarePrincipal" maxlength="80" placeholder="Ej. Ableton Live" required>
            </label>
            <label>
                Años de experiencia
                <input type="number" name="aniosExperiencia" min="0" max="80" value="1" required>
            </label>
            <label>
                ¿Ofrece servicios?
                <select name="ofreceServicios" required>
                    <option value="true">Sí</option>
                    <option value="false">No</option>
                </select>
            </label>
        `;
    } else {
        elementos.specialFields.innerHTML = `
            <label>
                Artista representado
                <input type="text" name="artistaRepresentado" maxlength="80" placeholder="Ej. The Marías" required>
            </label>
            <label>
                Número de miembros
                <input type="number" name="numeroMiembros" min="1" value="10" required>
            </label>
            <label>
                Ciudad base
                <input type="text" name="ciudadBase" maxlength="80" placeholder="Ej. Quito" required>
            </label>
        `;
    }
}

function mostrarAfinidadInicial() {
    elementos.affinitySummary.innerHTML = `
        <div class="affinity-placeholder">
            <span aria-hidden="true">✦</span>
            <h3>Selecciona un perfil</h3>
            <p>Aquí aparecerán sus mejores coincidencias.</p>
        </div>
    `;
    elementos.affinityList.innerHTML = '';
}

function renderizarAfinidades(datos) {
    const perfilBase = estado.perfiles.find((perfil) => perfil.id === datos.perfilBase.id);
    const coincidencias = Array.isArray(datos.coincidencias) ? datos.coincidencias : [];

    elementos.affinitySummary.innerHTML = `
        <div class="affinity-summary-content">
            <div>
                <span class="avatar ${escaparHtml(perfilBase?.tipo ?? '')}">${escaparHtml(obtenerIniciales(datos.perfilBase.nombre))}</span>
                <h3>${escaparHtml(datos.perfilBase.nombre)}</h3>
                <p>${coincidencias.length} coincidencia${coincidencias.length === 1 ? '' : 's'} ordenada${coincidencias.length === 1 ? '' : 's'} por afinidad.</p>
            </div>
            <span class="affinity-decoration" aria-hidden="true">♫</span>
        </div>
    `;

    if (coincidencias.length === 0) {
        elementos.affinityList.innerHTML = contenidoVacio(
            'Necesitamos más perfiles',
            'Agrega otra persona para poder comparar sus gustos musicales.'
        );
        return;
    }

    elementos.affinityList.innerHTML = coincidencias.map((coincidencia, indice) => `
        <article class="affinity-row">
            <span class="affinity-rank">${String(indice + 1).padStart(2, '0')}</span>
            <div class="affinity-person">
                <strong>${escaparHtml(coincidencia.nombre)}</strong>
                <small>${escaparHtml(nombreTipo(coincidencia.tipo))} · ${escaparHtml(coincidencia.gustoPrincipal)}</small>
            </div>
            <div class="affinity-track" aria-label="Afinidad ${coincidencia.afinidad}%">
                <span style="width:${Math.max(0, Math.min(100, coincidencia.afinidad))}%"></span>
            </div>
            <span class="affinity-score">${coincidencia.afinidad}%</span>
        </article>
    `).join('');
}

async function cargarAfinidades(idPerfil) {
    const id = Number(idPerfil);
    if (!Number.isInteger(id) || id <= 0) {
        perfilAfinidadActual = null;
        mostrarAfinidadInicial();
        return;
    }

    try {
        const respuesta = await solicitar(`/api/afinidades?id=${encodeURIComponent(id)}`);
        perfilAfinidadActual = id;
        elementos.affinityProfileSelect.value = String(id);
        renderizarAfinidades(respuesta.datos);
    } catch (error) {
        mostrarToast(error.message, 'error');
    }
}

async function crearPerfil(evento) {
    evento.preventDefault();
    const boton = elementos.profileForm.querySelector('button[type="submit"]');
    const textoOriginal = boton.textContent;
    boton.disabled = true;
    boton.textContent = 'Creando…';

    try {
        const parametros = parametrosDeFormulario(elementos.profileForm);
        const respuesta = await solicitar('/api/perfiles', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8' },
            body: parametros.toString()
        });

        cerrarDialogoPerfil();
        elementos.profileForm.reset();
        actualizarCamposEspeciales();
        await cargarEstado(false);
        mostrarToast(respuesta.mensaje);
        cambiarSeccion('perfiles');
    } catch (error) {
        mostrarToast(error.message, 'error');
    } finally {
        boton.disabled = false;
        boton.textContent = textoOriginal;
    }
}

async function eliminarPerfil(idPerfil) {
    const perfil = estado.perfiles.find((elemento) => elemento.id === idPerfil);
    if (!perfil) return;

    const confirmado = window.confirm(
        `¿Eliminar el perfil de ${perfil.nombre}? También se borrarán sus conexiones.`
    );
    if (!confirmado) return;

    try {
        const respuesta = await solicitar(`/api/perfiles/${idPerfil}`, { method: 'DELETE' });
        await cargarEstado(false);
        mostrarToast(respuesta.mensaje);
    } catch (error) {
        mostrarToast(error.message, 'error');
    }
}

async function crearConexion(evento) {
    evento.preventDefault();
    const idPrimero = elementos.firstProfileSelect.value;
    const idSegundo = elementos.secondProfileSelect.value;

    if (!idPrimero || !idSegundo) {
        mostrarToast('Selecciona dos perfiles.', 'error');
        return;
    }
    if (idPrimero === idSegundo) {
        mostrarToast('Elige dos perfiles diferentes.', 'error');
        return;
    }

    const textoOriginal = elementos.connectButton.textContent;
    elementos.connectButton.disabled = true;
    elementos.connectButton.textContent = 'Conectando…';

    const parametros = new URLSearchParams({
        idPrimerPerfil: idPrimero,
        idSegundoPerfil: idSegundo
    });

    try {
        const respuesta = await solicitar('/api/conexiones', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8' },
            body: parametros.toString()
        });
        await cargarEstado(false);
        mostrarToast(`${respuesta.mensaje} Afinidad: ${respuesta.afinidad}%.`);
    } catch (error) {
        mostrarToast(error.message, 'error');
    } finally {
        elementos.connectButton.disabled = estado.perfiles.length < 2;
        elementos.connectButton.textContent = textoOriginal;
    }
}

async function guardarDatos() {
    try {
        const respuesta = await solicitar('/api/guardar', { method: 'POST' });
        mostrarToast(respuesta.mensaje);
    } catch (error) {
        mostrarToast(error.message, 'error');
    }
}

async function recargarDatos() {
    const confirmado = window.confirm(
        'Se reemplazará el estado actual por el contenido de datos.txt. ¿Continuar?'
    );
    if (!confirmado) return;

    try {
        const respuesta = await solicitar('/api/cargar', { method: 'POST' });
        await cargarEstado(false);
        mostrarToast(respuesta.mensaje);
    } catch (error) {
        mostrarToast(error.message, 'error');
    }
}

async function cerrarAplicacion() {
    const confirmado = window.confirm(
        'Se guardarán los datos y se cerrará el servidor local de SoundBridge. ¿Continuar?'
    );
    if (!confirmado) return;

    try {
        const respuesta = await solicitar('/api/cerrar', { method: 'POST' });
        mostrarToast(respuesta.mensaje);
        marcarServidor(false);
        window.setTimeout(() => {
            elementos.shutdownScreen.hidden = false;
        }, 350);
    } catch (error) {
        mostrarToast(error.message, 'error');
    }
}

function registrarEventos() {
    document.querySelectorAll('.nav-item').forEach((boton) => {
        boton.addEventListener('click', () => cambiarSeccion(boton.dataset.section));
    });

    document.querySelectorAll('[data-go]').forEach((elemento) => {
        elemento.addEventListener('click', (evento) => {
            evento.preventDefault();
            cambiarSeccion(elemento.dataset.go);
        });
    });

    document.querySelector('#openProfileDialog').addEventListener('click', abrirDialogoPerfil);
    document.querySelector('#heroCreateProfile').addEventListener('click', abrirDialogoPerfil);
    document.querySelector('#closeProfileDialog').addEventListener('click', cerrarDialogoPerfil);
    document.querySelector('#cancelProfileDialog').addEventListener('click', cerrarDialogoPerfil);

    elementos.profileDialog.addEventListener('click', (evento) => {
        if (evento.target === elementos.profileDialog) cerrarDialogoPerfil();
    });

    elementos.profileType.addEventListener('change', actualizarCamposEspeciales);
    elementos.profileForm.addEventListener('submit', crearPerfil);
    elementos.profileSearch.addEventListener('input', renderizarPerfiles);
    elementos.connectionForm.addEventListener('submit', crearConexion);

    elementos.firstProfileSelect.addEventListener('change', () => {
        if (elementos.firstProfileSelect.value === elementos.secondProfileSelect.value) {
            const alternativa = estado.perfiles.find(
                (perfil) => String(perfil.id) !== elementos.firstProfileSelect.value
            );
            elementos.secondProfileSelect.value = alternativa ? String(alternativa.id) : '';
        }
    });

    elementos.secondProfileSelect.addEventListener('change', () => {
        if (elementos.firstProfileSelect.value === elementos.secondProfileSelect.value) {
            mostrarToast('Selecciona perfiles diferentes.', 'error');
        }
    });

    elementos.affinityProfileSelect.addEventListener('change', (evento) => {
        cargarAfinidades(evento.target.value);
    });

    elementos.profileGrid.addEventListener('click', (evento) => {
        const botonEliminar = evento.target.closest('[data-delete-profile]');
        if (botonEliminar) {
            eliminarPerfil(Number(botonEliminar.dataset.deleteProfile));
            return;
        }

        const botonAfinidad = evento.target.closest('[data-match-profile]');
        if (botonAfinidad) {
            const id = Number(botonAfinidad.dataset.matchProfile);
            cambiarSeccion('afinidades');
            cargarAfinidades(id);
            return;
        }

        if (evento.target.closest('[data-create-profile]')) {
            abrirDialogoPerfil();
        }
    });

    document.querySelector('#saveDataButton').addEventListener('click', guardarDatos);
    document.querySelector('#reloadDataButton').addEventListener('click', recargarDatos);
    document.querySelector('#shutdownButton').addEventListener('click', cerrarAplicacion);
}

async function iniciar() {
    registrarEventos();
    actualizarCamposEspeciales();
    mostrarAfinidadInicial();

    const seccionInicial = location.hash.replace('#', '');
    cambiarSeccion(titulosSeccion[seccionInicial] ? seccionInicial : 'inicio');

    const cargado = await cargarEstado();
    if (cargado) {
        marcarServidor(true);
    }
}

iniciar();
