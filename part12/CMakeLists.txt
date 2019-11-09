cmake_minimum_required(VERSION 3.1)
project(DS_PART12)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
    message("Using [Debug] mode")
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message("Using [Release] mode")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 高级数据结构
# RedBlackTree 红黑树
set(DEMO rb_tree)
set(LIB ../lib)
set(SOURCE
        ${DEMO}.hpp
        ${DEMO}_test.cpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# Treap 树堆
set(DEMO treap)
set(LIB ../lib)
set(SOURCE
        ${DEMO}.hpp
        ${DEMO}_test.cpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# KdTree k维搜索树
set(DEMO kd_tree)
set(LIB ../lib)
set(SOURCE
        ${DEMO}.hpp
        ${DEMO}_test.cpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# PairingHeap 配对堆
set(DEMO pairing_heap)
set(LIB ../lib)
set(SOURCE
        ${DEMO}.hpp
        ${DEMO}_test.cpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})


