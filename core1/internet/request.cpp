#ifndef request_f
#define request_f

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/init.h"
#include "lwip/ip_addr.h"
#include "lwip/tcp.h"
#include "../../config.cpp"

#include <string>
namespace INTERNET {
    static char http_request[512];
    static struct tcp_pcb *client_pcb = NULL;
    void * request_callback = nullptr;

    bool ongoing_connection = false;

    // ----------------------------
    // CALLBACK: when data received
    // ----------------------------
    static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
        if (!p) {
            #if INTERNET_PRINT_DEBUG
                printf("Connection closed by server.\n");
            #endif
            tcp_close(tpcb);
            ongoing_connection = false;
            return ERR_OK;
        }

        char *buf = (char*)malloc(p->tot_len + 1);
        if (!buf) {
            return ERR_MEM;
        };

        pbuf_copy_partial(p, buf, p->tot_len, 0);
        buf[p->tot_len] = '\0';
        #if INTERNET_PRINT_HTML_RESPONSE
            printf("Received %d bytes:\n", p->tot_len);
            printf("%s", (char*)buf);
        #endif
        if (request_callback != nullptr) {
            ((void (*)(const char *))request_callback)(buf);
        }
        free(buf);
        pbuf_free(p);
        
        ongoing_connection = false;
        return ERR_OK;
    }

    // ----------------------------
    // CALLBACK: after connect
    // ----------------------------
    static err_t http_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
        if (err != ERR_OK) {
            printf("Connection failed: %d\n", err);
            ongoing_connection = false;
            return err;
        }

        #if INTERNET_PRINT_DEBUG
            printf("Connected to server.\n");
        #endif
        tcp_recv(tpcb, http_recv);

        err_t e = tcp_write(tpcb, http_request, strlen(http_request), TCP_WRITE_FLAG_COPY);
        if (e != ERR_OK) {
            printf("tcp_write failed: %d\n", e);
            ongoing_connection = false;
            return e;
        }

        tcp_output(tpcb);
        return ERR_OK;
    }
    static void http_err(void *arg, err_t err) {
        printf("TCP error occurred: %d\n", err);
        if (client_pcb) {
            client_pcb = NULL;
        }
        ongoing_connection = false;
    }
    
    static err_t http_poll(void *arg, struct tcp_pcb *tpcb) {
        printf("Poll timeout, aborting.\n");
        if (tpcb->state == ESTABLISHED || tpcb->state == CLOSE_WAIT) {
            tcp_close(tpcb);
            ongoing_connection = false;
            client_pcb = nullptr;
            return ERR_OK;
        } else {
            tcp_abort(tpcb);
            ongoing_connection = false;
            client_pcb = nullptr;
            return ERR_OK;
        }
    }
    // ----------------------------
    // MAIN START
    // ----------------------------
    void http_raw_client(std::string ip, uint32_t port) {
        ip_addr_t dest_ip;
        ipaddr_aton(ip.c_str(), &dest_ip);

        client_pcb = tcp_new();
        if (!client_pcb) {
            printf("Failed to create pcb.\n");
            ongoing_connection = false;
            return;
        }

        err_t err = tcp_connect(client_pcb, &dest_ip, port, http_connected);
        if (err != ERR_OK) {
            printf("Connect failed: %d\n", err);
            tcp_abort(client_pcb);
            ongoing_connection = false;
            return;
        }
        tcp_err(client_pcb, http_err);
        tcp_poll(client_pcb, http_poll, 20);

    }

    void raw_make_http_get_request(std::string ip, uint32_t port, std::string url, std::string get_data = "", void * callback = nullptr) {
        ongoing_connection = true;
        request_callback = callback;

        snprintf(http_request, sizeof(http_request),
            "GET %s%s%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "\r\n",
            url.c_str(), CONFIG::PICO_REQUEST_HEADER.c_str(), get_data.c_str(), ip.c_str());
        http_raw_client(ip, port);
    }

    void simple_send_data(std::string url, std::string get_data = "", void * callback = nullptr) {
        ongoing_connection = true;
        request_callback = callback;

        snprintf(http_request, sizeof(http_request),
            "GET %s%s%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "\r\n",
            url.c_str(), CONFIG::PICO_REQUEST_HEADER.c_str(), get_data.c_str(), CONFIG::SERVER_IP.c_str());
        http_raw_client(CONFIG::SERVER_IP, CONFIG::SERVER_PORT);
    }
}

#endif
