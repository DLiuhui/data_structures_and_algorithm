cmake_minimum_required(VERSION 3.3)
project(DS_PART1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# find_max
set(DEMO "findMax")
add_executable(${DEMO} ${DEMO}.cpp)

# IntCell
set(DEMO "IntCell")
set(SOURCE
        ${DEMO}Test.cpp
        ${DEMO}.h
        ${DEMO}.cpp)
add_executable(${DEMO} ${SOURCE})

# LambdaExample
set(DEMO "LambdaExample")
set(SOURCE ${DEMO}.cpp)
add_executable(${DEMO} ${SOURCE})

# matrix
set(DEMO "matrix")
set(SOURCE
        ${DEMO}Test.cpp
        ${DEMO}.h)
add_executable(${DEMO} ${SOURCE})

# printDigits
add_executable(printDigits printDigits.cpp)

# square
set(DEMO "square")
set(SOURCE
        ${DEMO}Test.cpp
        ${DEMO}.h)
add_executable(${DEMO} ${SOURCE})