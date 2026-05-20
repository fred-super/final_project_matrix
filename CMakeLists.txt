cmake_minimum_required(VERSION 3.14)
project("matrix_gui")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(a.out main.cpp)
target_include_directories(a.out PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tests tests/test_main.cpp)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

gtest_discover_tests(tests)


add_executable(matrix_gui src/main_gui.cpp src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui)
target_include_directories(matrix_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(matrix_gui PRIVATE Qt6::Widgets)

