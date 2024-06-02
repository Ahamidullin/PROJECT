cmake_minimum_required(VERSION 3.5)

project(NEOO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(CURL REQUIRED)

set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    neo.cpp
)

set(HEADER_FILES
    mainwindow.h
    neo.h
)

set(UI_FILES
    mainwindow.ui
)

qt6_wrap_ui(UI_HEADERS ${UI_FILES})
qt6_wrap_cpp(MOC_SOURCES ${HEADER_FILES})

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CURL_INCLUDE_DIRS})
include_directories(/opt/homebrew/Cellar/jsoncpp/1.9.5/include)

link_directories(/opt/homebrew/Cellar/jsoncpp/1.9.5/lib)

add_executable(NEOO
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_HEADERS}
    ${MOC_SOURCES}
)

target_link_libraries(NEOO Qt6::Widgets ${CURL_LIBRARIES} jsoncpp)
