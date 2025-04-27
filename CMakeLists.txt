cmake_minimum_required(VERSION 3.24)
project(Stock)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

# Include headers
include_directories(include)

set(SOURCES_FILES
    src/main.cpp
    src/Stock.cpp
    src/Market.cpp
)

set(HEADER_FILES
    include/Market.h
    include/Stock.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES_FILES}
    ${HEADER_FILES}
)