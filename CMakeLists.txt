cmake_minimum_required(VERSION 3.10)
project(PROJECT)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/googletest)
enable_testing()

find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories(${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS} ${JSONCPP_LIBRARY_DIRS})

add_executable(PROJECT src/main.cpp src/neo.cpp)

# Связывание библиотек с основным проектом
target_link_libraries(PROJECT ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES} gtest gtest_main)

add_executable(test_project test/test_neo.cpp)

# Связывание библиотек с тестовым проектом
target_link_libraries(test_project ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES} gtest gmock gtest_main gmock_main)

add_test(NAME test_project COMMAND test_project)
