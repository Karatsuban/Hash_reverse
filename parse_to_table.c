#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

#define TRUE 1
#define FALSE 0

typedef struct HashTable
{
	list_t** items;
	int *item_count;
	int max;
	int count;
} HashTable;


int hash4_function(char *str, int max)
{
	int ret = 0;
	for (int i=0; i<strlen(str); i++)
	{
		ret +=  i*str[i];
	}
	
	return ret%max;
}



int hash5_function(char *str, int max)
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
	table->item_count = malloc(max*sizeof(int));
	for (int i=0; i<max; i++)
	{
		table->items[i] = NULL;
		table->item_count[i] = 0;
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
	free(T->item_count);
	free(T);
}


void add_item(HashTable* table, char* hash, char* clear)
{
	node_t *node = new_node(hash, clear);

	int index = hash5_function(node->hash, table->max);

	if (index < 0)
	{
		//printf("Dropping malformed string %s (index %i)\n", hash, index);
		return;
	}

	
	table->item_count[index] += 1;
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

int has_password(HashTable* table, char* hash, char *PTR_clear)
{
	int index = hash5_function(hash, table->max);

	int ret = 0;

	if (table->items[index] == NULL)
	{
		PTR_clear = NULL;
		ret = FALSE;
	}
	else
	{
		node_t *temp = find_node_by_hash(table->items[index], hash);
		if (temp != NULL)
		{
			PTR_clear = temp->clear;
			ret = TRUE;
		}
		else
		{
			PTR_clear = NULL;
			ret = FALSE;
		}
	}
	return ret;
}

void get_item_counts(HashTable *table, char *filename)
{
	FILE *output = fopen(filename, "w");
	if (output == NULL) return;

	for (int i = 0; i<table->max; i++)
		fprintf(output, "%i\n", table->item_count[i]);
	fclose(output);
}



void prompt(HashTable *table)
{
    int isOver = FALSE;
    char hash[100];
    char *PTR_clear;
    int ret_val;

    while (!isOver)
    {
        printf("Enter a hash: ");
        scanf("%s", hash);
        //printf("entered hash is %s\n", hash);

        if (strcmp(hash, "exit") == 0)
        {
            isOver = TRUE;
        }
		/*
        else
        {

			ret_val = has_password(table, hash, PTR_clear);

			if (ret_val == TRUE)
                printf("Password found! Clear password is '%s'\n", PTR_clear);
            else
                printf("Sorry not found!\n");
        }*/
    }
}



int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("Specify a filename!");
		return 1;
	}

	FILE *input_file;
	char hash[70];
	char clear[50];


	input_file = fopen(argv[1], "r");
	if (input_file == NULL)
	{
		printf("The file '%s' does not exist!\n", argv[1]);
		return -1;
	}


	fseek(input_file, 0, SEEK_END); // seek to end of file
	int size = ftell(input_file); 
	fseek(input_file, 0, SEEK_SET); // seek back to beginning of file


	int ht_size = size/(36*500); // totally arbitrary way to get the size
	// well, 36 is the average of value per line, so size/36 is roughly the number of lines

	//printf("size = %i, ht_size = %i\n", size, ht_size);

	printf("Creating table with %i elements\n", ht_size);	
	HashTable *table = new_table(ht_size);
	
	int i = 0;

	int isOver = FALSE;
	while (!isOver)
	{
		if ( fscanf(input_file, "%s %s", clear, hash) != 2)
		{
			isOver = TRUE;
		}
		else
		{
			add_item(table, hash, clear);
	
			if (i%1000000 == 0)
				printf("%i\n", i);
			i++;
		}

    }
    
	//display_table(table);

	/*
	if (argc >= 3)
		get_item_counts(table, argv[2]);
	*/

	//prompt(table);


	fclose(input_file);
	free_HashTable(table);


	return 0;
}
