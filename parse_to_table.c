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


int hash1_function(char *str, int max)
{
	int ret = 0;
	for (int i = 0; i<strlen(str); i++)
		ret += str[i];
	return ret%max;
}


int hash2_function(char *str, int max)
{
	int ret = 0;
	for (int i = 0; i<strlen(str); i++)
	{
		if (i<strlen(str)/2)
			ret += str[i];
		else
			ret += 10*str[i];
	}
	return ret%max;
}

int hash3_function(char *str, int max)
{
	int ret = 0;
	for (int i=1; i<strlen(str); i+=2)
	{
		ret += 10*str[i-1]+str[i];
	}
	return ret%max;
		
}

int hash4_function(char *str, int max)
{
	int ret = 0;
	for (int i=0; i<strlen(str); i++)
	{
		ret += i*str[i];
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

	int index = hash4_function(node->hash, table->max);
	
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

int has_password(HashTable* table, char* hash, char **PTR_clear)
{
	int index = hash4_function(hash, table->max);

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

void get_item_counts(HashTable *table, char *filename)
{
	FILE *output = fopen(filename, "w");
	if (output == NULL) return;

	for (int i = 0; i<table->max; i++)
		fprintf(output, "%i %i\n", table->item_count[i], table->items[i]->size);
	fclose(output);
}



void prompt(HashTable *table)
{
    int isOver = FALSE;
    char hash[100];
    char* PTR_clear = NULL;
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
        else
        {

                ret_val = has_password(table, hash, &PTR_clear);

                if (ret_val == TRUE)
                        printf("Password found! Clear password is '%s'\n", PTR_clear);
                else
                        printf("Sorry not found!\n");
        }
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
	char *string = NULL;

	HashTable *table = new_table(20000);

	input_file = fopen(argv[1], "r");
	if (input_file == NULL)
	{
		printf("The file '%s' does not exist!\n", argv[1]);
		return -1;
	}

	int i = 0;

	while (1)
	{
		string = fgets(clear, 50, input_file);
		if (string == NULL)
			break;
        clear[strlen(clear)-1] = '\0'; // remove the \n
        fgets(hash, 70, input_file); // get the hash
        hash[strlen(hash)-1] = '\0'; // remove the \n
		
		add_item(table, hash, clear);

		if (i%10000 == 0)
			printf("%i\n", i);
		i++;

    }
    
	//display_table(table);


	if (argc >= 3)
		get_item_counts(table, argv[2]);

	prompt(table);

	//char *user_input = NULL;
	//scanf("%s", user_input);
	

	fclose(input_file);
	free_HashTable(table);


	return 0;
}
