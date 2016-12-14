project(map_template)
include_directories("map_template/include/")
FILE(GLOB SRC map_template/src/*.c*)
FILE(GLOB INCLUDE map_template/include/*.h)
include_directories("Eigen")
add_executable(${PROJECT_NAME} ${SRC} ${INCLUDE})
target_link_libraries(${PROJECT_NAME})
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