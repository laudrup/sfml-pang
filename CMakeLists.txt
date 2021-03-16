cmake_minimum_required(VERSION 3.15)

project(pang CXX)

add_subdirectory(thirdparty)

find_package(SFML
  COMPONENTS
  system
  window
  graphics
  REQUIRED
)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/ball.cpp
  src/player.cpp
  src/shot.cpp
  src/background.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  thor
  sfml-system
  sfml-window
  sfml-graphics
)
