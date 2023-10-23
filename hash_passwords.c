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

int generate(char *filename)
{

	FILE *output_file = NULL;

	size_t mdlen;
	unsigned char* md_buf = malloc(32); // 256 bits
	
	// TODO : check for errors at opening !
	output_file = fopen(filename, "w");

	char line[128]; // buffer of length 128 should be enough to store a plain text password

	int isOver = FALSE;
	int nb = 0;

	while (!isOver)
	{
		if (fgets(line, 128, stdin) == NULL)
		{
			isOver = TRUE;
		}
		else
		{
			line[strlen(line)-1] = '\0'; // removing the '\n' character

			if (strlen(line) < 100)
			{
				EVP_Q_digest(NULL, "SHA256", NULL, line, strlen(line), md_buf, &mdlen);
				for (int i=0; i<32; i++)
				{
					fprintf(output_file, "%02x", md_buf[i]); // write the hash to file
				}
				fprintf(output_file, " %s", line); // write the clear password to file
				fputc('\n', output_file);
			}

			nb += 1;
			if (nb % 1000000 == 0)
			{
				printf("DONE %i hashes\n", nb);
			}
		}

	}

	fclose(output_file);
	printf("DONE %i hashes\n", nb);
	printf("Correspondance table written in file '%s'\n", filename);

	return 0;
}

