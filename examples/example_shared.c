#define __WGEO_SHARED__

#include <wgeo/wgeo.h>
#include <stdio.h>

#define API_HOST "https://www.googleapis.com"
#define API_ROUTE "/geolocation/v1/geolocate?key=%s"
#define MY_API_KEY "some_garbled_characters"


int main(int argc, char** argv){
    double lat = 0, lon = 0, accuracy = 0;
    int wgeo_status = 0;

    wgeo_status = wgeo_set_api_req(API_HOST,  API_ROUTE,  MY_API_KEY);
    if(wgeo_status != WGEO_SUCCESS) {
        perror("error: could not set up API request\n");
        return -1;
    }

    wgeo_status = wgeo_get_location(&lat, &lon, &accuracy);
    if(wgeo_status != WGEO_SUCCESS){
        perror("error: could not get location\n");
        return -1;
    }

    printf("latitude, longitude: (%f, %f)\n", lat, lon);
    printf("accuracy: %f\n", accuracy);
    return 0;
}