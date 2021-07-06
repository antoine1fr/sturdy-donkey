add_executable(editor
  "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/ResourceLoaderDelegate.cpp")
target_include_directories(editor PUBLIC
  "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(editor
  sturdy-donkey)
if(MSVC)
	# Don't bother with /Wall on MSVC since it's incompatible with system headers.
	# Also their magical /external: switch family doesn't seem to work anymore.
	# Sad times.
else()
	target_compile_options(editor PRIVATE -Werror -Wall -pedantic)
endif()
target_compile_features(editor PRIVATE cxx_std_17)
set_target_properties(editor PROPERTIES CXX_EXTENSIONS OFF)
