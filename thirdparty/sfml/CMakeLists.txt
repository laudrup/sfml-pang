include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
  FetchContent_Populate(sfml)
  set(BUILD_SHARED_LIBS OFF)
  add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()
