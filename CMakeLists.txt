cmake_minimum_required(VERSION 3.0)

#set(CMAKE_C_COMPILER "C:/MinGW/bin")	#unneeded, user should have compiler in path.

project(pl4tform VERSION 1.0
                DESCRIPTION "pl4tform"
                LANGUAGES C)

#Spawn bin upon build
file(MAKE_DIRECTORY ${directory} bin)

#Set the output folder where the program will be created in bin
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#The following folders will be included
include_directories("${PROJECT_SOURCE_DIR}/Dependencies")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include/GLFW")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/glad/include")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/cglm")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/freetype")
include_directories("${PROJECT_SOURCE_DIR}/src/include")

file(GLOB SOURCES
					"src/engine/*.c"
					"src/entity/*.c"
					"src/gfx/*.c"
					"src/utils/*.c"
					"src/*.c")

add_executable(pl4tform ${SOURCES})
if (UNIX)
	find_library(MATH_LIBRARY m)
	if (MATH_LIBRARY)
		target_link_libraries(pl4tform ${MATH_LIBRARY})
	endif (MATH_LIBRARY)
	target_link_libraries(pl4tform OpenGL:GL ${CMAKE_DL_LIBS})
endif (UNIX)

#Link the libraries
if (WIN32)
	target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/libglfw3dll.a)	#need to also link dll
	target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/libglfw3.a)
	target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/freetype.a)
endif (WIN32)

if (UNIX)
	target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/libglfw3dll.so.3.3)	#need to also link so
	target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/libglfw3.a)
	target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/libfreetype.so)
endif (UNIX)

#Copy the dlls/shared libraries and Resources folder to the output folder
if (UNIX)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/libglfw3dll.so.3.3 ${CMAKE_BINARY_DIR} COPYONLY)
endif (UNIX)
if (WIN32)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.dll ${CMAKE_BINARY_DIR} COPYONLY)
endif (WIN32)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Resources)
					   
