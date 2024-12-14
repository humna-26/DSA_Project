cmake_minimum_required(VERSION 3.10)

# Set project name
project(ChessProject)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set the source directory where the .cpp and .h files are located
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# Optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -funroll-loops -flto -fomit-frame-pointer -ffast-math")

# Add the executable target from the .cpp files in src directory
add_executable(ChessProject
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/PieceUtil.cpp
    ${SRC_DIR}/Board.cpp
    ${SRC_DIR}/MoveUtil.cpp
    ${SRC_DIR}/MoveList.cpp
    ${SRC_DIR}/Evaluation.cpp
    ${SRC_DIR}/uci.cpp
    ${SRC_DIR}/SearchUtil.cpp
)

# Include header files from the src directory
include_directories(${SRC_DIR})

# Set output directory for the final executable
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})