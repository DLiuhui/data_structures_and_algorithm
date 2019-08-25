cmake_minimum_required(VERSION 3.1)
project(DS_PART2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# binarySearch
set(DEMO "binarySearch")
set(SOURCE
        ${DEMO}Test.cpp
        ${DEMO}.h)
add_executable(${DEMO} ${SOURCE})

# pow
set(DEMO "pow")
set(SOURCE ${DEMO}.cpp)
add_executable(${DEMO} ${SOURCE})

# maxSum
set(DEMO "maxSum")
set(SOURCE ${DEMO}Test.cpp)
add_executable(${DEMO} ${SOURCE})