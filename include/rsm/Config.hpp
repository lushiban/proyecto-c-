#ifndef RSM_CONFIG_HPP
#define RSM_CONFIG_HPP

#ifndef RSM_ENABLE_GRAPHICS
#define RSM_ENABLE_GRAPHICS 0
#endif

#ifndef RSM_VERSION_PROYECTO
#define RSM_VERSION_PROYECTO "0.1.0"
#endif

namespace rsm {

inline constexpr const char* NOMBRE_PROYECTO = "Red Social Musical";
inline constexpr const char* VERSION_PROYECTO = RSM_VERSION_PROYECTO;
inline constexpr bool SOPORTE_GRAFICO_HABILITADO = RSM_ENABLE_GRAPHICS != 0;

}

#endif
