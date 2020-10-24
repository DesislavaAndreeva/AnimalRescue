#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <linked-list.h>

ARL_LL_STATUS arl_init_list(AnimalRescue_List_Node** head) 
{
   *head = NULL;
   return ARL_LL_SUCCESS;
}

ARL_LL_STATUS arl_print_list(AnimalRescue_List_Node* head) 
{
    AnimalRescue_List_Node * temp = NULL;
    
    for (temp = head; temp; temp = temp->next){
        printf("%s %s %s %s %s %s", temp->data.animal_name, temp->data.animal_type, temp->data.adopter, ctime(&temp->data.arrival), ctime(&temp->data.castrated), ctime(&temp->data.departure));
	}
	printf("\n");
	return ARL_LL_SUCCESS;
}

char* arl_get_all_arrived_animals(int month, int count) 
{
    AnimalRescue_List_Node * temp = NULL;
	struct tm *tmp = NULL;
	char *buffer = NULL;
    int cnt = 0;
    
	buffer = malloc(512);
	bzero(buffer, 512);
	
    for (temp = head; temp; temp = temp->next)
    {
		tmp = gmtime(&temp->data.arrival);
		
		if( tmp->tm_mon + 1 == month )
        {
			cnt++;
			if(count == cnt)
			{
				sprintf ( buffer, "Animal name: %s ", temp->data.animal_name);
				sprintf ( buffer + strlen(buffer) , "Animal type: %s ", temp->data.animal_type);
				sprintf ( buffer + strlen(buffer) , "Arrival date: %s", ctime(&temp->data.arrival));
				return buffer;
			}
		}
	}
	
	return NULL;
}

char* arl_get_all_adopted_animals(int month, int count) 
{
    AnimalRescue_List_Node * temp = NULL;
	char *buffer = NULL;
    struct tm *tmp = NULL;
	int cnt = 0;
	
	buffer = malloc(1024);
	bzero(buffer, 1024);
	
    for (temp = head; temp; temp = temp->next)
    {
		tmp = gmtime(&temp->data.departure);
		
		if( tmp->tm_mon + 1 == month )
		{
			cnt++;
			if(count == cnt)
			{
				sprintf ( buffer, "Animal name: %s ", temp->data.animal_name);
				sprintf ( buffer + strlen(buffer) , "Animal type: %s ", temp->data.animal_type);
				sprintf ( buffer + strlen(buffer) , "Adopter: %s ", temp->data.adopter);
				sprintf ( buffer + strlen(buffer) , "Adoption date: %s ", ctime(&temp->data.departure));
				return buffer;
			}
		}
	}
	
	return NULL;
}

char* arl_get_all_castrated_animals(int month, int count) 
{
    AnimalRescue_List_Node * temp = NULL;
    char *buffer = NULL;
    struct tm *tmp = NULL;
    int cnt = 0;
    
	buffer = malloc(1024);
	bzero(buffer, 1024);
	
    for (temp = head; temp; temp = temp->next)
    {
		tmp = gmtime(&temp->data.castrated);
		
		if( tmp->tm_mon + 1 == month )
        {
			cnt++;
			if(count == cnt)
			{
				sprintf ( buffer, "Animal name: %s ", temp->data.animal_name);
				sprintf ( buffer + strlen(buffer) , "Animal type: %s ", temp->data.animal_type);
				sprintf ( buffer + strlen(buffer) , "Castration date: %s", ctime(&temp->data.castrated));
				return buffer;
			}	
		}
	}
	
	return NULL;
}

int arl_get_all_count_arrived_animals(int month) 
{
    AnimalRescue_List_Node * temp = NULL;
    struct tm *tmp = NULL;
	int count = 0;
	
    for (temp = head; temp; temp = temp->next)
    {
		tmp = gmtime(&temp->data.arrival);
		
		if( tmp->tm_mon + 1 == month )
			count++;
	}
	
	return count;
}

int arl_get_all_count_adopted_animals(int month) 
{
    AnimalRescue_List_Node * temp = NULL;
    struct tm *tmp = NULL;
    int count = 0;
    
    for (temp = head; temp; temp = temp->next)
    {
		tmp = gmtime(&temp->data.departure);
		
		if( tmp->tm_mon + 1 == month )
			count++;
	}
	
	return count;
}

int arl_get_count_all_castrated_animals(int month) 
{
    AnimalRescue_List_Node * temp = NULL;
    struct tm *tmp = NULL;
    int count = 0 ;
    
    for (temp = head; temp; temp = temp->next)
    {
		tmp = gmtime(&temp->data.castrated);
		
		if( tmp->tm_mon + 1 == month )
			count++;
	}
	
	return count;
}


AnimalRescue_List_Node* arl_add_node(AnimalRescue_List_Node* node, AnimalRescue_Data data) 
{
    AnimalRescue_List_Node* temp = (AnimalRescue_List_Node*) malloc(sizeof (AnimalRescue_List_Node));
    if (temp == NULL) {
        return NULL; 
    }
	temp->data = data;
    temp->next = node;
    node = temp;
    return node;
}

