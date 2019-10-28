cmake_minimum_required(VERSION 3.1)
project(DS_PART9)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
    message("Using [Debug] mode")
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message("Using [Release] mode")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 图类算法
# word_ladder
set(DEMO word_ladder)
set(LIB ../lib)
set(SOURCE
        ${DEMO}.cpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})