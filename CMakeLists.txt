#CMAKE FOR GLFW, GLEW, GLM, SOIL
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(projectname EVA-Engine)

project(${projectname})
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

message( "system name is: " ${CMAKE_SYSTEM} )
message( "system processor is: " ${CMAKE_SYSTEM_PROCESSOR} )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET( LINUX 1)
	SET( APPLE 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET( LINUX 0)
	SET( APPLE 1 ) 
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	SET( WINDOWS 1)
endif()

find_package(OpenGL REQUIRED)

if(APPLE OR LINUX)
	find_package(GLEW REQUIRED)
endif(APPLE OR LINUX)

set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "turn off build examples") 
add_subdirectory(glfw)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${GLFW_BINARY_DIR})

add_subdirectory(SOIL)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${SOIL_BINARY_DIR})

if(WINDOWS)
	link_directories(${PROJECT_SOURCE_DIR}/glew/lib/)
endif(WINDOWS)

file(GLOB SOURCES
    source/*
)

add_executable(main ${SOURCES})

if(WINDOWS)
	target_link_libraries(main glfw glew32 SOIL ${GLFW_LIBRARIES}  ${SOIL_LIBRARIES}) 
else()
	target_link_libraries(main glfw SOIL ${GLEW_LIBRARY} ${GLFW_LIBRARIES}  ${SOIL_LIBRARIES}) 
endif(WINDOWS)

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/glfw/include/)
include_directories(${PROJECT_SOURCE_DIR}/SOIL/src/)
include_directories(${PROJECT_SOURCE_DIR}/glm/)
include_directories(${PROJECT_SOURCE_DIR}/glew/include/)

if(WINDOWS)
	include_directories(${PROJECT_SOURCE_DIR}/glew/include/)
	file(COPY  ${PROJECT_SOURCE_DIR}/dll/glew32.dll DESTINATION ${PROJECT_BINARY_DIR}/bin)
endif(WINDOWS)

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${GLEW_INCLUDE_DIRS}")
endif(APPLE)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "dir='${dir}'")
endforeach()

file(COPY  ${PROJECT_SOURCE_DIR}/assets DESTINATION ${PROJECT_BINARY_DIR}/bin)