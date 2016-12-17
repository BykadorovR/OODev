cmake_minimum_required(VERSION 2.7)
project(map_template)
set(wxWidgets_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wxWidgets-3.1.0)
find_package(wxWidgets REQUIRED gl core base)
include(${wxWidgets_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/map_template/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Eigen)
include_directories(${wxWidgets_INCLUDE_DIRS})
link_directories(${wxWidgets_LIBRARY_DIRS})


FILE(GLOB SRC map_template/src/*.c*)
FILE(GLOB INCLUDE map_template/include/*.h)
add_executable(${PROJECT_NAME} ${SRC} ${INCLUDE})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

if(WIN32)
	if(MSVC AND MSVC_VERSION EQUAL 1600)
		 set_target_properties(map_template PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	else()
		message(SEND_ERROR "MSVC 1600 (vs10) Required")
	endif()
else()
  message(SEND_ERROR "WIN32 only")
endif()

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