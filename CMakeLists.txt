project(sync4cpp)

cmake_minimum_required( VERSION 2.8 )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(Boost_USE_STATIC_LIBS    ON)
find_package(Boost COMPONENTS system thread unit_test_framework REQUIRED)




#link_directories(${Boost_LIBRARY_DIRS})

file(GLOB include_files 		${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB sources_files 		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB tests_files 			${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)


source_group("includes" FILES ${include_files})
source_group("source" FILES ${sources_files})
source_group("tests" FILES ${tests_files})

include_directories(include ${Boost_INCLUDE_DIRS})

add_executable(sync4cpp		 		${include_files} ${sources_files})
add_executable(sync4cpp-tests 		${include_files} ${tests_files})

ADD_DEFINITIONS("-std=c++0x")

target_link_libraries(sync4cpp-tests 		${Boost_LIBRARIES})


# add a target to generate API documentation with Doxygen
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc.doxy ${CMAKE_CURRENT_BINARY_DIR}/doc.doxy @ONLY)
#	add_custom_target(documentation ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc.doxy
#		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#		COMMENT "Generating API documentation with Doxygen" VERBATIM
#	)
#endif(DOXYGEN_FOUND)
