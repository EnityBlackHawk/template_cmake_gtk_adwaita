cmake_minimum_required(VERSION 3.30)
project(cmake_gtk)

set(CMAKE_CXX_STANDARD 20)
set(NAMESPACE org.ebh.Example)
set(ICON org.gnome.Example)
set(EXECUTABLE ${CMAKE_PROJECT_NAME})
set(RESOURCE_BASE_PATH /org/ebh/Example)
set(APPLICATION_NAME "CMake Gtk")

find_package(PkgConfig REQUIRED)

# Find GTK and Libadwaita
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(LIBADWAITA REQUIRED libadwaita-1)

# Include GTK and Libadwaita
include_directories(${GTK_INCLUDE_DIRS} ${LIBADWAITA_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS} ${LIBADWAITA_LIBRARY_DIRS})


#####################################################################
# Blueprint-compiler

if(LOCAL_BLUEPRINT_COMPILER)

        set(BLUEPRINT_COMPILER_EXECUTABLE blueprint-compiler)

        add_custom_target(
                BlueprintCompiler
                COMMAND ${CMAKE_COMMAND} -E echo "Using local blueprint-compiler"
        )

else()

        include(ExternalProject)
        find_package(Python3 REQUIRED COMPONENTS Interpreter)

        ExternalProject_Add(
                BlueprintCompiler
                GIT_REPOSITORY https://gitlab.gnome.org/jwestman/blueprint-compiler.git
                GIT_TAG main
                PREFIX ${CMAKE_BINARY_DIR}/blueprint-compiler
                CONFIGURE_COMMAND ""
                BUILD_COMMAND ""
                INSTALL_COMMAND ""
                LOG_DOWNLOAD ON
        )

        ExternalProject_Get_Property(BlueprintCompiler SOURCE_DIR)
        set(BLUEPRINT_COMPILER_EXECUTABLE ${Python3_EXECUTABLE} ${SOURCE_DIR}/blueprint-compiler.py)
endif()

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/ui")
file(GLOB_RECURSE BLP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.blp")
foreach(BLP_FILE IN LISTS BLP_FILES)
        
        get_filename_component(FILE_NAME_WE ${BLP_FILE} NAME_WE)
        set(UI_FILE ${CMAKE_BINARY_DIR}/ui/${FILE_NAME_WE}.ui)
        list(APPEND UI_FILES ${UI_FILE})

        add_custom_command(
                OUTPUT ${UI_FILE}
                COMMAND ${BLUEPRINT_COMPILER_EXECUTABLE} compile --output ${UI_FILE} ${BLP_FILE}
                DEPENDS ${BLP_FILE} BlueprintCompiler
                COMMENT "Compiling ${BLP_FILE} to ${UI_FILE}"
        )

endforeach()

file(GLOB_RECURSE NON_BLUEPRINT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")
file(COPY ${NON_BLUEPRINT_FILES} DESTINATION "${CMAKE_BINARY_DIR}/ui")
add_custom_target(compile_ui ALL DEPENDS ${UI_FILES})


###########################################################
# Compile resources

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(GRESOURCE_C   resources.h)
set(GRESOURCE_XML resources.gresource.xml)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${GRESOURCE_XML}.in
        ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_XML}
        @ONLY
)


# Step 2:
add_custom_command(
        OUTPUT ${GRESOURCE_C}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${GLIB_COMPILE_RESOURCES}
        ARGS
        --generate-source
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}
        --sourcedir=${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_XML}
        VERBATIM
        MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_XML}
        DEPENDS ${UI_FILES} compile_ui
)

# Step 3:
add_custom_target(
        resource
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
)
add_dependencies(resource compile_ui)

set_source_files_properties(
        ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        PROPERTIES GENERATED TRUE
)


include_directories(${CMAKE_PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
FILE(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(${CMAKE_PROJECT_NAME} "${SOURCE}")
add_dependencies(${CMAKE_PROJECT_NAME} resource)

#Link GTK and Libadwaita
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTK_LIBRARIES} ${LIBADWAITA_LIBRARIES})

# Compile definitions
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${GTK4_CFLAGS_OTHER} ${LIBADWAITA_CFLAGS_OTHER})

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/service-files/application.desktop.in
        ${CMAKE_CURRENT_BINARY_DIR}/service-files/${NAMESPACE}.desktop
        @ONLY
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/service-files/application.service.in
        ${CMAKE_CURRENT_BINARY_DIR}/service-files/${NAMESPACE}.service
        @ONLY
)

install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/service-files/${NAMESPACE}.desktop
        DESTINATION share/applications
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/service-files/${NAMESPACE}.service
        DESTINATION share/dbus-1/services
)

install(FILES ${GSETTINGS_SCHEMA_DIR}/org.example.myapp.gschema.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/glib-2.0/schemas)