ARL_LL_STATUS arl_push_end(AnimalRescue_List_Node* head, AnimalRescue_Data data) 
{
    AnimalRescue_List_Node * current = head;
    
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (AnimalRescue_List_Node*) malloc(sizeof (AnimalRescue_List_Node));
    current->next->data = data;
    current->next->next = NULL;
    
    return ARL_LL_SUCCESS;
}


ARL_LL_STATUS arl_remove_node(AnimalRescue_List_Node* head) 
{
    AnimalRescue_List_Node* temp = (AnimalRescue_List_Node*) malloc(sizeof (AnimalRescue_List_Node));
    if (temp == NULL) {
		return ARL_LL_MEMORY_FAIL;
    }
    
    temp = head->next;
    head->next = head->next->next;
    free(temp);
    return ARL_LL_SUCCESS;
}

ARL_LL_STATUS arl_pop_first_node(AnimalRescue_List_Node** head) {
    AnimalRescue_List_Node * next_node = NULL;

    if (*head == NULL) {
        return ARL_LL_EMPTY_NODE;
    }
    next_node = (*head)->next;
    free(*head);
    *head = next_node;
    
    return ARL_LL_SUCCESS;
}

ARL_LL_STATUS arl_remove_last_node(AnimalRescue_List_Node** head) 
{
    if ((*head)->next == NULL) {
        free(*head);
        return ARL_LL_SUCCESS;
    }

    AnimalRescue_List_Node* current = *head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    free(current->next);
    current->next = NULL;
    
    return ARL_LL_SUCCESS;
}

ARL_LL_STATUS arl_remove_node_by_index(AnimalRescue_List_Node** head, int n) 
{
    AnimalRescue_List_Node *current = *head;
    AnimalRescue_List_Node *temp_node = NULL;
	int i = 0;
	
    if (n == 0) {
        return ARL_LL_INVALID_INDEX;
    }

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return ARL_LL_INVALID_INDEX;
        }
        current = current->next;
    }

    temp_node = current->next;
    current->next = temp_node->next;
    free(temp_node);
    
	return ARL_LL_SUCCESS;
}

ARL_LL_STATUS arl_remove_node_by_name(AnimalRescue_List_Node ** head, char* name) 
{
    AnimalRescue_List_Node * current = *head;
    AnimalRescue_List_Node * temp_node = NULL;
    AnimalRescue_List_Node * prev_node = NULL;
	int i = 0;
	
    while(current != NULL) {
        if(strcmp(current->data.animal_name, name) == 0)
        {
			if(prev_node == NULL){
				if(ARL_LL_SUCCESS == arl_pop_first_node(head))
					return ARL_LL_SUCCESS;
				else
					return ARL_LL_DELETE_FAIL;
			}
			
			if(prev_node->next == NULL){
				if(ARL_LL_SUCCESS == arl_remove_last_node(head))
					return ARL_LL_SUCCESS;
				else
					return ARL_LL_DELETE_FAIL;
			}
			
			temp_node = current;
			prev_node->next = temp_node->next;
			free(temp_node);
			return ARL_LL_SUCCESS;
	    }
	    prev_node = current;
	    
	    if(current->next != NULL)
			current = current->next;
        else
			return ARL_LL_SUCCESS;
    }
    
    return ARL_LL_SUCCESS;
}

AnimalRescue_List_Node *arl_free_list(AnimalRescue_List_Node *head) 
{
    AnimalRescue_List_Node *tmpPtr = head;
    AnimalRescue_List_Node *followPtr = NULL;
    
    while (tmpPtr != NULL) {
        followPtr = tmpPtr;
        tmpPtr = tmpPtr->next;
        free(followPtr);
    }
    return NULL;
}

void arl_save_data_to_struct(char* str)
{
    AnimalRescue_Data res = {0};
    char *token = strtok(str, "#"); 
    int flag = 0;
   
    while( token != NULL )
    {
        if (0 == flag)
        {
			res.animal_name = malloc(strlen(token) + 5);
			memset(res.animal_name, 0, sizeof(res.animal_name));
            strcpy(res.animal_name, token);
        }
        else if (1 == flag)
        {
			res.animal_type = malloc(strlen(token + 5));
			memset(res.animal_type, 0, sizeof(res.animal_type));
            strcpy(res.animal_type, token);
        }
        else if (2 == flag)
        {
			res.adopter = malloc(strlen(token + 5));
			memset(res.adopter, 0, sizeof(res.adopter));
            strcpy(res.adopter, token);
        }
        else if (3 == flag)
            res.arrival = atoi(token);
        else if (4 == flag)
            res.departure = atoi(token);
	else
            res.castrated = atoi(token);

        flag++;
        token = strtok( NULL, "#" );
    }
    
    //printf("%s,%s,%s,%s,%s\n", res.animal_name, res.animal_type, ctime(&res.arrival) , ctime(&res.departure) , ctime(&res.castrated));
	head = arl_add_node(head, res);
}

int arl_read_statup_data()
{   
	char buffer[BUFFER_SIZE];
    int flag = 0;
    FILE* file = fopen("animalrescue.txt", "r");
    if (!file)
        return -1;
 
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        arl_save_data_to_struct(buffer);
        flag++;
    }
    
    fclose(file);
    
    return 0;
}
