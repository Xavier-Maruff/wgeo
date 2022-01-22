cmake_minimum_required(VERSION 3.10)
project(wgeo VERSION 1.0 DESCRIPTION "Wifi based geolocation library")

find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    wgeo_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/api_success.cpp
)
target_link_libraries(
  wgeo_tests
  wgeo_static
  ${OPENSSL_LIBRARIES}
  gtest_main
)
target_compile_definitions(wgeo_tests PRIVATE -DCONFIG_PATH="tools/env.txt")

include(GoogleTest)
gtest_discover_tests(wgeo_tests)