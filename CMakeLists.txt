#CMAKE FOR GLFW, GLEW, GLM
cmake_minimum_required(VERSION 3.1)

# Project name
project(EVA)

# System type
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET( LINUX 1)
	SET( APPLE 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET( LINUX 0)
	SET( APPLE 1 ) 
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	SET( WINDOWS 1)
endif()

# Set GLFW options
set(GLFW_BUILD_EXAMPLES OFF) 
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

# Find packages
find_package(OpenGL REQUIRED)

if(APPLE OR LINUX)
	find_package(GLEW REQUIRED)
endif(APPLE OR LINUX)

# Link GLFW and GLEW directories
add_subdirectory(dependencies/glfw)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${GLFW_BINARY_DIR})

add_subdirectory(dependencies/assimp)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${ASSIMP_BINARY_DIR})

if(WINDOWS)
	link_directories(${PROJECT_SOURCE_DIR}/dependencies/glew/lib/)
	link_directories(${PROJECT_SOURCE_DIR}/dependencies/freetype/lib/)
endif(WINDOWS)

# Main executeable
file(GLOB SOURCES
	source/*
	source/*/*
	EVA/*
	EVA/*/*
	TestScenes/*
	TestScenes/*/*
	dependencies/stb-image/*
	dependencies/imgui/*
	dependencies/tfd/*
)

add_executable(main ${SOURCES})

set_target_properties(
    main
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Link and include
target_link_libraries(
	main 
	glfw 
	${GLFW_LIBRARIES}
	assimp
	${ASSIMP_LIBRARIES}
	freetype
	${FREETYPE_LIBRARIES}
) 

target_include_directories(
    main 
	PRIVATE ${PROJECT_SOURCE_DIR}/EVA/include
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/glew/include
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/glfw/include
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/glm
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/stb-image
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/assimp/include/
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/freetype/include/
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/rapidjson/include/
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/imgui
	PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/tfd
)

if(WINDOWS)
	target_link_libraries(
		main 
		glew32
	) 
	
	target_include_directories(
		main 
		PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/glew/include
	)

	file(
		COPY  ${PROJECT_SOURCE_DIR}/dependencies/dll/glew32.dll 
		DESTINATION ${PROJECT_BINARY_DIR}/bin
	)
	file(
		COPY  ${PROJECT_SOURCE_DIR}/dependencies/freetype/bin/freetype6.dll
		DESTINATION ${PROJECT_BINARY_DIR}/bin
	)
else()
	target_link_libraries(
		main 
		${GLEW_LIBRARY}
	)
endif(WINDOWS)

if(LINUX)
	add_library(stdc++fs UNKNOWN IMPORTED)
	set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/dependencies/stdfs/libstdc++fs.a")
	target_link_libraries(
			main
			stdc++fs
	)
endif(LINUX)

# Apple
if(APPLE)
	set(
		CMAKE_CXX_FLAGS 
		"${CMAKE_CXX_FLAGS} -I${GLEW_INCLUDE_DIRS}"
	)
endif(APPLE)


# Copy assets
file(COPY  ${PROJECT_SOURCE_DIR}/assets DESTINATION ${PROJECT_BINARY_DIR}/bin)

find_package(Doxygen)
#Set up doxygen if it is installed
if(DOXYGEN_FOUND)

	message("Setting up doxygen")
	set(DOXYGEN_IN ${PROJECT_BINARY_DIR}/CMakeDoxyfile.in)
	set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)

	# Setting folders to generate documentation for
	set(DOXYGEN_INPUT 
		"${PROJECT_SOURCE_DIR}/source ${PROJECT_SOURCE_DIR}/EVA"
	)
	set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/doc)
	set(DOXYGEN_INCLUDE_GRAPH YES)

	# This first one is optional
	set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
	set(DOXYGEN_EXTRACT_ALL YES)
	set(DOXYGEN_EXTRACT_STATIC YES)

	set(DOXYGEN_EXTRACT_STATIC YES)
	set(DOXYGEN_HAVE_DOT YES)

	set(DOXYGEN_DOT_NUM_THREADS 4)
	set(DOXYGEN_CALL_GRAPH  YES)
	set(DOXYGEN_CALLER_GRAPH YES)
	set(DOXYGEN_RECURSIVE YES)

	#DOT_PATH = ${DOXYGEN_DOT_PATH}

	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

	add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} 
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

	#set_target_properties(doc_doxygen
	#	PROPERTIES
	#	DOXYGEN_INPUT ""
	#	DOXYGEN_OUTPUT_DIRECTORY
	#)
else()

	message("Could not find doxygen")

endif(DOXYGEN_FOUND)
