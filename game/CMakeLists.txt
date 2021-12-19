add_executable(pang
  game.cpp
  main.cpp
)

target_link_libraries(pang
  common
)


if(WIN32)
  add_custom_command(TARGET pang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-audio> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-system> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:thor> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif()

if(APPLE)
  add_custom_command(TARGET pang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:thor> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif()
