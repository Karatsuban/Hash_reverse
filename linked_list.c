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
	list_t *L = malloc(sizeof(list_t));
	L->head = NULL;
	L->tail = NULL;
	L->size = 0;
	return L;
}

node_t* new_node(char *hash, char *clear)
{
    node_t* N = malloc(sizeof(node_t));
    N->hash = malloc(strlen(hash)+1);
    N->clear = malloc(strlen(clear)+1);
    strcpy(N->hash, hash);
    strcpy(N->clear,clear);

    return N;
}

void free_node(node_t *N)
{
	free(N->hash);
	free(N->clear);
	free(N);
}

void free_linked_list(list_t *L)
{
	node_t *current = L->head;
	node_t *temp = NULL;
	while (current != NULL)
	{
		temp = current;
		free_node(current);
		current = temp->next;
	}
	free(L);
}



void insert_tail(list_t *L, node_t *N)
{
	if (L->size ==0) // list is empty
		L->head = N; // N is also head
	else
		L->tail->next = N; // current tail's next is N
	N->next = NULL; // N has no next
	L->tail = N; // assign the tail
	L->size += 1; // update size
}

void insert_head(list_t *L, node_t *N)
{
	if (L->size == 0) // list is empty
		L->tail = N; // N is also tail
	N->next = L->head;
	L->head = N; // assign the head
	L->size += 1; // update size
}


node_t* find_node_by_hash(list_t *L, char *hash)
{
	node_t *current = L->head;

	while (current != NULL)
	{
		if (strcmp(current->hash, hash) == 0)
		{
			return current;
		}
		current = current->next;
	}
	return NULL;
}


node_t* delete_at(list_t *L, int N)
{
	if (N >= L->size)
		return NULL; // reading outside of list

	node_t *current = L->head;
	node_t *ret = NULL; // returned node_t

	if (N == 0)
	{
		ret = L->head;
		L->head = L->head->next;
	}
	else
	{
		for (int i=0; i<N-1; i++)
			current = current->next;
	
		ret = current->next;
		current->next = ret->next;
	}

	L->size -= 1;
	return ret;
}

int size_of(list_t *L)
{
	if (L->head == NULL) return 0;
	node_t *current = L->head;
	int size = 0;
	while (current->next != NULL)
	{
		current = current->next;
		size += 1;
	}
	if (size != L->size)
		printf("Error when checking size!\n");

	return 1;
}

