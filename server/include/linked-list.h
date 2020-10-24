#ifndef _ARL_LINKED_LIST_H
#define _ARL_LINKED_LIST_H

typedef struct Animal_Data{
	char *animal_name;
	char *animal_type;
	char *adopter;
    time_t arrival;
    time_t departure;
    time_t castrated;
}AnimalRescue_Data;

typedef struct node {
    AnimalRescue_Data data;
    struct node* next;
}AnimalRescue_List_Node;

typedef int ARL_LL_STATUS;	
#define ARL_LL_SUCCESS	0
#define ARL_LL_FAIL	  	1
#define ARL_LL_MEMORY_FAIL	  2
#define ARL_LL_DELETE_FAIL	  3
#define ARL_LL_INVALID_INDEX  4
#define ARL_LL_EMPTY_NODE  5

#define BUFFER_SIZE 1024

AnimalRescue_List_Node *head;

ARL_LL_STATUS arl_init_list(AnimalRescue_List_Node** head);
ARL_LL_STATUS arl_print_list(AnimalRescue_List_Node* head);
ARL_LL_STATUS arl_remove_node(AnimalRescue_List_Node* head);
ARL_LL_STATUS arl_pop_first_node(AnimalRescue_List_Node** head);
ARL_LL_STATUS arl_remove_last_node(AnimalRescue_List_Node** head);
ARL_LL_STATUS arl_remove_node_by_index(AnimalRescue_List_Node** head, int n);
ARL_LL_STATUS arl_remove_node_by_name(AnimalRescue_List_Node ** head, char* name);
ARL_LL_STATUS arl_push_end(AnimalRescue_List_Node* head, AnimalRescue_Data data);

AnimalRescue_List_Node *arl_free_list(AnimalRescue_List_Node *head);
AnimalRescue_List_Node* arl_add_node(AnimalRescue_List_Node* node, AnimalRescue_Data data);


int arl_read_statup_data();
void arl_save_data_to_struct(char* str);
int arl_get_all_count_adopted_animals(int month);
int arl_get_all_count_arrived_animals(int month); 
int arl_get_count_all_castrated_animals(int month);
char* arl_get_all_arrived_animals(int month, int count);
char* arl_get_all_adopted_animals(int month, int count);
char* arl_get_all_castrated_animals(int month, int count);

#endif
