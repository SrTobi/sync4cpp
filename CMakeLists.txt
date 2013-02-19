project(sync4cpp)

cmake_minimum_required( VERSION 2.8 )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(Boost_USE_STATIC_LIBS    ON)
find_package(Boost 1.47.0 COMPONENTS system thread unit_test_framework REQUIRED)




link_directories(${Boost_LIBRARY_DIRS})

file(GLOB header 				 ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB test_sources 			 ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB tests_sources 		 ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)


source_group("includes" FILES ${header})
source_group("source" FILES ${test_sources})
source_group("source\\tests" FILES ${tests_sources})

include_directories(include ${Boost_INCLUDE_DIRS})

add_executable(sync4cpp-tests 		${test_sources} ${header} ${tests_sources})

ADD_DEFINITIONS("-std=c++0x")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
SET(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ftest-coverage -fprofile-arcs -fprofile-values -fvpt -pg")
MARK_AS_ADVANCED(CMAKE_CXX_FLAGS_PROFILE)
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Profile."
    FORCE )

set(DEBUG_BUILD 0)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG_BUILD 1)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(DEBUG_BUILD 1)
endif ()
message (STATUS "Enabling debug code: ${DEBUG_BUILD}")

message(STATUS "Default: ${CMAKE_CXX_FLAGS}")
message(STATUS "Debug: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "Profile: ${CMAKE_CXX_FLAGS_PROFILE}")
message(STATUS "Release: ${CMAKE_CXX_FLAGS_RELEASE}")

target_link_libraries(sync4cpp-tests 		${Boost_LIBRARIES})


# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc.doxy ${CMAKE_CURRENT_BINARY_DIR}/doc.doxy @ONLY)
	add_custom_target(documentation ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc.doxy
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)
