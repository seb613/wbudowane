cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

# Set the path to your vcpkg installation
set(CMAKE_TOOLCHAIN_FILE "A/microsoft-vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

add_executable(helloWorld main.c)

# Link the libraries
find_package(SDL2 REQUIRED)
target_link_libraries(helloWorld PRIVATE SDL2::SDL2)