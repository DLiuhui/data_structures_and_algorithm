cmake_minimum_required(VERSION 3.1)
project(DS_PART3)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
    message("Using [Debug] mode")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message("Not using [Debug] mode")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# binary_search_tree
set(DEMO binary_search_tree)
set(LIB ../lib/dsexceptions.h)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})