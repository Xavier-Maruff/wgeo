/**
 * @file api.hpp
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief Contains the API declarations
 * @version 0.1
 * @date 2021-11-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __WGEO_FILE_API_HPP__
#define __WGEO_FILE_API_HPP__

#include <string>
#include <vector>
#include "wgeo/wgeo.h"
#include "get_wap.hpp"

/**
 * @brief The API host domain
 * 
 */
extern std::string api_host;

/**
 * @brief The API endpoint route
 * 
 */
extern std::string api_route_fmt;

/**
 * @brief Get the API response body as a string
 * 
 * @param wap_vect vector of wireless access points
 * @return std::string response body
 */
std::string api_response(std::vector<wap_data>& wap_vect);

#endif