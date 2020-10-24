#ifndef _ARC_IPC_CLIENT_H
#define _ARC_IPC_CLIENT_H

#include <sys/un.h>

#define SERVER_PATH "/tmp/anr_unix_sock.server"
#define CLIENT_PATH "/tmp/anr_unix_sock.client"

typedef struct {
	int client_sock;
	int len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
}AnimalRescue_Client_Data;

AnimalRescue_Client_Data client;

typedef int ARC_CONECTION_STATUS;	
#define ARC_CONECTION_CONTINUE	10
#define ARC_CONECTION_SUCCESS	11
#define ARC_CONECTION_RESET 12
#define ARC_CONECTION_FAIL	13
#define ARC_CONECTION_END	14

void arc_init_client();
int arc_init_connection();
ARC_CONECTION_STATUS arc_receive_message();
ARC_CONECTION_STATUS arc_handle_communication();
ARC_CONECTION_STATUS arc_send_message();

#endif

