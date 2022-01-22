/**
 * @file wap_data.cpp
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief Contains the wap_data struct definitions
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "wap_data.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

wap_data::wap_data() noexcept:
ssid(), strength(0), bssid() {
    //
}

wap_data::wap_data(const std::string ssid, uint8_t* bssid, size_t bssid_size, const double strength) noexcept :
    ssid(ssid), strength(strength) {
    while(bssid_size--) {
        this->bssid.push_back(*bssid++);
    }
}

wap_data::wap_data(const std::string ssid, std::vector<uint8_t> bssid, const double strength) noexcept :
    ssid(ssid), bssid(bssid), strength(strength) {
    //
}

wap_data::wap_data(wap_data& w) noexcept :
    ssid(w.ssid), bssid(w.bssid), strength(w.strength) {
    //
}

wap_data::wap_data(wap_data&& w) noexcept :
    ssid(std::move(w.ssid)), bssid(std::move(w.bssid)), strength(std::move(w.strength)) {
    //
}

wap_data::~wap_data() noexcept {
    //
}

std::string wap_data::bssid_fmt() const noexcept {
    std::stringstream fmt_stream;
    fmt_stream << std::uppercase << std::hex << std::setfill('0');
    for(const uint8_t& c : bssid) {
        fmt_stream << std::setw(2) << int(c) << ":";
    }
    return fmt_stream.str().substr(0, fmt_stream.str().length() - 1);
}

std::ostream& operator<<(std::ostream& os, const wap_data& w) noexcept {
    os << "{\"ESSID\": \"" << w.ssid << "\", \"macAddress\": \"" << w.bssid_fmt() << "\", \"signalStrength\": " << w.strength << "}";
    return os;
}