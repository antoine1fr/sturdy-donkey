find_package(GTest 1.10 REQUIRED)

find_package(Threads REQUIRED) # for pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(test "${CMAKE_CURRENT_LIST_DIR}/simple_test.cpp")

target_compile_options(test PRIVATE -Werror -Wall -pedantic)
target_compile_features(test PRIVATE cxx_std_17)
set_target_properties(test PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(test
  PUBLIC
  GTest::GTest
  GTest::Main
  Threads::Threads
)
