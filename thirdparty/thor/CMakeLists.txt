include(ExternalProject)

ExternalProject_Add(thor_external
  GIT_REPOSITORY https://github.com/Bromeon/Thor.git
  GIT_TAG v2.0
  CMAKE_ARGS -DSFML_ROOT:PATH=${SFML_ROOT} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  UPDATE_COMMAND ""
  BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libthor.so # Only used by Ninja generator
)

ExternalProject_Add_StepDependencies(thor_external download
  ${CMAKE_CURRENT_LIST_FILE}
)

ExternalProject_Get_Property(thor_external INSTALL_DIR)

# Work around CMake bug #15052
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(thor SHARED IMPORTED GLOBAL)
target_include_directories(thor INTERFACE ${INSTALL_DIR}/include)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties(thor PROPERTIES IMPORTED_IMPLIB_RELEASE ${INSTALL_DIR}/lib/thor.lib)
  set_target_properties(thor PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/bin/thor.dll)
  set_target_properties(thor PROPERTIES IMPORTED_IMPLIB_DEBUG ${INSTALL_DIR}/lib/thor-d.lib)
  set_target_properties(thor PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/bin/thor-d.dll)
elseif(APPLE)
  set_target_properties(thor PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libthor.dylib)
else()
  set_target_properties(thor PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libthor.so)
endif()

add_dependencies(thor thor_external)
