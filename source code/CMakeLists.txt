cmake_minimum_required(VERSION 3.24)
project(Stock)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

# Include headers
include_directories(include)

set(SOURCES_FILES
    main.cpp
    Stock.cpp
    Market.cpp
)

set(HEADER_FILES
    Market.h
    Stock.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES_FILES}
    ${HEADER_FILES}
)