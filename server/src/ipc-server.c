#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include "ipc-server.h"
#include "linked-list.h"

const char* print_months[] = {
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",
};

void ars_init_server()
{
	memset(&server.server_sockaddr, 0, sizeof(struct sockaddr_un));
	
	server.server_sockaddr.sun_family = AF_UNIX;   
    strcpy(server.server_sockaddr.sun_path, SOCK_PATH); 
    server.len = sizeof(server.server_sockaddr);
}

void ars_init_connection()
{
	server.queue_size = QUEUE_SIZE;
	
	server.server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server.server_sock == -1){
        printf("SOCKET ERROR: %d\n", errno);
        exit(1);
    }
	
	unlink(SOCK_PATH);
    
    server.listenfd = bind(server.server_sock, (struct sockaddr *) &server.server_sockaddr, server.len);
    if (server.listenfd == -1){
        printf("BIND ERROR: %d\n", errno);
        close(server.server_sock);
        exit(1);
    }
    
    server.listenfd = listen(server.server_sock, server.queue_size);
    if (server.listenfd == -1){ 
        printf("LISTEN ERROR: %d\n", errno);
        close(server.server_sock);
        exit(1);
    }
	
}

void ars_accept_new_connections()
{
	int client_len = 0;
	int client_sock = 0;
	int rc = 0;
	struct sockaddr_un client_sockaddr;
	memset(&server.server_sockaddr, 0, sizeof(struct sockaddr_un));
	
	printf("Server waiting for new connection ... \n");
	
	while ( ( client_sock = accept(server.server_sock, (struct sockaddr *) &client_sockaddr, &server.len)) ) 
	{
		printf("Server connected to new client ... \n");
		if (client_sock == -1){
			printf("ACCEPT ERROR: %d\n", errno);
			close(server.server_sock);
			close(client_sock);
			exit(1);
		}
		
		if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
		{
			perror("could not create thread");
			return;
		}
	}
}

void ars_end_server()
{
	close(server.server_sock);
}

ARS_CONECTION_STATUS ars_send_menu(int sock)
{
	char output[ 1000 ] = {0};
	int rc = 0;
	
	sprintf ( output + strlen ( output ), "%s==================================================================%s", LOG_NEWLINE, LOG_NEWLINE );
	sprintf ( output + strlen ( output ), "===================Animal rescue center menu======================%s", LOG_NEWLINE );
	sprintf ( output + strlen ( output ), "==================================================================%s", LOG_NEWLINE );
	sprintf ( output + strlen ( output ), "1. Enter '1' to get all animals that arrived in chosen month !%s", LOG_NEWLINE );
	sprintf ( output + strlen ( output ), "2. Enter '2' to get the adopted animals in chosen month !%s", LOG_NEWLINE );
	sprintf ( output + strlen ( output ), "3. Enter '3' to get all castrated animals in chosen month !%s", LOG_NEWLINE );
	sprintf ( output + strlen ( output ), "4. Enter '4' if you want to exit !%s", LOG_NEWLINE );
	sprintf ( output + strlen ( output ), "==================================================================%s", LOG_NEWLINE );
	
	rc = send(sock, output, strlen(output), 0);
    if (rc == -1) {
        printf("SEND ERROR: %d", errno);
        close(sock);
        return ARS_CONNECTION_FAIL;
    }   
    else {
        printf("Menu sent!\n");
        return ARS_CONNECTION_SUCCESS;
    }
    
    return ARS_CONNECTION_SUCCESS;
}

ARS_CONECTION_STATUS ars_ask_for_month(int sock)
{
	char output[ 1024 ] = {0};
	int rc = 0;
	int month = 0;
		
	bzero(output, 1024);
	sprintf ( output , "Enter the month: %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 1 - January %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 2 - February %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 3 - March %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 4 - April %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 5 - May %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 6 - June %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 7 - July %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 8 - August %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 9 - September %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 10 - October %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 11 - November %s", LOG_NEWLINE );
	sprintf ( output + strlen(output), " 12 - December %s", LOG_NEWLINE );
	
	rc = send(sock, output, strlen(output), 0);
    if (rc == -1) {
        printf("SEND ERROR: %d", errno);
        close(sock);
        return ARS_CONNECTION_FAIL;
    }   
    else {
        printf("Month data sent!", output);
        return ARS_CONNECTION_SUCCESS;
    }
    
	return ARS_CONNECTION_SUCCESS;
}

ARS_CONECTION_STATUS ars_send_end(int sock)
{
	char output[ 30 ] = {0};
	int month = 0;
	int rc = 0;
	
	bzero(output, 10);
	sprintf ( output , "End connection");
	
	rc = send(sock, output, strlen(output), 0);
    if (rc == -1) {
        printf("SEND ERROR: %d", errno);
        close(sock);
        return ARS_CONNECTION_FAIL;
    }   
    else {
        printf("End message sent!\n");
        return ARS_CONNECTION_SUCCESS;
    }
    
	return ARS_CONNECTION_SUCCESS;
}


ARS_CONECTION_STATUS ars_send_result(int sock, char* output)
{
	int rc = 0;
	
	rc = send(sock, output, strlen(output), 0);
    if (rc == -1) {
        printf("SEND ERROR: %d", errno);
        close(sock);
        return ARS_CONNECTION_FAIL;
    }   
    else {
        printf("Results from request sent!\n", output);
        return ARS_CONNECTION_SUCCESS;
    }    
    
    return ARS_CONNECTION_SUCCESS;
}

