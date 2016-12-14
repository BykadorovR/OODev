cmake_minimum_required(VERSION 3.2)
project(map_template)
include_directories("map_template/include/")
include_directories("Eigen")
include_directories("wxWidgets-3.1.0/include")
link_directories("wxWidgets-3.1.0/lib/vc_lib")
FILE(GLOB SRC map_template/src/*.c*)
FILE(GLOB INCLUDE map_template/include/*.h)
FILE(GLOB WXLIB wxWidgets-3.1.0/lib/vc_lib/*.lib)
add_executable(${PROJECT_NAME} ${SRC} ${INCLUDE})
target_link_libraries(${PROJECT_NAME} ${WXLIB})
################################
# GTest
################################
ADD_SUBDIRECTORY (googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
################################
FILE(GLOB SRC_test tests/*.c*)
SET(CMAKE_CXX_FLAGS_DEBUG "/MDd")
add_executable(${PROJECT_NAME}_test ${SRC_test} ${INCLUDE})
target_link_libraries(${PROJECT_NAME}_test gtest gtest_main)
add_test( ${PROJECT_NAME}_test ${PROJECT_NAME}_test )