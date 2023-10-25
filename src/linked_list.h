#ifndef LINKED_LIST
#define LINKED_LIST 1


typedef struct Node
{
    char* hash;
    char* clear;
    struct Node* next;
} node_t;

typedef struct LinkedList
{
    node_t *head;
    node_t *tail;
    int size;
} list_t;


list_t* init_linked_list();
node_t* new_node(char*, char*);
void free_linked_list(list_t *L);
void free_node(node_t *N);
void insert_tail(list_t *L, node_t *N);
node_t* find_node_by_hash(list_t*, char*);

#endif
