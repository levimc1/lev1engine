cmake_minimum_required(VERSION 3.16)
project(lev1engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(code)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS code/*.cpp)
file(GLOB_RECURSE HDR CONFIGURE_DEPENDS code/*.hpp)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)
message(STATUS "Found GLFW includes: ${GLFW_INCLUDE_DIRS}")
message(STATUS "Found GLFW libs: ${GLFW_LIBRARIES}")

pkg_check_modules(GLEW REQUIRED glew)
message(STATUS "Found GLEW includes: ${GLEW_INCLUDE_DIRS}")
message(STATUS "Found GLEW libs: ${GLEW_LIBRARIES}")

find_package(OpenGL REQUIRED)

add_executable(engine ${SRC} ${HDR})

target_include_directories(engine PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(engine PRIVATE
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    OpenGL::GL
)