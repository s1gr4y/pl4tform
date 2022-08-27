cmake_minimum_required(VERSION 3.0)

#set(CMAKE_C_COMPILER "C:/MinGW/bin")	#unneeded, user's should have compiler in path.

project(pl4tform VERSION 1.0
                DESCRIPTION "pl4tform"
                LANGUAGES C)
# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/Dependencies")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include/GLFW")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/glad/include")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/cglm")
include_directories("${PROJECT_SOURCE_DIR}/src/include")

file(GLOB SOURCES	"src/engine/*.c"
					"src/entity/*.c"
					"src/gfx/*.c"
					"src/utils/*.c"
					"src/*.c")

add_executable(pl4tform ${SOURCES})

target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/libglfw3dll.a)	#one of these might not be needed
target_link_libraries(pl4tform ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/libglfw3.a)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.dll ${CMAKE_BINARY_DIR} COPYONLY)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Resources/Textures ${CMAKE_BINARY_DIR}/Resources/Textures)
