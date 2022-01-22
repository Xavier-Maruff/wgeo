#include <gtest/gtest.h>
#include "wgeo/wgeo.h"
#include <cstdlib>
#include <fstream>
#include <string>

#define noconst(O) const_cast<char*>(O)

#ifndef CONFIG_PATH
    #define CONFIG_PATH "./tools/env.txt"
#endif

static const char* api_host = "https://www.googleapis.com";
static const char* api_route = "/geolocation/v1/geolocate?key=%s";
static std::string api_key;


TEST(api_success, set_api_req) {
    std::ifstream env_config(CONFIG_PATH);
    if(!env_config.is_open())
        FAIL() << "COULD NOT OPEN ENV CONFIG FILE";
    std::getline(env_config, api_key);
    EXPECT_EQ(wgeo_set_api_req(noconst(api_host), noconst(api_route), noconst(api_key.c_str())), WGEO_SUCCESS)
        << "COULD NOT SET API REQUEST";
}

TEST(api_success, get_location){
    double lat = 0, lon = 0, accuracy = 0;
    EXPECT_EQ(wgeo_get_location(&lat, &lon, &accuracy), WGEO_SUCCESS)
        << "COULD NOT GET LOCATION";
}