cmake_minimum_required(VERSION 3.2)
project(nps-surfer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(CURL REQUIRED)

add_executable(nps-surfer
  src/main.cpp
  src/dataloader.cpp
  src/mainwindow.ui
  src/mainwindow.h
  src/mainwindow.cpp
  src/progressdialog.ui
  src/progressdialog.h
  src/progressdialog.cpp
  src/titletablemodel.h
  src/titletablemodel.cpp)

target_include_directories(nps-surfer PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(nps-surfer PRIVATE Qt6::Widgets ${CURL_LIBRARIES})
