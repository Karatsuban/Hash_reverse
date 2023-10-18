#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h> 
#include <unistd.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
	char* filename = NULL;

	if (argc == 1)
	{
		printf("Use at least one argument : the path to a txt file!\n");
		exit(1);
	}

	filename = argv[1];

	FILE *input_file = NULL;
	FILE *output_file = NULL;

	size_t mdlen;
	unsigned char* md_buf = malloc(32); // 256 bits
	
	// TODO : check for errors at opening !
	input_file = fopen(filename, "r");
	output_file = fopen("hashes.txt", "w");

	char line[128]; // buffer of length 128 should be enough to store a plain text password

	int isOver = FALSE;

	while (!isOver)
	{
		if (fgets(line, 128, input_file) == NULL)
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
				fprintf(output_file, " %s ", line); // write the clear password to file
				printf("clear is #%s#", line);
				fputc('\n', output_file);
			}
			printf("\n");
		}

	}

	fclose(input_file);
	fclose(output_file);

	return 0;
}

