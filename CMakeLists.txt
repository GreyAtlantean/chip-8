cmake_minimum_required(VERSION 3.28)
project(chip8)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)

set(SOURCE_FILES
	src/main.cpp
	src/renderer.cpp
	src/screen.cpp
	src/chip8.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PUBLIC
	-Wall
	-Wextra
	-Wpedantic
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_SOURCE_DIR}/include
	${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
