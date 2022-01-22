/**
 * @file get_wap.hpp
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief Declarations for functions relating to getting nearby WAP info
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __WGEO_FILE_GET_WAP_HPP__
#define __WGEO_FILE_GET_WAP_HPP__

#include <vector>
#include <string>
#include <ostream>
#include "wap_data.hpp"

#define WGEO_ERR_ACCESS_INTERFACE_FAILED -1
#define WGEO_ERR_ENUM_INTERFACE_FAILED -2
#define WGEO_ERR_DEFAULT_ADAPTER_FAILED -3
#define WGEO_ERR_NOTIFY_FAILED -4
#define WGEO_ERR_ADAPTER_DISABLED -5
#define WGEO_ERR_NO_NETWORK_LIST -6
#define WGEO_ERR_BSS_FAILED -7
#define WGEO_ERR_NO_WAP_FOUND -8

/**
 * @brief Get information about nearby WAPs
 * 
 * @param wap_vect vector of wap_info structs
 * @return int the return code
 */
int get_wap_info(std::vector<wap_data>& wap_vect) noexcept;

#endif