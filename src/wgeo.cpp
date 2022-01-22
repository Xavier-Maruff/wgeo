/**
 * @file wgeo.cpp
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief WGEO exposed function definitions
 * @version 0.1
 * @date 2021-11-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "wgeo/wgeo.h"
#include "get_wap.hpp"
#include "api.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <boost/property_tree/json_parser.hpp>

namespace ptree = boost::property_tree;

static bool api_req_set = false;
static std::vector<wap_data> wap_vect;

int wgeo_set_api_req(char* host, char* route, char* api_key){
    if(!host || !route || !api_key) return WGEO_ERR_NULLPTR;

    size_t api_route_buff_len = std::strlen(route)+std::strlen(api_key);
    char* api_route_buff = new char[api_route_buff_len];
    std::snprintf(api_route_buff, api_route_buff_len*sizeof(char), route, api_key);

    api_route_fmt = api_route_buff;
    api_host = host;

    delete[] api_route_buff;

    api_req_set = true;
    return WGEO_SUCCESS;
}


int wgeo_get_location(double *lat, double *lon, double* accuracy){//, void** collected_wap_data, bool wap_data_only){ 
    wap_vect.clear();
    int ret_code = get_wap_info(wap_vect);

    if(ret_code != WGEO_SUCCESS) return ret_code;
    if(wap_vect.size() == 0) return WGEO_ERR_NO_WAP_FOUND;
    //if(collected_wap_data) *collected_wap_data = (void*)&wap_vect;

    //if(wap_data_only){
    //    if(!collected_wap_data) return WGEO_ERR_NULLPTR;
    //    return WGEO_SUCCESS;
    //}

    if(!lat || !lon || !accuracy) return WGEO_ERR_NULLPTR;
    if(!api_req_set) return WGEO_ERR_API_REQ_NOT_SET;

    std::stringstream response_json;
    response_json << api_response(wap_vect);

    ptree::ptree response_ptree;
    try{
        ptree::read_json(response_json, response_ptree);
    } catch(ptree::ptree_error& e){
        return WGEO_ERR_API;
    }

    auto error_code = response_ptree.get_optional<int>("error.code");
    if(error_code.is_initialized())
        return WGEO_ERR_API;

    auto lat_option = response_ptree.get_optional<double>("location.lat");
    auto lon_option = response_ptree.get_optional<double>("location.lng");
    auto accuracy_option = response_ptree.get_optional<double>("accuracy");

    if(!lat_option.is_initialized() || !lon_option.is_initialized())
        return WGEO_ERR_UNKNOWN;
    
    *lat = lat_option.get();
    *lon = lon_option.get();

    if(accuracy_option.is_initialized())
        *accuracy = accuracy_option.get();
    else
        *accuracy = -1;

    return WGEO_SUCCESS;
}