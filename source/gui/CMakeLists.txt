include_directories(../external/glew)
include_directories(../external/glfw3/include/GLFW)
add_library(guiobjects OBJECT application.cpp)
set_target_properties(guiobjects PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
