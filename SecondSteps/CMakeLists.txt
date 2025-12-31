cmake_minimum_required(VERSION 3.23)

project(TextureTest)

add_executable(main)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3)

target_sources(main
    PRIVATE
        src/GLT02.cc
)

target_link_libraries(main
    PRIVATE
        util
        OpenGL::GL
        glfw
        glad
)

add_library(util)
target_sources(util
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/stb_image.hh
)

add_library(glad)
target_sources(glad
    PRIVATE
        src/glad.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/glad/glad.h
            include/KHR/khrplatform.h
)

set_target_properties(util PROPERTIES LINKER_LANGUAGE CXX)
