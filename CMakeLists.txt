cmake_minimum_required(VERSION 3.21)

project(blackhole)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(DEPS OpenGL::GL glfw GLEW::GLEW glm::glm)

set(SOURCES main.cpp ray.cpp physics.cpp rendering.cpp)
set(HEADERS constants.h ray.h physics.h rendering.h)

add_executable(blackhole ${SOURCES} ${HEADERS})

target_link_libraries(blackhole PRIVATE ${DEPS})