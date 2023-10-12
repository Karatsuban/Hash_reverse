#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct HashItem
{
	char *hash;
	char *clear;
} HashItem;

typedef struct HashTable
{
	HashItem** items;
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



HashTable* new_table(int max)
{
	HashTable* table = malloc(sizeof(HashTable));
	table->count = 0;
	table->max = max;
	table->items = malloc(max*sizeof(HashItem*));
	table->item_count = malloc(max*sizeof(int));
	for (int i=0; i<max; i++)
	{
		table->items[i] = NULL;
		table->item_count[i] = 0;
	}
	return table;
}


HashItem* new_item(char *hash, char *clear)
{
	HashItem* I = malloc(sizeof(HashItem));
	I->hash = malloc(strlen(hash)+1);
	I->clear = malloc(strlen(clear)+1);
	strcpy(I->hash, hash);
	strcpy(I->clear,clear);

	return I;
}

void free_HashItem(HashItem *H)
{
	free(H->hash);
	free(H->clear);
	free(H);
}


void free_HashTable(HashTable *T)
{
	for (int i=0; i<T->max; i++)
	{
		if (T->items[i] != NULL)
			free_HashItem(T->items[i]);
	}
	free(T->items);
	free(T->item_count);
	free(T);
}


void add_item(HashTable* table, char* hash, char* clear)
{
	HashItem *item = new_item(hash, clear);

	int index = hash2_function(item->hash, table->max);

	table->item_count[index] += 1;

	if (table->items[index] == NULL)
	{
		table->items[index] = item;
		table->count += 1;
	}
	else
	{
		//HashItem *old = table->items[index];
		/*
		printf("Collision between:\nNEW %s (%s)\nOLD %s (%s)\n\n", 
						item->hash, item->clear, 
						old->hash, old->clear);
		*/
		free_HashItem(item);
	}
}


void display_table(HashTable *table)
{
	HashItem *item = NULL;
	for (int i=0; i<table->max; i++)
	{
		item = table->items[i];
		if (item != NULL)
			printf("%i\t-> %s (%s) (%p)\n", i, item->hash, item->clear, item);
	}
}

void get_item_counts(HashTable *table, char *filename)
{
	FILE *output = fopen(filename, "w");
	if (output == NULL) return;

	for (int i = 0; i<table->max; i++)
		fprintf(output, "%i\n", table->item_count[i]);
	fclose(output);
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

	HashTable *table = new_table(50000);

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

        if (i++ % 10000 == 0)
            printf("%i\n", i);
    }
    
	//display_table(table);

	if (argc >= 3)
		get_item_counts(table, argv[2]);

	fclose(input_file);

	free_HashTable(table);

	return 0;
}
