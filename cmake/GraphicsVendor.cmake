# Punto reservado para configurar la tecnologia grafica cuando sea seleccionada.
function(rsm_preparar_integracion_grafica)
    message(FATAL_ERROR
        "La interfaz grafica todavia no ha sido seleccionada. "
        "No se agregaron Qt, SFML, ImGui ni otras dependencias. "
        "Revise docs/INTEGRACION_FRONTEND.md y configure "
        "RSM_ENABLE_GRAPHICS=OFF para compilar la version actual."
    )
endfunction()
