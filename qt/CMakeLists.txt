cmake_minimum_required(VERSION 3.16)

project(NEOClusteringApp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Qt's auto MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt libraries for Qt 6, including Qt Charts
find_package(Qt6 COMPONENTS Widgets Network Charts REQUIRED)

# Include the current directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add the executable
add_executable(NEOClusteringApp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    fetch_neo_data.cpp
    fetch_neo_data.h
    neo.h
)

# Link Qt libraries
target_link_libraries(NEOClusteringApp Qt6::Widgets Qt6::Network Qt6::Charts)
