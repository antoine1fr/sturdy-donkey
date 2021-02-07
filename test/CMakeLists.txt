add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/googletest-release-1.10.0"
	EXCLUDE_FROM_ALL)

find_package(Threads REQUIRED) # for pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(test "${CMAKE_CURRENT_LIST_DIR}/simple_test.cpp")

if(MSVC)
	# Don't bother with /Wall on MSVC since it's incompatible with system headers.
	# Also their magical /external: switch family doesn't seem to work anymore.
	# Sad times.
else()
	target_compile_options(test PRIVATE -Werror -Wall -pedantic)
endif()

target_compile_features(test PRIVATE cxx_std_17)
set_target_properties(test PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(test
  PUBLIC
  gtest_main
  Threads::Threads
)
