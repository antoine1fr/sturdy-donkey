find_path(gl3w_INCLUDE_DIR
  NAMES GL/gl3w.h
  PATHS "${gl3w_ROOT}/include" "$ENV{gl3w_ROOT}/include"
)
find_library(gl3w_LIBRARY
  NAMES gl3w
)

set(gl3w_VERSION ${PC_gl3w_VERSION})
mark_as_advanced(gl3w_FOUND gl3w_INCLUDE_DIR gl3w_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gl3w
  REQUIRED_VARS gl3w_INCLUDE_DIR gl3w_LIBRARY
  VERSION_VAR gl3w_VERSION
)

if(gl3w_FOUND)
  set(gl3w_INCLUDE_DIRS "${gl3w_INCLUDE_DIR}")
endif()

if(gl3w_FOUND AND NOT TARGET gl3w::gl3w)
  add_library(gl3w::gl3w UNKNOWN IMPORTED)
  set_target_properties(gl3w::gl3w PROPERTIES
    IMPORTED_LOCATION "${gl3w_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${gl3w_INCLUDE_DIRS}"
  )
endif()
