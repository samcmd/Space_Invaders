cmake_minimum_required(VERSION 3.17)
project(M4OEP_Graphics2_smschult)

if (WIN32)
    set(FREEGLUT_INCLUDE_DIRS "C:/Program\ Files/Common\ Files/freeglut/include")
    set(FREEGLUT_LIBRARY_DIRS "C:/Program\ Files/Common\ Files/freeglut/lib/x64")
endif (WIN32)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

find_package (OpenGL REQUIRED)

if (APPLE)
    find_package(GLUT REQUIRED)
endif (APPLE)

if (WIN32)
    include_directories(${OPENGL_INCLUDE_DIR}  ${FREEGLUT_INCLUDE_DIRS})
    link_directories(${FREEGLUT_LIBRARY_DIRS})
elseif (APPLE)
    include_directories(${OPENGL_INCLUDE_DIR}  ${GLUT_INCLUDE_DIRS})
endif ()

file(GLOB SOURCE_FILES
        *.cpp
        *.h
        )

add_executable(M4OEP_Graphics2_smschult graphics.h shape.h shape.cpp rect.h rect.cpp graphics.cpp circle.h circle.cpp)

if (WIN32)
    target_link_libraries (M4OEP_Graphics2_smschult ${OPENGL_LIBRARIES} freeglut)
elseif (APPLE)
    target_link_libraries (M4OEP_Graphics2_smschult ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
endif ()