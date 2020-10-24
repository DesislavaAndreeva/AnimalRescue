#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/unistd.h>
#include "ipc-client.h"

void arc_init_client()
{
	memset(&client.server_sockaddr, 0, sizeof(struct sockaddr_un));
	memset(&client.client_sockaddr, 0, sizeof(struct sockaddr_un));
	
	client.client_sockaddr.sun_family = AF_UNIX;   
    strcpy(client.client_sockaddr.sun_path, CLIENT_PATH); 
    client.len = sizeof(client.client_sockaddr);
	
	client.server_sockaddr.sun_family = AF_UNIX;   
    strcpy(client.server_sockaddr.sun_path, SERVER_PATH); 
}

int arc_init_connection()
{
	int rc = 0;
	
	client.client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client.client_sock == -1) {
        printf("SOCKET ERROR = %d\n", errno);
        exit(1);
    }
    
    unlink(CLIENT_PATH);
    rc = bind(client.client_sock, (struct sockaddr *) &client.client_sockaddr, client.len);
    if (rc == -1){
        printf("BIND ERROR: %d\n", errno);
        close(client.client_sock);
        exit(1);
    }
    
    rc = connect(client.client_sock, (struct sockaddr *) &client.server_sockaddr, client.len);
    if(rc == -1){
        printf("CONNECT ERROR = %d\n", errno);
        close(client.client_sock);
        exit(1);
    }
    
    return 1;
}

ARC_CONECTION_STATUS arc_send_message()
{
	int choice = 0;
	int rc = 0;
	
	printf("Enter your choice:");
	scanf("%d", &choice);
	
	rc = send(client.client_sock, (void*)&choice, sizeof(choice), 0);
	 if (rc == -1) {
		printf("SEND ERROR: %d", errno);
		close(client.client_sock);
		return ARC_CONECTION_FAIL;
	}   
	else {
		printf("Sending your choice to server ... \n\n", choice);
		return ARC_CONECTION_SUCCESS;
	}	
	
	return ARC_CONECTION_SUCCESS;
}

ARC_CONECTION_STATUS arc_receive_message()
{
	char buf[1024];
	int rc = 0;
	
	memset(buf, 0, sizeof(buf));
	
	rc = recv(client.client_sock, buf, sizeof(buf), 0);   
	if (rc == -1) {
		printf("RECV ERROR = %d\n", errno);
		close(client.client_sock);
		return ARC_CONECTION_FAIL;
	}   
	else {
		printf("%s", buf);
	}
	
	if( (strcmp(buf, "There is no such choice from the menu!")) == 0 )
		return ARC_CONECTION_RESET;
	else if ( (strcmp(buf, "End connection")) == 0 )
			return ARC_CONECTION_END;
	else
		return ARC_CONECTION_CONTINUE;
				
}

ARC_CONECTION_STATUS arc_handle_communication()
{
	char buf[1024];
	int i = 0;
	int rc = 0;

	memset(buf, 0, sizeof(buf));
	
	rc = recv(client.client_sock, buf, sizeof(buf), 0);   
	if (rc == -1) {
		printf("RECV ERROR = %d\n", errno);
		close(client.client_sock);
		return ARC_CONECTION_FAIL;
	}   
	else {
		printf("%s\n", buf);
	}   
	
	
    while(1)
    {
		arc_send_message();
		i = arc_receive_message();
		
		if( i == ARC_CONECTION_CONTINUE )
		{	
			arc_send_message();
			i = arc_receive_message();
		}else if ( i == ARC_CONECTION_RESET )
			continue;
		else
			break;
	}
		  
    close(client.client_sock);
}

int main()
{
	arc_init_client();
	arc_init_connection();
	arc_handle_communication();
}
