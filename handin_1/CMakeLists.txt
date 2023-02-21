cmake_minimum_required(VERSION 3.5)

project(handin_1)

add_executable(handin_1 main.cpp)


set(OPENSSL_ROOT_DIR "/Users/wachs/vcpkg/packages/openssl_arm64-osx")
find_package(OpenSSL REQUIRED)
target_link_libraries(handin_1 PRIVATE OpenSSL::SSL OpenSSL::Crypto)