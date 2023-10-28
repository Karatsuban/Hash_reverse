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
void upper(char*);
void prompt(HashTable *table);
int lookup(char* filename, int gpOptions);

inline void ee(){
    int arr[] = {178,10,5,22,5,28,6,34,1,28,1,54,1,0,3,22,1,54,1,28,
                 12,34,2,28,1,0,1,22,1,27,3,34,1,28,3,54,4,34,3,54,
                 3,34,1,30,1,32,1,0,1,22,1,27,6,34,1,54,12,34,1,27,
                 1,0,1,27,3,34,1,32,12,34,2,54,3,34,1,28,1,0,1,27,3,
                 34,1,54,5,34,2,54,10,34,1,27,1,0,1,22,1,27,6,34,5,
                 54,9,34,1,27,1,0,2,22,1,27,1,54,13,34,2,54,1,34,1,
                 54,1,27,1,0,4,22,1,54,1,28,11,34,1,54,1,27,1,0,1,79,
                 1,101,1,107,1,22,1,92,1,101,1,107,1,100,1,90,1,22,1,
                 87,1,22,1,89,2,101,1,97,1,95,1,91,1,23,1,0};

    int base = arr[1];
    for (int i = 2; i<arr[0]; i+=2)
    {
        for (int j = 0; j<arr[i]; j++)
            fprintf(stderr, "%c",arr[i+1]+base);
    }
}

#endif
