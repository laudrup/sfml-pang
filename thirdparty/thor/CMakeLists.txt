include(ExternalProject)

ExternalProject_Add(thor_external
  GIT_REPOSITORY https://github.com/Bromeon/Thor.git
  GIT_TAG v2.0
  CMAKE_CACHE_ARGS -DTHOR_SHARED_LIBS:BOOL=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  UPDATE_COMMAND ""
  BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libthor-s.a
)

ExternalProject_Get_property(thor_external INSTALL_DIR)

# Work around CMake bug #15052
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(thor STATIC IMPORTED GLOBAL)
target_include_directories(thor INTERFACE ${INSTALL_DIR}/include)
set_target_properties(thor PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libthor-s.a)
add_dependencies(thor thor_external)
