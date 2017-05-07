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

# Add subdirectory with Google Test Library.
add_subdirectory(3party/googletests)
# Enable unit testing.
enable_testing()
# Set up testing project name.
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
# Add include directories for testing project.
include_directories(3party/googletests/googletest/include ${SOURCE_ROOT})
# Set up testing project.
set(TESTS_SOURCE_ROOT tests)
aux_source_directory(${TESTS_SOURCE_ROOT} TEST_SRC_FILES)
set(TEST_SRC_FILES ${SRC_LIST} ${TEST_SRC_FILES})
# Важная строка! Исключает запуск основного приложения во время тестов
list(REMOVE_ITEM TEST_SRC_FILES src/main.cpp)

qt5_wrap_cpp(QT_WRAPPED_TEST_SRC ${TEST_SRC_FILES})
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES} ${QT_WRAPPED_TEST_SRC})
# Link gtest and gtest_main libraries.
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main)
qt5_use_modules(${PROJECT_TEST_NAME} Widgets OpenGL)


# Finish tests setting up.
add_test(test ${PROJECT_TEST_NAME})
