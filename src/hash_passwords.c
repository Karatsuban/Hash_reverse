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
	// reading a file containing passwords and creating a correspondance table
	// between them and their calculated hashes digested by the given hash algo

	if (EVP_get_digestbyname(hashAlgoName) == NULL)
	{
		// checks wether the given hash algo exists
		printf("Unknown Hash Algorithm name '%s'!\nAborting\n", hashAlgoName);
		return 1;
	}


	FILE *output_file = NULL;

	size_t mdlen;
	unsigned char* md_buf = malloc(2048); // temporary buffer

	// get the length of a hash produced by this hash algorithm	
	EVP_Q_digest(NULL, hashAlgoName, NULL, "test", 4, md_buf, &mdlen);

	free(md_buf); //
	md_buf = malloc(mdlen); // creating a buffer of the perfect length for sotring the hashes


	output_file = fopen(filename, "w");
	if (output_file == NULL)
	{
		// checks whether the file is opened correctly
		fprintf(stderr, "Fatal Error: can't open file '%s'\n", filename);
		return 1;
	}

	fprintf(output_file, "%s:%li\n", hashAlgoName, mdlen*2); // write the algorithm used and the lenght of the hash produced

	char line[2048]; // buffer of length 2048 should be enough to store a plain text password

	int isOver = FALSE;
	int nb = 0;

	while (!isOver)
	{
		if (fgets(line, 2048, stdin) == NULL) // get the password and store it
		{
			isOver = TRUE;
		}
		else
		{
			line[strlen(line)-1] = '\0'; // removing the '\n' character

			EVP_Q_digest(NULL, hashAlgoName, NULL, line, strlen(line), md_buf, &mdlen); // calculate the hash
			for (int i=0; i<mdlen; i++)
			{
				fprintf(output_file, "%02X", md_buf[i]); // write the hash in base64 to file
			}
			fprintf(output_file, " %s\n", line); // write a space and the clear password to file


			nb += 1;
			if (nb % 1000000 == 0)
			{
				printf("DONE %i hashes\n", nb);
			}
		}

	}

	free(md_buf); // free the buffers
	fclose(output_file); // close the opened file
	printf("DONE %i hashes\n", nb);
	printf("Correspondance table written in file '%s'\n", filename);

	return 0;
}

