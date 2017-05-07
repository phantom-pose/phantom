project(phantom)
cmake_minimum_required(VERSION 2.8)

add_definitions(--std=c++11)

# Добавил основную директорию в проект
set(SOURCE_ROOT src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(${SOURCE_ROOT} SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

qt5_use_modules(${PROJECT_NAME} Widgets OpenGL)

