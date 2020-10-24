#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "linked-list.h"
#include "ipc-server.h"

int main() 
{
    arl_init_list(&head);
    ars_init_server();
    arl_read_statup_data();
    //arl_print_list(head);
    ars_init_connection();
	ars_accept_new_connections();
	ars_end_server();
    head = arl_free_list(head);
    
    return 0;
}



