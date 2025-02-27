#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 
#include <time.h> 

#define TRUE            1 
#define FALSE           0
#define SUCCESS         1 
#define INVALID_KEY     2 
#define BUCKET_SIZE     26


typedef unsigned long long int number_t; 
typedef unsigned long long int index_t; 

typedef struct hash_queue hash_queue_t; 
typedef struct node node_t; 
typedef node_t list_t; 

typedef int status_t; 

struct node{
    char* number; 
    char* name;
    struct node* prev; 
    struct node* next; 
}; 

struct hash_queue{
    list_t** pp_list_head_array; 
    size_t bucket_size; 
}; 

/* Hash queue interface functions */
hash_queue_t* create_hash_queue(size_t bucket_size); 
void add_key(hash_queue_t* p_hash_queue,char* number,char* string); 
void remove_key(hash_queue_t* p_hash_queue, char* string); 
void search_key(hash_queue_t* p_hash_queue, char* string); 
void show_hash_queue(hash_queue_t* p_hash_queue, const char* msg); 
status_t destroy_hash_queue(hash_queue_t** pp_hash_queue); 

/* Hash queue helper function */
static index_t hash(number_t starting_char, size_t bucket_size);

/* Linked list interface functions */
list_t* create_list(void); 
status_t insert_end(list_t* p_list, char* number,char* string); 
status_t remove_data(list_t* p_list, char* string); 
node_t* find(list_t* p_list,char* string); 
void show_list(list_t* p_list); 

/* Linked list helper functions */
static void generic_insert(node_t* p_beg, node_t* p_mid, node_t* p_end); 
static void generic_delete(node_t* p_delete_node); 
static node_t* search_node(list_t* p_list, char* string); 
static node_t* get_node(char* number,char* string); 
static void* xmalloc(size_t size_in_bytes);