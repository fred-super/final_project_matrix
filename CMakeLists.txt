cmake_minimum_required(VERSION 3.14)
project("matrix_gui")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(USE_EIGEN "Use Eigen library as optional calculation backend" OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if(USE_EIGEN)
    find_package(Eigen3 REQUIRED NO_MODULE)
endif()

add_executable(a.out main.cpp)
target_include_directories(a.out PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tests tests/test_main.cpp)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE ${GTEST_LIBRARIES} pthread)

gtest_discover_tests(tests)


add_executable(matrix_gui src/main_gui.cpp src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui)
target_include_directories(matrix_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(matrix_gui PRIVATE Qt6::Widgets)

if(USE_EIGEN)
    target_compile_definitions(a.out PRIVATE USE_EIGEN)
    target_compile_definitions(tests PRIVATE USE_EIGEN)
    target_compile_definitions(matrix_gui PRIVATE USE_EIGEN)

    target_link_libraries(a.out PRIVATE Eigen3::Eigen)
    target_link_libraries(tests PRIVATE Eigen3::Eigen)
    target_link_libraries(matrix_gui PRIVATE Eigen3::Eigen)
endif()