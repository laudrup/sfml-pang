add_library(common
  src/background.cpp
  src/ball.cpp
  src/player.cpp
  src/shot.cpp
  src/sound_manager.cpp
  src/status_panel.cpp
)

target_include_directories(common PUBLIC
  include
)

target_link_libraries(common PUBLIC
  project_options
  project_warnings
  sfml-audio
  sfml-graphics
  sfml-system
  sfml-window
  thor
)

if(WIN32)
  target_compile_definitions(common PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
