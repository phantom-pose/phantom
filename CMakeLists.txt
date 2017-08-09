project(phantom)
cmake_minimum_required(VERSION 2.8)

add_definitions(--std=c++11)

# Добавил основную директорию в проект
set(SOURCE_ROOT src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(${SOURCE_ROOT} SRC_LIST)
aux_source_directory(3party/jsoncpp/src/lib_json JSONCPP_SRC)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${JSONCPP_SRC})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

qt5_use_modules(${PROJECT_NAME} Widgets OpenGL)

# Add subdirectory with Google Test Library.
add_subdirectory(3party/googletests)
# Add subdirectory with Jsoncpp library
add_subdirectory(3party/jsoncpp)
include_directories(3party/jsoncpp/include)
# Enable unit testing.
enable_testing()
# Set up testing project name.
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
# Add include directories for testing project.
include_directories(3party/googletests/googletest/include ${SOURCE_ROOT})
# Set up testing project.
set(TESTS_SOURCE_ROOT tests)
aux_source_directory(3party/jsoncpp/src/lib_json TEST_JSONCPP_SRC)
aux_source_directory(${TESTS_SOURCE_ROOT} TEST_SRC_FILES)
set(TEST_SRC_FILES ${SRC_LIST} ${TEST_SRC_FILES})
# Важная строка! Исключает запуск основного приложения во время тестов
list(REMOVE_ITEM TEST_SRC_FILES src/main.cpp)

qt5_wrap_cpp(QT_WRAPPED_TEST_SRC ${TEST_SRC_FILES})
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES} ${QT_WRAPPED_TEST_SRC} ${JSONCPP_SRC} ${TEST_JSONCPP_SRC})
# Link gtest and gtest_main libraries.
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main)
qt5_use_modules(${PROJECT_TEST_NAME} Widgets OpenGL)


# Finish tests setting up.
add_test(test ${PROJECT_TEST_NAME})

macro(copy_files GLOBPAT DESTINATION)
  file(GLOB COPY_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${GLOBPAT})
  foreach(FILENAME ${COPY_FILES})
    set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
    set(DST "${DESTINATION}/${FILENAME}")
    add_custom_command(TARGET ${PROJECT_TEST_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
  endforeach(FILENAME)
endmacro(copy_files)

set(DATA_FILES_LIST data/test.dat data/AF.dat data/AF_organs.dat data/palette.dat data/AF_bin.dat data/test_bin.dat data/bodyparts/leftHand-1.bin data/bodyparts/leftHand-2.bin data/bodyparts/leftHand-3.bin data/bodyparts/rightHand-1.bin data/bodyparts/rightHand-2.bin data/bodyparts/rightHand-3.bin data/bodyparts/leftLeg-1.bin data/bodyparts/leftLeg-2.bin data/bodyparts/leftLeg-3.bin data/bodyparts/rightLeg-1.bin data/bodyparts/rightLeg-2.bin data/bodyparts/rightLeg-3.bin)
#set(DATA_FILES_LIST data/bodyparts/leftHand-1.bin data/bodyparts/leftHand-2.bin data/bodyparts/leftHand-3.bin data/bodyparts/rightHand-1.bin data/bodyparts/rightHand-2.bin data/bodyparts/rightHand-3.bin data/bodyparts/leftLeg-1.bin data/bodyparts/leftLeg-2.bin data/bodyparts/leftLeg-3.bin data/bodyparts/rightLeg-1.bin data/bodyparts/rightLeg-2.bin data/bodyparts/rightLeg-3.bin)
set(DATA_FILES_LIST data/rotPoints.json)
copy_files("${DATA_FILES_LIST}" ${CMAKE_CURRENT_BINARY_DIR})
