cmake_minimum_required(VERSION 3.15)

project(pang CXX)

set(SFML_DIR ${SFML_ROOT}/lib/cmake/SFML)
find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Sanitizers.cmake)

add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_17)

set_project_warnings(project_warnings)
enable_sanitizers(project_options)

add_subdirectory(data)
add_subdirectory(game)
add_subdirectory(lib)
add_subdirectory(thirdparty)
