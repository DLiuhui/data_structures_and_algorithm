cmake_minimum_required(VERSION 3.1)
project(DS_PART6)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
    message("Using [Debug] mode")
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message("Using [Release] mode")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# heap 堆 / priority queue 优先队列

# binary_heap 最基本的二叉堆
set(DEMO binary_heap)
set(LIB ../lib)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# leftist_heap 左式堆
set(DEMO left_heap)
set(LIB ../lib)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# binomial_queue 二项队列
set(DEMO binomial_queue)
set(LIB ../lib)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# priority_queue 使用
set(DEMO priority_queue)
set(SOURCE ${DEMO}_test.cpp)
add_executable(${DEMO} ${SOURCE})
