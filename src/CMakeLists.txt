cmake_minimum_required(VERSION 3.10)
project(wgeo VERSION 1.0 DESCRIPTION "Wifi based geolocation library")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

#WAPI library
if(UNIX AND NOT APPLE)
    set(wapi_dir ${CMAKE_CURRENT_SOURCE_DIR}/../wapi)
    include_directories(${wapi_dir}/include)

    add_library(wapi STATIC
        ${wapi_dir}/src/network.c
        ${wapi_dir}/src/util.c
        ${wapi_dir}/src/wireless.c
    )

    target_include_directories(wapi PUBLIC ${wapi_dir}/src /usr/include/libnl3)
    target_link_libraries(wapi nl-3 iw m nl-genl-3)
    target_compile_definitions(wapi PUBLIC -DLIBNL3)
endif(UNIX AND NOT APPLE)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cpp-httplib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../lib)

set(wgeosrc
    ${wgeosrc}
    ${CMAKE_CURRENT_SOURCE_DIR}/wgeo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/get_wap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wap_data.cpp
)

add_library(wgeo_static STATIC ${wgeosrc})
add_library(wgeo_shared SHARED ${wgeosrc})

target_compile_definitions(wgeo_shared PRIVATE __WGEO_SHARED__  __WGEO_EXPORTS__)

if(WIN32)
    target_link_libraries(wgeo_static wlanapi ${OPENSSL_LIBRARIES})
    target_link_libraries(wgeo_shared wlanapi ${OPENSSL_LIBRARIES})
elseif(UNIX AND NOT APPLE)
    target_link_libraries(wgeo_static wapi)
    target_link_libraries(wgeo_shared wapi)
else()
#macOS
endif()