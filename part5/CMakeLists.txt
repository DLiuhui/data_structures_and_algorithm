cmake_minimum_required(VERSION 3.1)
project(DS_PART5)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
    message("Using [Debug] mode")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message("Not using [Debug] mode")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# three type hash table 三种hash方案

# separate chaining 分离链接型
set(DEMO separate_chaining)
set(LIB ../lib/dsexceptions.h)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# quadratic probing 二次再探型
set(DEMO quadratic_probing)
set(LIB ../lib/dsexceptions.h)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})

# cuckoo hash family
set(DEMO cuckoo_hash)
set(LIB ../lib)
set(SOURCE
        ${DEMO}_test.cpp
        ${DEMO}.hpp
        ${LIB})
add_executable(${DEMO} ${SOURCE})
