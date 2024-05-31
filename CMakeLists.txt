cmake_minimum_required(VERSION 3.10)
project(PROJECT_v2)

set(CMAKE_GENERATOR "Ninja")

# Подключение Google Test
add_subdirectory(lib/googletest)
enable_testing()

# Поиск и подключение зависимостей
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories(${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS} ${JSONCPP_LIBRARY_DIRS})

# Создание исполняемого файла
add_executable(PROJECT_v2 src/main.cpp src/neo.cpp)

# Связывание библиотек с проектом
target_link_libraries(PROJECT_v2 ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES} gtest gtest_main)
