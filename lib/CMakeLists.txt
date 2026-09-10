## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(${PROJECT_BINARY_DIR}/lib ${PROJECT_SOURCE_DIR}/lib)

file(GLOB _src pujOpenGL/*.cxx)
add_library(pujOpenGL SHARED ${_src})
target_link_libraries(pujOpenGL PUBLIC OpenGL::GL OpenGL::GLU GLUT::GLUT)

## eof - CMakeLists.txt
