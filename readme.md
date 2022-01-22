# wgeo
Wgeo, or "wi-fi geolocator", is a cross-platform C/C++ library for wifi-based device geolocation, utilising public wireless access point location databases.

## compatibility
| OS | Compatible |
| --- | --- |
| Windows 10 | ✅ |
| Windows 7 | ✅ |
| Linux* | ✅ |
| macOS | Soon |

<sub>\* - The linux implementation requires root permissions in order to perform a WAP scan.</sub>
## usage
Wgeo exposes a simple C API which abstracts the actual geolocation process. The API consists of two functions:
```c
//sets the wgeo api host, route, and key
//successful if return value == WGEO_SUCCESS
int wgeo_set_api_req(char* host, char* route, char* api_key);

//scans WAPs, queries API, writes resultant latitude and longitude (+ result accuracy) to the supplied pointers
//successful if return value == WGEO_SUCCESS
int wgeo_get_location(double* lat, double* lon, double* accuracy);
```
If you would like to use wgeo as a shared library, ensure that you define the `__WGEO_SHARED__` macro before including the header file (check the `examples` folder).

## building
The wgeo repo provides bash and batch build scripts, which require CMake to be installed.

Usage:
```bash 
./tools/build.sh {{Debug || Release || undefined}}
```
```batch
tools\build {{Debug || Release || undefined}}
```

### platform-independent external dependencies:
* Boost
* OpenSSL

### linux additional dependencies:
* libnl-3.0
* libnl-genl-3.0

## testing
Wgeo utilises GTest for testing, and can be run using the bash or batch `test` scripts, which rely on `ctest`.
To set up your environment, create the file `tools/env.txt` cat write your Google geolocation API key to it.

To run the tests:
```bash
sudo ./tools/test.sh {{Debug || Release || undefined}}
```
```batch
tools\test {{Debug || Release || undefined}}
```

## full usage example
More examples can be found in the `examples` directory.
```c
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
```