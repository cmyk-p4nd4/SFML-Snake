cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME sfml-snake)
project(${PROJECT_NAME} VERSION 1.0)

# CPP FLAGS
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -std=c++11 -pthread")

# Dependencies
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/depends)
set(SFML_DEP ${PROJECT_SOURCE_DIR}/depends/SFML)

set(SFML_INCLUDE_DIR ${SFML_DEP}/include)
set(SFML_LIB_DIR ${SFML_DEP}/lib)
set(SFML_DIR ${SFML_DEP}/lib/cmake/SFML)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
find_package(OpenGL REQUIRED)

if (SFML_FOUND)
    link_directories(${SFML_LIBRARY_DIR})
    include_directories(${SFML_INCLUDE_DIR})
endif()
if (NOT OPENGL_FOUND)
    message(WARNING "OpenGL Not Found!")
endif()

# Target
file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

configure_file(resources/brick_wall.png brick_wall.png COPYONLY)
configure_file(resources/VCR_OSD_MONO.ttf VCR_OSD_MONO.ttf COPYONLY)

include_directories(src)