void ars_arrived_animals(int sock)
{
	char* buffer = NULL;
	int count = 0;
	int month = 0;
	int rc = 0;
	int i = 0;
	int ret = 0;
	
	ars_ask_for_month(sock);
    
    rc = recv(sock, (void*)&month, sizeof(month), 0);   
    if (rc == -1) {
        printf("RECV ERROR = %d\n", errno);
        close(sock);
        return;
    }   
    else {
		printf("Send statistics for %s\n", print_months[month - 1 ]);
		if( month == 0)
		{
			close(sock);
			ret = ARS_CONECTION_END;
			pthread_exit(&ret);
		}
    }
    
	if(month > 0 && month <= 12)
	{
		count = arl_get_all_count_arrived_animals(month);
		
		if( count == 0){
			ars_send_result(sock, "There is no data for this month!");
			return;
		}
		
		for(i = 0; i < count; i ++){
			buffer = arl_get_all_arrived_animals(month, i + 1);
			
			
			if(buffer){
				ars_send_result(sock, buffer);
				free(buffer);
			}
				
		}
		
	}else{
		ars_send_result(sock, "There is no such month!");
	}
}

void ars_adopted_animals(int sock)
{
	char* buffer = NULL;
	int count = 0;
	int month = 0;
	int rc = 0;
	int i = 0;
	int ret = 0;
	
	ars_ask_for_month(sock);
	
    rc = recv(sock, (void*)&month, sizeof(month), 0);   
    if (rc == -1) {
        printf("RECV ERROR = %d\n", errno);
        close(sock);
        return;
    }   
    else {
		if( rc == 0)
		{
			close(sock);
			ret = ARS_CONECTION_END;
			pthread_exit(&ret);
		}
		printf("Send statistics for user choice: %s\n", print_months[month - 1 ]);
		
    }
    
	
	if(month > 0 && month <= 12){
		count = arl_get_all_count_adopted_animals(month);
		
		if( count == 0){	
			ars_send_result(sock, "There is no data for this month!");
			return;
		}
		
		for(i = 0; i < count; i ++){
			buffer = arl_get_all_adopted_animals(month, i + 1);
			
			if(buffer){
				ars_send_result(sock, buffer);
				free(buffer);
			}else{
				printf("No buffer");
			}
		}
		
	}else{
		ars_send_result(sock, "There is no such month!");
	}
}

void ars_castrated_animals(int sock)
{
	char* buffer = NULL;
	int count = 0;
	int month = 0;
	int rc = 0;
	int i = 0;
	int ret = 0;
	
	ars_ask_for_month(sock);
	
    rc = recv(sock, (void*)&month, sizeof(month), 0);   
    if (rc == -1) {
        printf("RECV ERROR = %d\n", errno);
        close(sock);
        return;
    }   
    else {
		if( month == 0)
		{
			close(sock);
			ret = ARS_CONECTION_END;
			pthread_exit(&ret);
		}
		printf("Send statistics for user choice: %s\n", print_months[month - 1 ]);
    }
    	
	if(month > 0 && month <= 12){
		count = arl_get_count_all_castrated_animals(month);
		
		if( count == 0){
			ars_send_result(sock, "There is no data for this month!");
			return;
		}
		
		for(i = 0; i < count; i ++){
			buffer = arl_get_all_castrated_animals(month, i + 1);
			
			if(buffer){
				ars_send_result(sock, buffer);
				free(buffer);
			}
		}
		
	}else{
		ars_send_result(sock, "There is no such month!");
	}
}

ARS_CONECTION_STATUS ars_process_received_request(int sock, int choice)
{
	switch(choice)
	{
		case 1:
			ars_arrived_animals(sock);
			return ARS_CONECTION_CONTINUE;
			break;
		case 2:
			ars_adopted_animals(sock);
			return ARS_CONECTION_CONTINUE;
			break;
		case 3:
		    ars_castrated_animals(sock);
			return ARS_CONECTION_CONTINUE;
			break;
		case 4:
			return ARS_CONECTION_END;
			break;
		default:
			ars_send_result(sock, "There is no such choice from the menu!");
			break;
	}
}

void *connection_handler(void *client_socket)
{
	int sock = *(int*)client_socket;
    int read_size = 0;
    char buf[1024];
    int choice = 0;
    int ret = 0;
    int rc = 0;
    int i = 0;
    
    
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		ars_send_menu(sock); 
		
		printf("Wait for client to select from the menu ... \n");
		rc = recv(sock, (void*)&choice, sizeof(choice), 0);   
		if (rc == -1) {
			printf("RECV ERROR = %d\n", errno);
			close(sock);
			ret = ARS_CONECTION_END;
			pthread_exit(&ret);
		}   
		else {
			if(rc == 0){
				close(sock);
				ret = ARS_CONECTION_END;
				pthread_exit(&ret);
			}
			
			printf("Client selected %d  from the menu ... \n", choice);		
		}

		i = ars_process_received_request(sock, choice);
		
		if(i == ARS_CONECTION_END){			
			printf("End connection with the client!\n");
			ars_send_end(sock);	
			close(sock);
			ret = ARS_CONECTION_END;
			pthread_exit(&ret);
		}
	}
}
