cmake_minimum_required(VERSION 3.10)
project(optHirrygated)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Add source directory
set(SRC_DIR src)
set(INCLUDE_DIR include)

# Diretórios do CPLEX/Concert
set(SYSTEM x86-64_linux)
set(LIBFORMAT static_pic)
set(CPLEXDIR /opt/ibm/ILOG/CPLEX_Studio2211/cplex)
set(CONCERTDIR /opt/ibm/ILOG/CPLEX_Studio2211/concert)
set(CPLEX_INCLUDE_DIR "${CPLEXDIR}/include")
set(CONCERT_INCLUDE_DIR "${CONCERTDIR}/include")
set(CPLEX_LIB_DIR "${CPLEXDIR}/lib/${SYSTEM}/${LIBFORMAT}")
set(CONCERT_LIB_DIR "${CONCERTDIR}/lib/${SYSTEM}/${LIBFORMAT}")

# Find OpenXLSX in lib/OpenXLSX
add_subdirectory(lib/OpenXLSX)

# Include directories
include_directories(
        ${INCLUDE_DIR}
        lib/OpenXLSX/include
        ${CPLEX_INCLUDE_DIR}
        ${CONCERT_INCLUDE_DIR}       
        )


# Diretórios de link
link_directories(
    ${CPLEX_LIB_DIR}
    ${CONCERT_LIB_DIR}
)


# Define o compilador para aceitar ILOG CPLEX padrão C++
add_definitions(-DIL_STD)

# Find all .cpp files in src/
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenXLSX library
target_link_libraries(${PROJECT_NAME}
    OpenXLSX
    ilocplex
    cplex
    concert
    pthread
    dl    
    )



