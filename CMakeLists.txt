cmake_minimum_required(VERSION 3.10)
project(Chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED IMPORTED_TARGET notcurses)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/inputOutput.cpp
    src/Cpu.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::NOTCURSES)
