#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h> 
#include <unistd.h>
#include "hash_passwords.h"

#ifndef BOOLEAN
#define BOOLEAN
#define TRUE 1
#define FALSE 0
#endif

int generate(char *filename, char* hashAlgoName)
{

	printf("Algo name is %s\n", hashAlgoName);


	if (EVP_get_digestbyname(hashAlgoName) == NULL)
	{
		printf("Unknown Hash Algorithm name '%s'!\nAborting\n", hashAlgoName);
		return 1;
	}


	FILE *output_file = NULL;

	size_t mdlen;
	unsigned char* md_buf = malloc(2048);

	// get the length of a hash produced by this hash algorithm	
	EVP_Q_digest(NULL, hashAlgoName, NULL, "test", 4, md_buf, &mdlen);

	free(md_buf);
	md_buf = malloc(mdlen);


	output_file = fopen(filename, "w");
	fprintf(output_file, "%s %li\n", hashAlgoName, mdlen*2); // write the algorithm used and the lenght of the hash produced

	char line[2048]; // buffer of length 2048 should be enough to store a plain text password

	int isOver = FALSE;
	int nb = 0;

	while (!isOver)
	{
		if (fgets(line, 2048, stdin) == NULL)
		{
			isOver = TRUE;
		}
		else
		{
			line[strlen(line)-1] = '\0'; // removing the '\n' character

			EVP_Q_digest(NULL, hashAlgoName, NULL, line, strlen(line), md_buf, &mdlen);
			for (int i=0; i<mdlen; i++)
			{
				fprintf(output_file, "%02X", md_buf[i]); // write the hash to file
			}
			fprintf(output_file, " %s\n", line); // write the clear password to file


			nb += 1;
			if (nb % 1000000 == 0)
			{
				printf("DONE %i hashes\n", nb);
			}
		}

	}

	free(md_buf);
	fclose(output_file);
	printf("DONE %i hashes\n", nb);
	printf("Correspondance table written in file '%s'\n", filename);

	return 0;
}

