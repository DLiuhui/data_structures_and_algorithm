cmake_minimum_required(VERSION 3.1)
project(DS_PART7)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
    message("Using [Debug] mode")
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message("Using [Release] mode")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# sort 排序
set(DEMO sort)
set(LIB ../lib)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# radix_sort 基数排序
set(DEMO radix_sort)
set(LIB ../lib)
set(SOURCE
        ${DEMO}.cpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})