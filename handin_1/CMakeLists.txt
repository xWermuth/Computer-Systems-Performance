cmake_minimum_required(VERSION 3.5)

project(handin_1)


set(SOURCES
    src/utils.cpp
    src/main.cpp
    include/parallel_buffer.h
    include/concurrent_output.h
    src/parallel_buffer.cpp
    src/concurrent_output.cpp
)

add_executable(handin_1 ${SOURCES})

set(OPENSSL_ROOT_DIR "/Users/wachs/vcpkg/packages/openssl_arm64-osx")
find_package(OpenSSL REQUIRED)
target_link_libraries(handin_1 PRIVATE OpenSSL::SSL OpenSSL::Crypto)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -Ofast")

if(UNIX AND NOT APPLE)
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D PERF_TIMEOUT_SERVER -pthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()


target_include_directories(handin_1
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)