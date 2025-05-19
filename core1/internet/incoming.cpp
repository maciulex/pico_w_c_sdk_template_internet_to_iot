#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include <string>

#include "incoming/get_param_processing.cpp"

namespace INTERNET {

    static struct tcp_pcb *server_pcb;
    
    static err_t on_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
        printf("Response sent successfully.\n");
        tcp_close(tpcb);
        return ERR_OK;
    }

    static err_t server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
        if (!p) {
            tcp_close(tpcb);
            return ERR_OK;
        }

        char buffer[1024] = {0};
        pbuf_copy_partial(p, buffer, p->tot_len, 0);
        buffer[p->tot_len] = '\0';
        printf("Received request: %s\n", buffer);

        //finding and processing GET PARAMS
        bool header_started= false;
        bool on_value      = false;
        std::string head_name = "", head_value = "";
        std::string return_value = "";

        char * ptr;
        ptr = strstr(buffer, "GET /");
        if (ptr != NULL) {
            ptr+=5;
            while (*ptr != '\0') {

                if (*ptr == '?' || *ptr == '&' || *ptr == ' ') {
                    header_started = true;
                    if (head_name.length() > 0) 
                        return_value += process_get_request(head_name, head_value);
                    head_name  ="";
                    head_value ="";
                    if (*ptr == ' ') break;
                    ptr++;
                    continue;
                }
                if (*ptr == '=') {
                    header_started = false;
                    on_value       = true;
                    ptr++;
                    continue;
                }
                if (header_started) {
                    head_name  += *ptr;
                    ptr++;
                    continue;
                }
                if (on_value) {
                    head_value += *ptr;
                    ptr++;
                    continue;
                }
                ptr++;
            }
        }

        static char http_response[512];
        snprintf(http_response, sizeof(http_response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %i\r\n"
            "Connection: close\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "\r\n"
            "%s\n",
            return_value.c_str(), return_value.length());
        tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);
        tcp_output(tpcb);
        tcp_sent(tpcb, on_sent);
        pbuf_free(p);

        return ERR_OK;
    }

    static err_t server_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
        printf("New connection\n");
        tcp_recv(newpcb, server_recv);
        return ERR_OK;
    }

    void start_server(uint16_t port = 80) {
        server_pcb = tcp_new();
        if (!server_pcb) {
            printf("Failed to create server pcb.\n");
            return;
        }

        err_t err = tcp_bind(server_pcb, IP_ADDR_ANY, port);
        if (err != ERR_OK) {
            printf("Bind failed: %d\n", err);
            return;
        }

        server_pcb = tcp_listen_with_backlog(server_pcb, 1);
        tcp_accept(server_pcb, server_accept);
        printf("Server listening on port %d\n", port);
    }
}