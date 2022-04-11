cmake_minimum_required(VERSION 3.2)
project(nps-surfer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(nps-surfer
  src/ui/mainwindow.ui
  src/main.cpp
  src/dataloader.cpp)

target_link_libraries(nps-surfer PRIVATE Qt6::Widgets)
