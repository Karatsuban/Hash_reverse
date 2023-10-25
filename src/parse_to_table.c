#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"

#ifndef BOOLEAN
#define BOOLEAN
#define TRUE 1
#define FALSE 0
#endif


typedef struct HashTable
{
	list_t** items;
	int max;
	int count;
} HashTable;

int hash_function(char *str, int max)
{
	// custom hash function, creating a number from a string
	int ret = 0;
	for (int i=0; i<strlen(str); i++)
	{
		ret += i*i*str[i];
	}
	
	return ret%max; // number will be in [0,max[
}

HashTable* new_table(int max)
{
	// instances a new HashTable object
	HashTable* table = malloc(sizeof(HashTable));
	table->count = 0;
	table->max = max;
	table->items = malloc(max*sizeof(list_t*)); // malloc the space for all its elements
	for (int i=0; i<max; i++)
	{
		table->items[i] = NULL; // set the elements as NULL
	}
	return table;
}


void free_HashTable(HashTable *T)
{
	// free the contents of a HashTable and the HashTable itself
	for (int i=0; i<T->max; i++)
	{
		if (T->items[i] != NULL)
			free_linked_list(T->items[i]); // free each element
	}
	free(T->items);
	free(T); // free itself
}


void add_item(HashTable* table, char* hash, char* clear)
{
	// add a new node to the HashTable
	node_t *node = new_node(hash, clear); // create the Node

	int index = hash_function(node->hash, table->max); // get its index based on the hash function

	if (index < 0)
	{
		printf("Dropping malformed string %s (index %i)\n", hash, index);
		return;
	}

	table->count += 1;

	if (table->items[index] == NULL)
	{
		// first time storing a node at this index
		table->items[index] = init_linked_list(); // create a linked list
		insert_tail(table->items[index], node); // add this node in the new linked list
	}
	else
	{
		// a linked list already exists at this index
		insert_tail(table->items[index], node); // add this node in the linked list
	}
}

int has_password(HashTable* table, char* hash, char **PTR_clear)
{
	// returns whether the given HashTable contains the given hash
	int index = hash_function(hash, table->max); // get the index of the string based on the hash function

	int ret = 0;

	if (table->items[index] == NULL)
	{
		// there is no linked list at this index
		*PTR_clear = NULL;
		ret = FALSE; // the table does not contain the hash
	}
	else
	{
		// the index contains a linked list
		node_t *temp = find_node_by_hash(table->items[index], hash); // parsing the linked list to try to find the hash
		if (temp != NULL)
		{
			// a node was found with this hash
			*PTR_clear = temp->clear; // store the clear password
			ret = TRUE;
		}
		else
		{
			// NO node found with this hash
			*PTR_clear = NULL;
			ret = FALSE; // table does not contain the hash
		}
	}
	return ret;
}


void upper(char *str)
{
	// parse a string, puttin all its elements to upper case if possible
	char *a = str;
	while (*a)
	{
		*a = toupper(*a);
		a++;
	}
}

void prompt(HashTable *table, int hashSize)
{
	// reads hashes from stdin and try to look them up in the table
    int isOver = FALSE;
    char* hash = malloc(hashSize+1);
    char *PTR_clear = NULL;
    int ret_val;
	int matched= 0;
	int total = 0;

    while (!isOver)
    {
        if (scanf("%s", hash) == EOF) // try to get a hash
        {
            isOver = TRUE;
        }
        else
        {
			total += 1;
			upper(hash); // change to upper case
			ret_val = has_password(table, hash, &PTR_clear); // lookup the hash

			if (ret_val == TRUE)
			{
                printf("MATCH %s %s\n", hash, PTR_clear); // a corresponding clear password was found
				matched += 1;
			}
			else
			{
				fprintf(stderr, "not match %s\n", hash);
			}
        }
    }
	printf("Matched %i/%i\n", matched, total);
	free(hash);
}



int lookup(char* filename)
{
	// loads a hash-clear correspondance table from a file and try to lookup given hashes from stdin
	FILE *input_file;
	char *hash = NULL;
	char clear[2048]; // buffer to contain clear password

	char* hashAlgoName = malloc(100); // will contain the name of the hash algorithm used
	int hashLen = 0; // will contain the lenght of a hash produced with the chosen algo (in bytes)

	input_file = fopen(filename, "r"); // opening the correspondance table file
	if (input_file == NULL)
	{
		fprintf(stderr, "The file '%s' does not exist!\n", filename);
		return 1;
	}

	fseek(input_file, 0, SEEK_END); // seek to end of file
	int size = ftell(input_file); // get the file's size
	fseek(input_file, 0, SEEK_SET); // seek back to beginning of file
	
	int check = fscanf(input_file, "%s %i\n", hashAlgoName, &hashLen); // read the file's first line

	if (strlen(hashAlgoName) == 0 || hashLen == 0 || check != 2)
	{
		// checkin the file's header
		printf("Fatal Error: incorrect header ('%s %i') in file '%s'\nAborting\n", hashAlgoName, hashLen, filename);
		return 1;
	}


	hash = malloc(hashLen+1); // set a buffer of the correct size for the hashes


	int ht_size = size/((hashLen+10)*50); // set the table size
	// (hashLen+10) is a rough average of the number of characters per line
	// we create the table so that is stores more or less 50 nodes in each index

	// min size for the table
	if (ht_size < 10)
		ht_size = 10; 


	fprintf(stderr, "Creating table with %i elements\n", ht_size);	
	HashTable *table = new_table(ht_size); // table creation
	
	int i = 0;
	int isOver = FALSE;

	while (!isOver)
	{
		if ( fgets(hash, hashLen+1, input_file) == NULL) // getting a hash and storing it
		{
			isOver = TRUE;
		}
		else
		{
			fgetc(input_file); // read only one char (separator)
			fgets(clear, 2048, input_file); // read and store the clear password
			clear[strlen(clear)-1] = '\0'; // replace the '\n' at the end with '\0'
			add_item(table, hash, clear); // add the hash and the clear to the table

			i++;
			if (i%1000000 == 0)
				fprintf(stderr, "STORED %i nodes %s %s\n", i, clear, hash);
			
		}

    }

	fprintf(stderr, "STORED %i nodes\n", i);

	prompt(table, hashLen); // try to lookup the given hashes

	fprintf(stderr, "Freeing table, please wait\n");

	// free the structures
	free(hash);
	free(hashAlgoName);
	free_HashTable(table);

	// close the file
	fclose(input_file);

	return 0;
}
