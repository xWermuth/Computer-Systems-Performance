cmake_minimum_required(VERSION 3.5)

project(handin_1)


set(SOURCES
    src/utils.cpp
    src/main.cpp
)



add_executable(handin_1 ${SOURCES})

set(OPENSSL_ROOT_DIR "/Users/wachs/vcpkg/packages/openssl_arm64-osx")
find_package(OpenSSL REQUIRED)
target_link_libraries(handin_1 PRIVATE OpenSSL::SSL OpenSSL::Crypto)


set(CMAKE_CXX_FLAGS "-std=gnu++17 -O2 ${CMAKE_CXX_FLAGS}")

target_include_directories(handin_1
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)