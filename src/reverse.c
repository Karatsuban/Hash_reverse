#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_to_table.h"
#include "hash_passwords.h"

#ifndef BOOLEAN
#define BOOLEAN
#define TRUE 1
#define FALSE 0
#endif


int main(int argc, char *argv[])
{

	/*
	expected arguments:
	-G : hash generation (REQUIRED
	-L : hash lookup (REQUIRED)

	Generate parameters:
	-o : output file (T3C table) (REQUIRED)
	-d <value> : drop password with length > value (OPTIONAL)

	Lookup parameters:
	-i : T3C table (REQUIRED)

	*/

	if (argc < 4)
	{
		printf("Fatal error: expected at least 3 arguments, received %i\nAborting\n", argc-1);
		exit(1);
	}

	int mode = -1;
	char *filename = NULL;
	int argsOK = FALSE;

	char *hashAlgoName = "SHA256";
	int chosenHash = FALSE;

	// get the mode
	if (strcmp(argv[1], "-G") == 0)
		mode = 0;
	else if (strcmp(argv[1], "-L") == 0)
		mode = 1;

	// verify the arguments
	switch (mode)
	{
		case 0: // generate
			if (strcmp(argv[2], "-o") == 0)
			{
				filename = malloc(strlen(argv[3])+1);
				strcpy(filename, argv[3]);
				argsOK = TRUE;
			}
			else
			{
				printf("'-o' expected, %s received\n", argv[2]);
			}
			int index = 4;
			while (index < argc)
			{
				if (strcmp(argv[index], "-a") == 0)
				{
					if (index+1<argc)
					{
						if (chosenHash)
							free(hashAlgoName); // avoid memory leak in case this argument is repeated
						index += 1;
						hashAlgoName = malloc(sizeof(argv[index])+1);
						strcpy(hashAlgoName, argv[index]);
						chosenHash = TRUE;
					}
				}
				index += 1;
			}
			break;
		case 1: // lookup
			if (strcmp(argv[2], "-i") == 0)
			{
				filename = malloc(strlen(argv[3])+1);
				strcpy(filename, argv[3]);
				argsOK = TRUE;
			}
			else
			{
				printf("'-i' expected, %s received\n", argv[2]);
			}
			

			break;
		default:
			printf("Fatal error: expected -G or -L, received %s\n", argv[0]);
	};


	if (!argsOK)
	{
		printf("Aborting\n");
		exit(1);
	}

	switch (mode)
	{
		case 0: // generate
			generate(filename, hashAlgoName);
			break;
		case 1: // lookup
			lookup(filename);
			break;
	};

	free(filename);
	if (chosenHash)
		free(hashAlgoName);

	return 0;
}
