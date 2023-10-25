#ifndef TABLE
#define TABLE 1

#include "linked_list.h"

typedef struct HashTable
{
	list_t** items;
	int max;
	int count;
} HashTable;


int hash_function(char *str, int max);
HashTable* new_table(int max);
void free_HashTable(HashTable *T);
void add_item(HashTable* table, char* hash, char* clear);
int has_password(HashTable* table, char* hash, char **PTR_clear);
void prompt(HashTable *table);
int lookup(char* filename);

#endif
