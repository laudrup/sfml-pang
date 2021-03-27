cmake_minimum_required(VERSION 3.15)

project(pang CXX)

set(SFML_DIR ${SFML_ROOT}/lib/cmake/SFML)
find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(gfx)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/ball.cpp
  src/player.cpp
  src/shot.cpp
  src/background.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  sfml-graphics
  sfml-system
  sfml-window
  thor
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
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
