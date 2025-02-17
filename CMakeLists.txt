cmake_minimum_required(VERSION 3.10)
project(optHirrygated)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Add source directory
set(SRC_DIR src)
set(INCLUDE_DIR include)

# Find OpenXLSX in lib/OpenXLSX
add_subdirectory(lib/OpenXLSX)

# Include directories
include_directories(${INCLUDE_DIR} lib/OpenXLSX/include)

# Find all .cpp files in src/
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenXLSX library
target_link_libraries(${PROJECT_NAME} OpenXLSX)
