#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

#ifndef BOOLEAN
#define BOOLEAN
#define TRUE 1
#define FALSE 0
#endif

typedef struct HashTable
{
	list_t** items;
	int *item_count;
	int max;
	int count;
} HashTable;


int hash_function(char *str, int max)
{
	int ret = 0;
	for (int i=0; i<strlen(str); i++)
	{
		ret += i*i*str[i];
	}
	
	return ret%max;
}

HashTable* new_table(int max)
{
	HashTable* table = malloc(sizeof(HashTable));
	table->count = 0;
	table->max = max;
	table->items = malloc(max*sizeof(list_t*));
	for (int i=0; i<max; i++)
	{
		table->items[i] = NULL;
	}
	return table;
}


void free_HashTable(HashTable *T)
{
	for (int i=0; i<T->max; i++)
	{
		if (T->items[i] != NULL)
			free_linked_list(T->items[i]);
	}
	free(T->items);
	free(T);
}


void add_item(HashTable* table, char* hash, char* clear)
{
	node_t *node = new_node(hash, clear);

	int index = hash_function(node->hash, table->max);

	if (index < 0)
	{
		printf("Dropping malformed string %s (index %i)\n", hash, index);
		return;
	}

	
	table->count += 1;

	if (table->items[index] == NULL)
	{
		table->items[index] = init_linked_list();
		insert_tail(table->items[index], node);
	}
	else
	{
		insert_tail(table->items[index], node);
	}
}

int has_password(HashTable* table, char* hash, char **PTR_clear)
{
	int index = hash_function(hash, table->max);

	int ret = 0;

	if (table->items[index] == NULL)
	{
		*PTR_clear = NULL;
		ret = FALSE;
	}
	else
	{
		node_t *temp = find_node_by_hash(table->items[index], hash);
		if (temp != NULL)
		{
			*PTR_clear = temp->clear;
			ret = TRUE;
		}
		else
		{
			*PTR_clear = NULL;
			ret = FALSE;
		}
	}
	return ret;
}


void prompt(HashTable *table, int hashSize)
{
    int isOver = FALSE;
    char* hash = malloc(hashSize+1);
    char *PTR_clear = NULL;
    int ret_val;


    while (!isOver)
    {
        if (scanf("%s", hash) == EOF)
        {
            isOver = TRUE;
        }
        else
        {
			ret_val = has_password(table, hash, &PTR_clear);

			if (ret_val == TRUE)
                printf("MATCH %s %s\n", hash, PTR_clear);
        }
    }
	free(hash);
}



int lookup(char* filename)
{

	FILE *input_file;
	char *hash = NULL;
	char clear[2048];

	char* hashAlgoName = malloc(100); // will contain the name of the hash algorithm used
	int hashLen = 0; // will contain the lenght of a hash produced with the chosen algo (in bytes)

	input_file = fopen(filename, "r");
	if (input_file == NULL)
	{
		fprintf(stderr, "The file '%s' does not exist!\n", filename);
		return -1;
	}

	fseek(input_file, 0, SEEK_END); // seek to end of file
	int size = ftell(input_file); 
	fseek(input_file, 0, SEEK_SET); // seek back to beginning of file
	

	fscanf(input_file, "%s %i\n", hashAlgoName, &hashLen);
	hash = malloc(hashLen+1); // set a buffer of the correct size


	int ht_size = size/(36*500); // totally arbitrary way to get the size
	// well, 36 is the average of value per line, so size/36 is roughly the number of lines

	if (ht_size < 10)
		ht_size = 10; // default size for the table


	fprintf(stderr, "Creating table with %i elements\n", ht_size);	
	HashTable *table = new_table(ht_size);
	
	int i = 0;

	int isOver = FALSE;
	while (!isOver)
	{
		if ( fgets(hash, hashLen+1, input_file) == NULL)
		{
			isOver = TRUE;
		}
		else
		{
			fgetc(input_file); // read only one char (separator)
			fgets(clear, 2048, input_file);
			clear[strlen(clear)-1] = '\0'; // remove the end
			// remove the /n at the end of clear
			add_item(table, hash, clear);

			i++;
			if (i%1000000 == 0)
				fprintf(stderr, "STORED %i nodes\n", i);
			
		}

    }

	fprintf(stderr, "STORED %i nodes\n", i);

	prompt(table, hashLen);

	fprintf(stderr, "Freeing table, please wait\n");

	free(hash);
	free(hashAlgoName);
	fclose(input_file);
	free_HashTable(table);


	return 0;
}
