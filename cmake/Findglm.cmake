find_package(PkgConfig)

pkg_check_modules(PC_glm QUIET glm)
find_path(glm_INCLUDE_DIR
  NAMES glm/glm.hpp
  PATHS ${PC_glm_INCLUDE_DIRS}
)

set(glm_VERSION ${PC_glm_VERSION})
mark_as_advanced(glm_FOUND glm_INCLUDE_DIR glm_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glm
  REQUIRED_VARS glm_INCLUDE_DIR
  VERSION_VAR glm_VERSION
)

if(glm_FOUND)
  set(glm_INCLUDE_DIRS ${glm_INCLUDE_DIR})
endif()

if(glm_FOUND AND NOT TARGET glm::glm)
  add_library(glm::glm INTERFACE IMPORTED)
  set_target_properties(glm::glm PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${glm_INCLUDE_DIRS}"
  )
endif()
