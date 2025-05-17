#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "secret.cpp"


namespace INTERNET {
    bool INTERNET_CONNECTION = false;


    bool create_connection(const char * ssid, const char * pass) {
        // Enable wifi station
        printf(ssid);
        printf(pass);

        printf("Network: Connecting to Wi-Fi...\n");
        uint8_t code = cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 30000);
        printf("Network: connection code %d", code);
        if (code != 0) {
            printf("Network: failed to connect.\n");
            return false;
        } else {
            printf("Network: Connected.\n");
            // Read the ip address in a human readable way
            uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
            printf("Network: IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
            return true;
        }
    }

    bool connect_to_network() {
        cyw43_arch_enable_sta_mode();
        for (int i = 0; i < possible_networks; i++) {
            INTERNET_CONNECTION = create_connection(network_ssid[i], network_pass[i]);
            if (INTERNET_CONNECTION) {
                return INTERNET_CONNECTION;
            }
        }
        return false;
    }
}