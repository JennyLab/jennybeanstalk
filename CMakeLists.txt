cmake_minimum_required(VERSION 3.10)
project(beanstalkd)

# Establecer el estándar de C y las banderas de compilación.
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wformat=2 -g")

# Incluir directorios de encabezados.
include_directories(${CMAKE_SOURCE_DIR})

# Archivos de origen principales.
set(OFILES
    conn.c
    file.c
    heap.c
    job.c
    ms.c
    net.c
    primes.c
    prot.c
    serv.c
    time.c
    tube.c
    util.c
    vers.c
    walg.c
)

set(MOFILE main.c)

# Configuración específica del sistema operativo.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND LDFLAGS "-lrt")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    list(APPEND LDFLAGS "-lxnet -lsocket -lnsl")
endif()

# Soporte para systemd.
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSYSTEMD libsystemd)
if(LIBSYSTEMD_FOUND AND NOT USE_SYSTEMD STREQUAL "no")
    list(APPEND LDFLAGS ${LIBSYSTEMD_LDFLAGS})
    add_definitions(-DHAVE_LIBSYSTEMD)
elseif(USE_SYSTEMD STREQUAL "yes")
    message(FATAL_ERROR "USE_SYSTEMD is set to '${USE_SYSTEMD}', but libsystemd is not found")
endif()

# Crear el ejecutable principal.
add_executable(${PROJECT_NAME} ${OFILES} ${MOFILE})

# Vincular bibliotecas.
target_link_libraries(${PROJECT_NAME} ${LDFLAGS})

# Configuraciones de instalación.
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Objetivo personalizado para limpiar archivos.
add_custom_target(clean
    COMMAND ${CMAKE_COMMAND} -E remove ${CLEANFILES}
)

# Objetivo personalizado para check.
add_custom_target(check
    COMMAND ct/_ctcheck
)

# Objetivo personalizado para bench.
add_custom_target(bench
    COMMAND ct/_ctcheck -b
)

# Generación de versión.
add_custom_command(
    OUTPUT vers.c
    COMMAND ./verc.sh > vers.c
    DEPENDS verc.sh
)

# Documentación.
add_custom_command(
    OUTPUT doc/beanstalkd.1 doc/beanstalkd.1.html
    COMMAND ronn doc/beanstalkd.ronn
    DEPENDS doc/beanstalkd.ronn
)

# Archivos de prueba.
set(TOFILES
    testheap.c
    testjobs.c
    testms.c
    testserv.c
    testutil.c
)

# Compilación de archivos de prueba.
add_executable(ct/_ctcheck ct/_ctcheck.c ct/ct.c ${TOFILES} ${OFILES})
