## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(${PROJECT_BINARY_DIR}/lib ${PROJECT_SOURCE_DIR}/lib)
add_executable(pujOpenGL_OBJViewer OBJViewer.cxx)
target_link_libraries(pujOpenGL_OBJViewer PRIVATE pujOpenGL)

## eof - CMakeLists.txt
