cmake_minimum_required(VERSION 3.10)

project(contract_violation_proposal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Create library for contract_violation
add_library(contract_violation contract_violation.cpp)
target_include_directories(contract_violation PUBLIC include)

# Create test executable
add_executable(contract_violation_test contract_violation_test.cpp)

# Link contract_violation library to the test executable
target_link_libraries(contract_violation_test contract_violation)

