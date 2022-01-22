/**
 * @file get_wap.cpp
 * @author Xavier Maruff (xavier.maruff@outlook.com)
 * @brief Definitions of wireless access point functions and structs
 * @version 0.1
 * @date 2021-11-03
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "wgeo/wgeo.h"
#include "get_wap.hpp"
#include <iostream>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <windows.h>
#include <wlanapi.h>

int get_wap_info(std::vector<wap_data>& wap_vect) noexcept {
    DWORD ret_code = 0;
    DWORD client_version = 1;

    HANDLE client = nullptr;

    OSVERSIONINFO os_version;
    ZeroMemory(&os_version, sizeof(OSVERSIONINFO));
    os_version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if(!GetVersionEx(&os_version) || os_version.dwMajorVersion < 6)
        client_version = 1;
    else client_version = 2;

    DWORD supported_version = 0;
    GUID guid_interface;
    ZeroMemory(&guid_interface, sizeof(GUID));

    WLAN_INTERFACE_INFO_LIST* interface_list = (WLAN_INTERFACE_INFO_LIST*)WlanAllocateMemory(sizeof(WLAN_INTERFACE_INFO_LIST));
    ZeroMemory(interface_list, sizeof(WLAN_INTERFACE_INFO_LIST));

    WLAN_AVAILABLE_NETWORK_LIST* network_list = (WLAN_AVAILABLE_NETWORK_LIST*)WlanAllocateMemory(sizeof(WLAN_AVAILABLE_NETWORK_LIST));
    ZeroMemory(network_list, sizeof(WLAN_AVAILABLE_NETWORK_LIST));

    ret_code = WlanOpenHandle(client_version, nullptr, &supported_version, &client);
    if(ret_code != ERROR_SUCCESS)
        return WGEO_ERR_ACCESS_INTERFACE_FAILED;

    ret_code = WlanEnumInterfaces(client, nullptr, &interface_list);
    if(ret_code != ERROR_SUCCESS)
        return WGEO_ERR_ENUM_INTERFACE_FAILED;

    if(interface_list->InterfaceInfo[0].isState != wlan_interface_state_connected)
        return WGEO_ERR_DEFAULT_ADAPTER_FAILED;

    guid_interface = interface_list->InterfaceInfo[0].InterfaceGuid;

    WLAN_BSS_LIST* bss_list = (WLAN_BSS_LIST*)WlanAllocateMemory(sizeof(WLAN_BSS_LIST));
    ZeroMemory(bss_list, sizeof(WLAN_BSS_LIST));
    ret_code = WlanGetNetworkBssList(client, &guid_interface, nullptr, dot11_BSS_type_any, false, nullptr, &bss_list);
    if(ret_code != ERROR_SUCCESS)
        return WGEO_ERR_BSS_FAILED;

    wap_vect.clear();

    for(DWORD i = 0; i < bss_list->dwNumberOfItems; i++) {
        wap_vect.push_back(
            wap_data(
                std::string(reinterpret_cast<const char*>(bss_list->wlanBssEntries[i].dot11Ssid.ucSSID)),
                bss_list->wlanBssEntries[i].dot11Bssid,
                6,
                bss_list->wlanBssEntries[i].lRssi
            )
        );
    }

    if(client) WlanCloseHandle(client, nullptr);
    if(interface_list) WlanFreeMemory(interface_list);
    if(network_list) WlanFreeMemory(network_list);
    if(bss_list) WlanFreeMemory(bss_list);

    return WGEO_SUCCESS;

}


#elif __linux__
#include <cstdio>
#include <cstring>
#include <chrono>
#include <ifaddrs.h>
#include <unistd.h>
#include <thread>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <iwlib.h>
#include <errno.h>


int get_wap_info(std::vector<wap_data>& wap_vect) noexcept {
    //get linked list of interfaces
    ifaddrs* initial_interface;
    if (getifaddrs(&initial_interface) == -1)
        return WGEO_ERR_ENUM_INTERFACE_FAILED;

    //first interface
    ifaddrs* current_interface = initial_interface;
    iwreq wrequest;
    for(; current_interface != nullptr; current_interface = current_interface->ifa_next) {
        if (current_interface->ifa_addr == nullptr || current_interface->ifa_addr->sa_family != AF_PACKET)
            continue;

        memset(&wrequest, 0, sizeof(iwreq));
        strncpy(wrequest.ifr_name, current_interface->ifa_name, IFNAMSIZ);
        int interface_socket = socket(AF_INET, SOCK_STREAM, 0);

        if(interface_socket != -1 && ioctl(interface_socket, SIOCGIWNAME, &wrequest) != -1) {
            //got a valid interface, scan for APs
            close(interface_socket);
            if((interface_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
                continue;

            memset(&wrequest, 0, sizeof(iwreq));
            iwrange wrange;
            if(iw_get_range_info(interface_socket, current_interface->ifa_name, &wrange) < 0 || wrange.we_version_compiled < 14){
                close(interface_socket);
                continue;
            }

            wrequest.u.param.value = 0;
            wrequest.u.param.flags = IW_SCAN_DEFAULT;

            if(iw_set_ext(interface_socket, current_interface->ifa_name, SIOCSIWSCAN, &wrequest) < 0){
                close(interface_socket);
                continue;
            }
            
            bool scan_failed = false;
            const auto init_time = std::chrono::steady_clock::now();
            char buffer[0xFFFF];

            int index = 0;
            while(true) {
                memset(buffer, 0, sizeof(buffer));
                wrequest.u.data.pointer = buffer;
                wrequest.u.data.length = sizeof(buffer);
                wrequest.u.data.flags = 0;

                if(iw_get_ext(interface_socket, current_interface->ifa_name, SIOCGIWSCAN, &wrequest) < 0) {
                    if(errno != EAGAIN){
                        scan_failed = true;
                        break;
                    }
                }
                else break;

                //check for timeout
                if(std::chrono::steady_clock::now() - init_time > std::chrono::seconds(15)){
                    scan_failed = true;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            close(interface_socket);
            if(scan_failed)
                continue;

            //parse scan results
            iw_event wevent;
            stream_descr wstream;
            iw_init_event_stream(&wstream, buffer, wrequest.u.data.length);
            char event_buffer[512];

            while(iw_extract_event_stream(&wstream, &wevent, wrange.we_version_compiled) > 0) {
                bool drop_wap = false;
                switch(wevent.cmd){
                    case SIOCGIWAP:
                        wap_vect.emplace_back();
                        wap_vect.back().bssid.assign(reinterpret_cast<uint8_t*>(wevent.u.ap_addr.sa_data), reinterpret_cast<uint8_t*>(wevent.u.ap_addr.sa_data) + ETH_ALEN);
                    break;

                    case SIOCGIWESSID:
                        wap_vect.back().ssid = std::string(reinterpret_cast<const char*>(wevent.u.essid.pointer), wevent.u.essid.length);
                        if(wap_vect.back().ssid == "")
                            drop_wap = true;
                    break;

                    case IWEVQUAL:
                        if(wevent.u.qual.updated & IW_QUAL_RCPI)
                            wap_vect.back().strength = (wevent.u.qual.level / 2.0) - 110.0;
                        else if(wevent.u.qual.updated & IW_QUAL_DBM){
                            if(wevent.u.qual.level >= 64)
                                wap_vect.back().strength = wevent.u.qual.level - 0x100;
                            else
                                wap_vect.back().strength = wevent.u.qual.level;
                        }
                    break;

                    default:
                    break;                       
                }
                
                if(drop_wap)
                    wap_vect.pop_back();
            }

            freeifaddrs(initial_interface);
            return WGEO_SUCCESS;
        }
        else close(interface_socket);
    }

    freeifaddrs(initial_interface);
    return WGEO_ERR_ACCESS_INTERFACE_FAILED;
}
#else
int get_wap_info(std::vector<wap_info>& wap_vect) noexcept {
    return WGEO_ERR_INVALID_PLATFORM;
}
#endif