#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


list_t* init_linked_list()
{
	// init a linked list object and its elements and returns it
	list_t *L = malloc(sizeof(list_t));
	L->head = NULL;
	L->tail = NULL;
	L->size = 0;
	return L;
}

node_t* new_node(char *hash, char *clear)
{
	// creates a new node and its elements and returns it
    node_t* N = malloc(sizeof(node_t));
	// allocating space to contain hash and clear
    N->hash = malloc(strlen(hash)+1);
    N->clear = malloc(strlen(clear)+1);
	// copy the hash and clear into the node's elements
    strcpy(N->hash, hash);
    strcpy(N->clear,clear);

    return N;
}

void free_node(node_t *N)
{
	// free the content of a node and the node itself
	free(N->hash);
	free(N->clear);
	free(N);
}

void free_linked_list(list_t *L)
{
	// free the contents of a linked list and the linked list itself
	node_t *current = L->head;
	node_t *temp = NULL;
	while (current != NULL)
	{
		// free each node
		temp = current->next;
		free_node(current);
		current = temp;
	}
	free(L); // free the list itself
}



void insert_tail(list_t *L, node_t *N)
{
	// insert a node as the tail of a linked list
	if (L->size ==0) // list is empty
		L->head = N; // N is also head
	else
		L->tail->next = N; // current tail's next is N
	N->next = NULL; // N has no next
	L->tail = N; // assign the tail
	L->size += 1; // update size
}


node_t* find_node_by_hash(list_t *L, char *hash)
{
	// returns the first node containing the given hash in the linked lis
	// else returns NULL
	node_t *current = L->head;

	while (current != NULL)
	{
		if (strcmp(current->hash, hash) == 0)
		{
			// if there is a match
			return current; // return the node
		}
		current = current->next;
	}
	return NULL; // no matching node was found
}


