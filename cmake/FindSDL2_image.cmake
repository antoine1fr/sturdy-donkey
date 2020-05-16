find_package(PkgConfig)

pkg_check_modules(PC_SDL2_image QUIET SDL2_image)
find_path(SDL2_image_INCLUDE_DIR
  NAMES SDL_image.h
  PATHS ${PC_SDL2_image_INCLUDE_DIRS}
)
find_library(SDL2_image_LIBRARY
  NAMES SDL2_image
  PATHS ${PC_SDL2_image_LIBRARY_DIRS}
)

set(SDL2_image_VERSION ${PC_SDL2_image_VERSION})
mark_as_advanced(SDL2_image_FOUND SDL2_image_INCLUDE_DIR SDL2_image_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_image
  REQUIRED_VARS SDL2_image_INCLUDE_DIR
  VERSION_VAR SDL2_image_VERSION
)

if(SDL2_image_FOUND)
  set(SDL2_image_INCLUDE_DIRS ${SDL2_image_INCLUDE_DIR})
  set(SDL2_image_LIBRARIES ${SDL2_image_LIBRARY})
endif()

if(SDL2_image_FOUND AND NOT TARGET SDL2_image::SDL2_image)
  add_library(SDL2_image::SDL2_image UNKNOWN IMPORTED)
  set_target_properties(SDL2_image::SDL2_image PROPERTIES
    IMPORTED_LOCATION "${SDL2_image_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_image_INCLUDE_DIRS}"
  )
endif()
