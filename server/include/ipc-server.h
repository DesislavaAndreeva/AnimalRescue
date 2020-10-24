#ifndef _ARS_IPC_SERVER_H
#define _ARS_IPC_SERVER_H

#include <sys/un.h>

#define SOCK_PATH  "/tmp/anr_unix_sock.server"
#define QUEUE_SIZE 100
#define LOG_NEWLINE "\n"

typedef struct {
	int listenfd;
	int server_sock;
	int queue_size;
	int len;
    struct sockaddr_un server_sockaddr;
}AnimalRescue_Server_Data;

AnimalRescue_Server_Data server;
pthread_t thread_id;

typedef int ARS_CONECTION_STATUS;	
#define ARS_CONECTION_CONTINUE	10
#define ARS_CONNECTION_SUCCESS	11
#define ARS_CONECTION_END	12
#define ARS_CONNECTION_FAIL	13

void ars_init_server();
void ars_end_server();
void ars_init_connection();
void ars_accept_new_connections();
void *connection_handler(void *);

ARS_CONECTION_STATUS ars_send_menu(int sock);
ARS_CONECTION_STATUS ars_send_end(int sock);
ARS_CONECTION_STATUS ars_ask_for_month(int sock);
ARS_CONECTION_STATUS ars_send_result(int sock, char* ouput);
ARS_CONECTION_STATUS ars_process_received_request(int sock, int choice);

void ars_arrived_animals(int sock);
void ars_adopted_animals(int sock);
void ars_castrated_animals(int sock);

#endif
