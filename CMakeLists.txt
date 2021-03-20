cmake_minimum_required(VERSION 3.15)

project(pang CXX)

add_subdirectory(thirdparty)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


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
