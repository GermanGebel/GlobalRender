cmake_minimum_required(VERSION 3.19)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -fopenmp")
set(${PROJECT_NAME}_SOURCE_FILES
        structs.h
        Math.h
        Camera.h
        Camera.cpp
        Color.h
        Color.cpp
        Geometry.h
        Geometry.cpp
        Ray.h
        Light.h
        Light.cpp
#        Render.h
#        Render.cpp
        Scene.h
        Scene.cpp
        )
add_executable(main main.cpp ${${PROJECT_NAME}_SOURCE_FILES})