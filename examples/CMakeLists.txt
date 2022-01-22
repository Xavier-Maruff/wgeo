cmake_minimum_required(VERSION 3.10)
project(wgeo VERSION 1.0 DESCRIPTION "Wifi based geolocation library")

find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(
    c_example
    ${CMAKE_CURRENT_SOURCE_DIR}/example.c
)
target_link_libraries(
  c_example
  wgeo_static
  ${OPENSSL_LIBRARIES}
)

add_executable(
    cxx_example
    ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp
)
target_link_libraries(
  cxx_example
  wgeo_static
  ${OPENSSL_LIBRARIES}
)

add_executable(
    c_example_shared
    ${CMAKE_CURRENT_SOURCE_DIR}/example_shared.c
)
target_link_libraries(
  c_example_shared
  wgeo_shared
  ${OPENSSL_LIBRARIES}
)