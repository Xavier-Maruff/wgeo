/**
 * @file wgeo.h
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief Contains the wgeo C API declarations
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __WGEO_FILE_WGEO_H__
#define __WGEO_FILE_WGEO_H__

#ifdef __cplusplus
#include <cstdio>
extern "C" {
#else
#include <stdio.h>
#endif

#if defined(__WGEO_SHARED__)

    #if defined(__WGEO_EXPORTS__)
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            #define WGEO_API  __declspec(dllexport)
        #elif __linux__
            #define WGEO_API  __attribute__((visibility("default")))
        #else
            #define WGEO_API
        #endif

    #else

        #ifdef _WIN32
            #define WGEO_API  __declspec(dllimport)
        #else
            #define WGEO_API
        #endif

    #endif

#else
    #define WGEO_API
#endif

#define WGEO_SUCCESS 1
#define WGEO_ERR_API -1
#define WGEO_ERR_UNKNOWN -2
#define WGEO_ERR_NULLPTR -3
#define WGEO_ERR_INVALID_PLATFORM -4
#define WGEO_ERR_API_REQ_NOT_SET -5

/**
 * @brief Sets up the API route and key
 * 
 * @param host the API host
 * @param route the API route printf-style formatted for the API key
 * @param api_key API key to use
 * @return int return code
 */
WGEO_API int wgeo_set_api_req(char* host, char* route, char* api_key);

/**
 * @brief Geolocates the device
 * 
 * @param lat The latitude of the device
 * @param lon The longitude of the device
 * @param accuracy The accuracy of the returned latitude and longitude
 * @return int Success or failure code
 */
WGEO_API int wgeo_get_location(double* lat, double* lon, double* accuracy);


#ifdef __cplusplus
}
#endif

#endif