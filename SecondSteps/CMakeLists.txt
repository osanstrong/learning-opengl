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

set_target_properties(util PROPERTIES LINKER_LANGUAGE CXX)
