#include <wgeo/wgeo.h>
#include <iostream>
 
#define API_HOST "https://www.googleapis.com"
#define API_ROUTE "/geolocation/v1/geolocate?key=%s"
#define MY_API_KEY "some_garbled_characters"

int main(int argc, char* argv[]){
    double lat = 0, lon = 0, accuracy = 0;
    int wgeo_status = 0;

    wgeo_status = wgeo_set_api_req(const_cast<char*>(API_HOST),  const_cast<char*>(API_ROUTE),  const_cast<char*>(MY_API_KEY));
    if(wgeo_status != WGEO_SUCCESS) {
        std::cerr << "error: could not set up API request\n";
        return -1;
    }

    wgeo_status = wgeo_get_location(&lat, &lon, &accuracy);
    if(wgeo_status != WGEO_SUCCESS){
        std::cerr << "error: could not get location\n";
        return -1;
    }

    std::cout << "latitude, longitude: (" << lat << ", " << lon << ")\n";
    std::cout << "accuracy: " << accuracy << "\n";
    return 0;
}