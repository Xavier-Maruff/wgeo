/**
 * @file wap_data.hpp
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief Contains the declaration of the wap_data struct
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __WGEO_FILE_WAP_DATA_HPP__
#define __WGEO_FILE_WAP_DATA_HPP__

#include <string>
#include <vector>
#include <iostream>

/**
 * @brief Contains information about a wireless access point (ESSID, BSSID, signal strength)
 * 
 */
typedef struct wap_data {
    /**
     * @brief WAP SSID
     * 
     */
    std::string ssid;

    /**
     * @brief WAP BSSID (MAC address)
     * 
     */
    std::vector<uint8_t> bssid;

    /**
     * @brief WAP signal strength
     * 
     */
    double strength;

    wap_data() noexcept;
    wap_data(const std::string ssid, const std::vector<uint8_t> bssid, const double strength) noexcept;
    wap_data(const std::string ssid, uint8_t* bssid, size_t bssid_size, const double strength) noexcept;
    wap_data(wap_data& w) noexcept;
    wap_data(wap_data&& w) noexcept;
    ~wap_data() noexcept;

    /**
     * @brief Format the BSSID as a MAC address string
     * 
     * @return std::string the formatted BSSID
     */
    std::string bssid_fmt() const noexcept;
} wap_data;

/**
 * @brief JSON representation of a WAP information struct
 * 
 * @param os output stream
 * @param w WAP information struct
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const wap_data& w) noexcept;

#endif