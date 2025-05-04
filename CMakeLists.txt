cmake_minimum_required(VERSION 3.30)
project(cmake_gtk)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

# Find GTK and Libadwaita
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(LIBADWAITA REQUIRED libadwaita-1)

# Include GTK and Libadwaita
include_directories(${GTK_INCLUDE_DIRS} ${LIBADWAITA_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS} ${LIBADWAITA_LIBRARY_DIRS})


add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ui/window.ui
        COMMAND blueprint-compiler batch-compile ${CMAKE_CURRENT_BINARY_DIR}/ui ${CMAKE_CURRENT_SOURCE_DIR}/ui/ ${CMAKE_CURRENT_SOURCE_DIR}/ui/window.blp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ui/window.blp
)

add_custom_target(compile_ui ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ui/window.ui)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(GRESOURCE_C   resources.h)
set(GRESOURCE_XML resources.gresource.xml)

# Step 2:
add_custom_command(
        OUTPUT ${GRESOURCE_C}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${GLIB_COMPILE_RESOURCES}
        ARGS
        --generate-source
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        ${CMAKE_CURRENT_SOURCE_DIR}/${GRESOURCE_XML}
        VERBATIM
        MAIN_DEPENDENCY ${GRESOURCE_XML}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ui/window.ui
)

# Step 3:
add_custom_target(
        resource
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
)

set_source_files_properties(
        ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        PROPERTIES GENERATED TRUE
)


include_directories(cmake_gtk "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
FILE(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(cmake_gtk "${SOURCE}")
add_dependencies(cmake_gtk compile_ui)
add_dependencies(cmake_gtk resource)

#Link GTK and Libadwaita
target_link_libraries(cmake_gtk ${GTK_LIBRARIES} ${LIBADWAITA_LIBRARIES})

# Compile definitions
target_compile_options(cmake_gtk PRIVATE ${GTK4_CFLAGS_OTHER} ${LIBADWAITA_CFLAGS_OTHER})