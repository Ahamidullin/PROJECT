cmake_minimum_required(VERSION 3.10)
project(PROJECT)

set(CMAKE_CXX_STANDARD 11)

# Подключение Google Test
add_subdirectory(lib/googletest)  # Ensure the Google Test library is in this directory
enable_testing()

# Поиск и подключение зависимостей
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories(${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS} ${JSONCPP_LIBRARY_DIRS})

# Создание исполняемого файла для основного проекта
add_executable(PROJECT src/main.cpp src/neo.cpp)

# Связывание библиотек с основным проектом
target_link_libraries(PROJECT ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES} gtest gtest_main)

# Создание тестового исполняемого файла
add_executable(test_project test/test_neo.cpp)

# Связывание библиотек с тестовым проектом
target_link_libraries(test_project ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES} gtest gtest_main)

# Добавление тестов1
add_test(NAME test_project COMMAND test_project)
