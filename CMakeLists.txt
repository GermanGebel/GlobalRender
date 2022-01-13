cmake_minimum_required(VERSION 3.19)
project(RayTracing)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -fopenmp")
set(${PROJECT_NAME}_SOURCE_FILES
        Math.h
        Camera.h
        Camera.cpp
        Geometry.h
        Geometry.cpp
#        Scene.h Удалено из-за нарушения ODR, необходимо восстанавливать по мере разработки, за пояснениями -> К Юле
#        Scene.cpp Удалено из-за нарушения ODR, необходимо восстанавливать по мере разработки, за пояснениями -> К Юле
#        Light.h Удалено из-за нарушения ODR, необходимо восстанавливать по мере разработки, за пояснениями -> К Юле
#        Light.cpp Удалено из-за нарушения ODR, необходимо восстанавливать по мере разработки, за пояснениями -> К Юле
#        structs.h Удалено из-за нарушения ODR , необходимо восстанавливать по мере разработки, за пояснениями -> К Юле
        )
add_executable(main main.cpp ${${PROJECT_NAME}_SOURCE_FILES})
