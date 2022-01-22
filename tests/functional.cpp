#include <gtest/gtest.h>
#include "wgeo/wgeo.h"
#include <cstdlib>
#include <fstream>

#define noconst(O) const_cast<char*>(O)

#ifndef CONFIG_PATH
    #define CONFIG_PATH "./env.txt"
#endif

TEST(functional, get_location) {
    std::ifstream env_config(CONFIG_PATH);
    std::string env_var;

    double lat = 0, lon = 0, accuracy = 0;
    std::getline(env_config, env_var);
    const char* api_key = env_var.c_str();
    const char* api_host = "https://www.googleapis.com";
    const char* api_route = "/geolocation/v1/geolocate?key=%s";

    EXPECT_EQ(wgeo_set_api_req(noconst(api_host), noconst(api_route), noconst(api_key)), WGEO_SUCCESS);
    EXPECT_EQ(wgeo_get_location(&lat, &lon, &accuracy), WGEO_SUCCESS);
    
    std::cout << "lat, lon: (" << lat << ", " << lon << ")\n";

    EXPECT_NEAR(lat, -37.893208, 0.01);
    EXPECT_NEAR(lon, 145.021780, 0.01);

}