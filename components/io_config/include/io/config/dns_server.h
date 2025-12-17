#ifndef DNS_SERVER_H
#define DNS_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dns_server_handle *dns_server_handle_t;

dns_server_handle_t start_capture_portal_dns_server();
void stop_capture_portal_dns_server(dns_server_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif