/**
 * @file api.cpp
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief Contains the API definitions
 * @version 0.1
 * @date 2021-11-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include "api.hpp"
#include <string>
#include <sstream>
#include <memory>

bool client_setup = false;
std::unique_ptr<httplib::Client> http_client;

std::string api_host;
std::string api_route_fmt;
std::pair<std::string, std::string> api_req_shell = {
    "{\"wifiAccessPoints\": [",
    "]}"
};

std::string api_response(std::vector<wap_data>& wap_vect) {
    if(!client_setup) {
        http_client = std::make_unique<httplib::Client>(api_host);
        client_setup = true;
    }

    std::stringstream req_body_stream;
    req_body_stream << api_req_shell.first;
    for(wap_data& w: wap_vect){
        req_body_stream << w << ", ";
    }
    req_body_stream << api_req_shell.second;
    std::string req_body = req_body_stream.str();

    auto res = http_client->Post(api_route_fmt.c_str(), req_body.c_str(), "application/json");
    return res.value().body;
}