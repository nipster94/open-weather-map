cmake_minimum_required(VERSION 3.0.2)
project(OpenWeatherMap)

find_package(Qt5 REQUIRED COMPONENTS
  Core
  Widgets
  Network
)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_FORMS forms/owmgui.ui)
set(QT_SOURCES src/main.cpp src/owmgui.cpp src/owmapi.cpp)
set(QT_HEADERS includes/owmgui.h includes/owmapi.h)

QT5_WRAP_CPP(QT_HEADERS_MOC ${QT_HEADERS} ${QT_SOURCES})
QT5_WRAP_UI(QT_FORMS_MOC ${QT_FORMS})

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

add_library(owmgui ${QT_HEADERS} ${QT_SOURCES} ${QT_FORMS})
target_link_libraries (owmgui Qt5::Widgets Qt5::Core Qt5::Network)

add_executable(OpenWeatherMap src/main.cpp)
target_link_libraries (OpenWeatherMap owmgui)
