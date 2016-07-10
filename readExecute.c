#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "util.h"
#include "table.h"

#define COMMAND_SIZE 20000
#define CMP(x, y) strncmp(x, y, strlen(y))

typedef enum {ADD, REMOVE, FIND, CLEAR, PRINT_BUCKET, PRINT, RESIZE} command_type;
void execute_command(FILE *pFile, bucket **hashtable, int size);
command_type get_command(char *c);
FILE *get_file(char* filename);

int main(int argc, char **argv)
{
	DIE(argc < 2, "Please enter the initial length of hashtable:\n");
	
	int i = 0; // itearates through args
	int size = 0;
	bucket **hashtable;
	
	size = atoi(argv[1]);
	
	// create the hashtable
	hashtable = create_hashtable(size);
	
	FILE *pFile;
	
	if (argc == 2) {
		// there are no optional parameters, read from stdin
		execute_command(stdin, hashtable, size);
	} else {
		for (int i = 2; i < argc; i++) {
			
			pFile = fopen (argv[i], "r");
			
			DIE (!pFile, "Error when opening file.\n");
			execute_command(pFile, hashtable, size);
			
			fclose (pFile);
		}
	}
}

void execute_command(FILE *pFile, bucket **hashtable, int size)
{
	char command_name[COMMAND_SIZE];
	FILE *pFile_out;

	char* r = fgets (command_name, COMMAND_SIZE , pFile);
	
	DIE (r < 0, "Error when reading file.\n");
	
	char *token = strtok(command_name, " ");
	DIE (!token, "Invalid command.\n");
	
	command_type command = get_command(token);
	printf("Command is: %d", command);

	token = strtok( NULL, " ");
	
	switch(command) {
		case ADD :
			DIE (!token, "Please provide the word to be added.\n");
			
			add_hash(token, hashtable, size);
			break;
		
		case REMOVE :
			DIE (!token, "Please provide the word to be removed.\n");			
			
			remove_hash(token, hashtable, size);
			break;
		
		case CLEAR :
			clear_hash(hashtable);
			break;
			
		case RESIZE :
			resize_hash("double", hashtable);
			break;
		
		case FIND :
			DIE (!token, "Please provide the word to be found.\n");		
			
			// file ouput is the 3rd argument, split again
			token = strtok( NULL, " ");
			pFile_out = get_file(token);
			
			find_hash(token, hashtable, size, pFile_out);
			
			fclose(pFile_out);
			break;
		
		case PRINT :
			// file ouput is the 2nd argument (already splitted)
			pFile_out = get_file(token);
			
			print(hashtable, pFile_out);
			
			fclose(pFile_out);
			break;
			
		case PRINT_BUCKET :
		
			DIE (!token, "Please provide the Bucket Index.\n");
			
			// file ouput is the 3rd argument, split again
			token = strtok( NULL, " ");
			pFile_out = get_file(token);
			
			int index = atoi(token);
			print_bucket(index, hashtable, pFile_out);
			
			fclose(pFile_out);
			break;	
	}
	
}

command_type get_command(char *c)
{
	if (CMP(c, "add") == 0)
		return ADD;
	else if (CMP(c, "find") == 0)
		return FIND;
	else if (CMP(c, "print_bucket") == 0)
		return PRINT_BUCKET;
	else if (CMP(c, "print") == 0)
		return PRINT;
	else if (CMP(c, "remove") == 0)
		return REMOVE;
	else if (CMP(c, "clear") == 0)
		return CLEAR;			
	else if (CMP(c, "resize") == 0)
		return RESIZE;				
}

FILE *get_file(char* filename)
{
	FILE *fp;
	if (filename == NULL)
	{
		fp = stdout;
	} else {
		fp = fopen (filename, "a");
					
		DIE(!fp, "Error when opening the file to write");
	}
	
	return fp;
}