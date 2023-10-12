#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h> 
#include <unistd.h>


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

	char line[300]; // buffer of length 300 should be enough
	char *string;

	while (1)
	{
		string = fgets(line, 300, input_file);
		if (string == NULL) // end of file reached
			break;
		fprintf(output_file, "%s", line); // write the clear password to file
		EVP_Q_digest(NULL, "SHA256", NULL, line, strlen(line)-1, md_buf, &mdlen);
		for (int i=0; i<32; i++)
		{
			fprintf(output_file, "%x", md_buf[i]); // write the hash to file
		}
		fputc('\n', output_file);

	}

	fclose(input_file);
	fclose(output_file);

	return 0;
}

