set(CMAKE_C_COMPILER "i686-elf-gcc")
set(CMAKE_CXX_COMPILER "i686-elf-gcc")

project(SynapseOS)

cmake_minimum_required(VERSION 3.16)


set(BINARY_DIR ./bin/)
set(SOURCE_DIR ./src/)
set(MODULES_DIR ./src/modules/)
set(HEADERS_DIR ./src/include/)

set(CMAKE_INSTALL_PREFIX ${BINARY_DIR})

include_directories(${BINARY_DIR} ${SOURCE_DIR})

file(GLOB CS "*.c")

add_definitions(-O3 -std=gnu99 -ffreestanding -Wall -Wextra )

add_executable(kernel ${CS})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)










