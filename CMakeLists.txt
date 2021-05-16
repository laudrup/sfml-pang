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

add_subdirectory(thirdparty)
add_subdirectory(data)

add_executable(${PROJECT_NAME}
  src/background.cpp
  src/ball.cpp
  src/game.cpp
  src/main.cpp
  src/player.cpp
  src/shot.cpp
  src/sound_manager.cpp
  src/status_panel.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} PRIVATE
  project_warnings
  sfml-audio
  sfml-graphics
  sfml-system
  sfml-window
  thor
)

if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-system> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:thor> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif()

if(APPLE)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:thor> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif()
