Cmake_minimum_required(VERSION 2.8)
project(scigma)

add_subdirectory(external)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_MACOSX_RPATH OFF)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Weverything -Wno-c++98-compat-pedantic -Wno-documentation -Wno-unknown-pragmas")
  message("Using OpenGL 3.2+")								
  add_definitions(-DSCIGMA_USE_OPENGL_3_2)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic -Wextra -Werror -Wno-unknown-pragmas")	
endif()

include_directories(external/glew)
include_directories(external/glfw3/include/GLFW)
add_subdirectory(gui)

add_library(popup SHARED popup.cpp $<TARGET_OBJECTS:guiobjects>)
target_link_libraries(popup glew glfw ${GLFW_LIBRARIES})
