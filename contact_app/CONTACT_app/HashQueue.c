#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 
#include <time.h> 
#include"HashQueue.h"
#include<Windows.h>


/* Hash queue interface functions */
hash_queue_t* create_hash_queue(size_t bucket_size){
    hash_queue_t* p_hash_queue = NULL; 
    size_t index; 

    if(bucket_size == 0)
        return (NULL); 

    p_hash_queue = (hash_queue_t*)xmalloc(sizeof(hash_queue_t)); 
    p_hash_queue->pp_list_head_array = (list_t**)xmalloc(sizeof(list_t*) * bucket_size); 
    memset(p_hash_queue->pp_list_head_array, 0, sizeof(list_t*) * bucket_size); 
    p_hash_queue->bucket_size = bucket_size; 

    for(index = 0; index < bucket_size; ++index)
        p_hash_queue->pp_list_head_array[index] = create_list(); 

    return (p_hash_queue); 
}   

void add_key(hash_queue_t* p_hash_queue, char* number,char* string){
    index_t index; 
    int key = string[0];

    index = hash(key, p_hash_queue->bucket_size); 
    insert_end(p_hash_queue->pp_list_head_array[index],number,string); 
     MessageBox(
             (HWND)NULL,
             TEXT("Number Has Been Saved!!!!"),
             TEXT("SAVED"),
             MB_TOPMOST | MB_OK
             );
     
}

void remove_key(hash_queue_t* p_hash_queue, char* string){
    index_t index; 
    node_t* r_node = NULL; 
    int key = string[0];

    index = hash(key, p_hash_queue->bucket_size); 
    r_node = search_node(p_hash_queue->pp_list_head_array[index], string); 
    if(r_node != NULL)
    {
        MessageBox(
             (HWND)NULL,
             TEXT("Number Has Been Deleted!!!!"),
             TEXT("DELETE"),
             MB_TOPMOST | MB_OK
             );
    }else
    {
        MessageBox(
             (HWND)NULL,
             TEXT("Not Found!!!!!"),
             TEXT("DELETE"),
             MB_TOPMOST | MB_OK
             );
    }
    generic_delete(r_node); 


}

void search_key(hash_queue_t* p_hash_queue, char* string){
    index_t index; 
    node_t* e_node=NULL;
    TCHAR szstring[] = TEXT("The Number OF %s Is %s.");
    TCHAR szformat[100];
    int key = string[0];

    index = hash(key, p_hash_queue->bucket_size); 
    e_node = find(p_hash_queue->pp_list_head_array[index],string);
    if(e_node != NULL)
    {
         int iLength = wsprintf(szformat,szstring,e_node->name,e_node->number);
         MessageBox(
             (HWND)NULL,
             szformat,
             TEXT("SEARCH"),
             MB_TOPMOST | MB_OK
             );
    }else
    {
         MessageBox(
             (HWND)NULL,
             TEXT("Not Found"),
             TEXT("SEARCH"),
             MB_TOPMOST | MB_OK
             );
    }
          
}

void show_hash_queue(hash_queue_t* p_hash_queue, const char* msg){
    index_t index; 

    if(msg)
        puts(msg); 

    for(index = 0; index < p_hash_queue->bucket_size; ++index){
        printf("LIST(%llu)<->\t", index); 
        show_list(p_hash_queue->pp_list_head_array[index]); 
    }
}

status_t destroy_hash_queue(hash_queue_t** pp_hash_queue){
    hash_queue_t* p_hash_queue = *pp_hash_queue; 
    index_t index; 

    for(index = 0; index < p_hash_queue->bucket_size; ++index)
        destroy_list(&p_hash_queue->pp_list_head_array[index]); 

    free(p_hash_queue->pp_list_head_array); 
    free(p_hash_queue); 

    *pp_hash_queue = NULL; 

    return (SUCCESS); 
}

/* Hash queue helper function */
static index_t hash(number_t starting_char, size_t bucket_size){
    return (starting_char % (bucket_size+71)); 
}

/* Linked list interface functions */
list_t* create_list(void){
    list_t* p_new_list = NULL; 

    p_new_list = get_node("0","Head"); 
    p_new_list->prev = p_new_list; 
    p_new_list->next = p_new_list; 

    return (p_new_list); 
}

status_t insert_end(list_t* p_list,char* number,char* string){
    generic_insert(p_list->prev, get_node(number,string), p_list); 
    return (SUCCESS); 
}

status_t remove_data(list_t* p_list,char* string){
    node_t* p_delete_node = NULL; 

    p_delete_node = search_node(p_list,string); 
    if(p_delete_node == NULL)
        return (INVALID_KEY); 

    generic_delete(p_delete_node); 

    return (SUCCESS); 
}

node_t* find(list_t* p_list,char* string){
    node_t* p_node = NULL; 

    p_node = search_node(p_list,string); 

    return (p_node); 
}


void show_list(list_t* p_list){
    node_t* p_run = NULL; 

    printf("[START]<->"); 
    for(p_run = p_list->next; p_run != p_list; p_run = p_run->next)
        printf("[%s,%s]<->", p_run->name,p_run->number); 
    puts("[END]"); 
}

status_t destroy_list(list_t** pp_list){
    list_t* p_list = *pp_list; 
    node_t* p_run = NULL; 
    node_t* p_run_next = NULL; 

    for(p_run = p_list->next; p_run != p_list; p_run = p_run_next){
        p_run_next = p_run->next; 
        free(p_run); 
    }

    free(p_list); 
    *pp_list = NULL; 

    return (SUCCESS); 
} 

/* Linked list helper functions */
static void generic_insert(node_t* p_beg, node_t* p_mid, node_t* p_end){
    p_mid->next = p_end; 
    p_mid->prev = p_beg; 
    p_beg->next = p_mid; 
    p_end->prev = p_mid; 
}

static void generic_delete(node_t* p_delete_node){
    p_delete_node->prev->next = p_delete_node->next; 
    p_delete_node->next->prev = p_delete_node->prev; 
    free(p_delete_node); 
}

static node_t* search_node(list_t* p_list,char* string){
    node_t* p_run = NULL; 

    for(p_run = p_list->next; p_run != p_list; p_run = p_run->next)
        if(!(strcmp(p_run->name,string)))
            return (p_run); 

    return (NULL); 
}

static node_t* get_node(char* number,char* string){
    node_t* p_new_node = NULL; 

    p_new_node = (node_t*)xmalloc(sizeof(node_t)); 
    p_new_node->number = (char*)xmalloc(strlen(number) + 1);
     strcpy(p_new_node->number,number);
    p_new_node->name =  (char*)xmalloc(strlen(string) + 1);
    strcpy(p_new_node->name,string);
    p_new_node->prev = NULL; 
    p_new_node->next = NULL; 

    return (p_new_node); 
}

static void* xmalloc(size_t size_in_bytes){
    void* ptr = NULL; 

    ptr = malloc(size_in_bytes); 
    if(ptr == NULL){
        puts("Error in allocating memory"); 
        exit(EXIT_FAILURE); 
    }

    return (ptr); 
}
