include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  thor
  GIT_REPOSITORY https://github.com/Bromeon/Thor.git
  GIT_TAG v2.0
  GIT_PROGRESS TRUE
)

FetchContent_GetProperties(thor)
if(NOT thor_POPULATED)
  FetchContent_Populate(thor)
  set(THOR_SHARED_LIBS FALSE CACHE BOOL "Build shared Thor libs")
  set(SFML_STATIC_LIBRARIES TRUE)
  add_subdirectory(${thor_SOURCE_DIR} ${thor_BINARY_DIR})
endif()

# Make thor a CMake linkable target by adding libraries and
# directories.
# Can hopefully be removed when upgrading thor library
target_link_libraries(thor PUBLIC
  sfml-window
  sfml-graphics
)
target_include_directories(thor PUBLIC
  ${thor_SOURCE_DIR}/extlibs/aurora/include
  ${thor_SOURCE_DIR}/include
)
