cmake_minimum_required(VERSION 3.10)
project(PROJECT)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS})
link_directories(${JSONCPP_LIBRARY_DIRS})

add_executable(PROJECT src/main.cpp
        src/neo.cpp)
target_link_libraries(PROJECT ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})