#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h> 
#include <unistd.h>

#define FALSE 0
#define TRUE 1

typedef struct TreeNode {
	char *clear;
	int has_passwd;
	struct TreeNode **kids;
	struct TreeNode *prev;
} TreeNode;

typedef struct TreeHead {
	struct TreeNode *first;
	int nb_kids;
	int nb_passwords; // nodes actually containing passwords
	int nb_nodes; // 'useless' nodes
} TreeHead;



TreeNode* init_TreeNode(char *clear, char *hash, int has_passwd)
{
	TreeNode* node = malloc(sizeof(TreeNode));
	node->clear = clear;
	node->has_passwd = has_passwd;
	node->kids = malloc(16*sizeof(TreeNode));
	for (int i=0; i<16; i++)
		node->kids[i] = NULL;
	node->prev = NULL;
	return node;
}


int offset(int val)
{
	// maps the digits and the alphabet to an index from 0 to 35
	if (val >= 48 && val <= 57)
		return val-48;
	if (val >= 97 && val <= 122)
		return val-97+10;
	return val;
}



TreeHead* init_TreeHead()
{
	TreeHead* head = malloc(sizeof(TreeHead));
	head->first = init_TreeNode(NULL, " ", FALSE);
	head->first->prev = head->first; // TODO : NULL or itself ?
	return head;
}

TreeNode* create_node_after(TreeNode* current, int index)
{
	TreeNode *new = init_TreeNode(NULL, NULL, FALSE);
	current->kids[index] = new;
	new->prev = current; // attach to prev
	return new;
}


void add_password(TreeHead *head, char *clear, char *hash)
{
	TreeNode *current = head->first;
	int real_id;
	
	for(int i=0; i<strlen(hash); i++)
	{
		real_id = offset(hash[i]);
		
		if (current->kids[real_id] == NULL)
		{
			create_node_after(current, real_id);
			head->nb_kids += 1;
			head->nb_nodes += 1;
		}
	
		current = current->kids[real_id];	
	}
	// set the current node as a password-holding node
	if (!current->has_passwd)
	{
		current->has_passwd = TRUE;
		head->nb_passwords += 1;
		head->nb_nodes -= 1;
	}
	current->clear = malloc(strlen(clear));
	strcpy(current->clear, clear);
}


int has_password(TreeHead *head, char *hash, char **PTR_clear)
{
	// if the hash exists, then TRUE is returned and the clear password is stored in clear
	TreeNode *current = head->first;
	int real_id;
	for (int i=0; i < strlen(hash); i++)
	{
		real_id = offset(hash[i]);
		/*printf("%c %i | ", hash[i], real_id);
	        for (int k=0; k<16; k++)
		{
			if (current->kids[k] != NULL)
				printf("%i ", k); 
		}
		printf("\n");
		

		if (current->has_passwd)
		{
			printf("This one has a password! %s\n", current->clear);
		}
		*/
	
		if (current->kids[real_id] == NULL)
		{
			return FALSE; // not in here !
		}
		current = current->kids[real_id]; // TODO : this line cause segfault... why ?
	}
	*PTR_clear = current->clear;
	return TRUE;
}

void display_info(TreeNode* node)
{
        printf("clear = %s\n", node->clear);
        for (int i=0; i<16; i++)
	{
		if (node->kids[i] != NULL)
			printf("%i ", i); 
	}
	printf("\n");
}


void interactive(TreeNode *head)
{
	int isOver = FALSE;
	int val = 0;
	TreeNode *current = head;

	while (!isOver)
	{
		display_info(current);
		printf("Operation ? ");
		scanf("%i", &val);

		if (val == -1)
		{
			printf("Going UP a node\n");
			current = current->prev;
		}
		if (val == 16)
		{
			printf("Going to HEAD node\n");
			current = head;
		}
		if (val >=0 && val < 16)
		{
			if (current->kids[val] != NULL)
			{
				printf("Going to KID %i node\n", val);
				current = current->kids[val];
			}
		}
	}
}


void prompt(TreeHead *head)
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

		        ret_val = has_password(head, hash, &PTR_clear);

		        if (ret_val == TRUE)
	        	        printf("Password found! Clear password is '%s'\n", PTR_clear);
		        else
		                printf("Sorry not found!\n");
		}
	}
}

void info_on_head(TreeHead *head)
{
	printf("%i kids\n%i nodes\n%i passwords\n", head->nb_kids, head->nb_nodes, head->nb_passwords);
	printf("Occupancy %f%%\n", 100.0*head->nb_passwords/head->nb_kids);
}

int main(int argc, char* argv[])
{
	FILE *input_file = NULL;
	char hash[70];
	char clear[50];

	TreeHead *head = init_TreeHead();

	input_file = fopen("hashes.txt", "r");

	char *string;

	int i=0;
	
	while (1)
	{
		string = fgets(clear, 50, input_file); // get the clear password
		if (string == NULL) // we've reached the end of the file
			break;
		clear[strlen(clear)-1] = '\0'; // remove the \n
		fgets(hash, 70, input_file); // get the hash
		hash[strlen(hash)-1] = '\0'; // remove the \n
		add_password(head, clear, hash); // add the clear and hash in the tree
		if (i++ % 10000 == 0)
			printf("%i\n", i);
	}

	fclose(input_file); // closing the file

	info_on_head(head);

	prompt(head);

	//interactive(head->first);

	return 0;
}

