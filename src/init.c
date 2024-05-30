#include "init.h"

#define BUF_SIZE 64
// Function written harishankarv, I really liked it so I decided to use it.
char * 
mash_readline(void) 
{
	char *line = NULL;
	size_t bufsize = 0;  // have getline allocate a buffer for us
	if (getline(&line, &bufsize, stdin) == -1) {
		if (feof(stdin)) {
		exit(EXIT_SUCCESS);
		} else {
		perror("readline");
		exit(EXIT_FAILURE);
		}
	}
	return line;
}

char ** 
mash_parse(char* cmd_line) {
	int bufsize = BUF_SIZE;
    	int position = 0;
    	char **tokens = malloc(bufsize * sizeof(char*));
    	char *token;

	if (!tokens) {
		fprintf(stderr, "mash: allocation error\n");
		exit(EXIT_FAILURE);
    	}	

	// Duplicate the cmd_line to avoid modifying the original
    	char *cmd_copy = strdup(cmd_line);

	while ((token = strsep(&cmd_copy, " \t\r\n\a")) != NULL) {
		if (strlen(token) > 0) {
			tokens[position] = token;
			position++;

			if (position >= bufsize) {
				bufsize += 64;
				tokens = realloc(tokens, bufsize * sizeof(char*));
				if (!tokens) {
					fprintf(stderr, "mash: allocation error\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}

	tokens[position] = NULL;
	free(cmd_copy); // Free the duplicated command line
	return tokens;
}

int
mash_exec(char** args) {
	int i = 0;

	if (args[0] == NULL)
		return 0;	


	while (args[i] != NULL) {
		if (strcmp(args[i], "exit") == 0) {
			return 1;
		} else if (strcmp(args[i], "echo") == 0) {
			mash_echo(args, i);
			return 0;
		} else {
		        fprintf(stdout, "MASH: %s: command not found\n", args[i]);
		}
		i++;
        } 

	return 0;
}

int 
init(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char *cmd_line;
	char **cmd_args;
	int status;

	for(;;) {
		fprintf(stdout, "MASH> ");
		cmd_line = mash_readline();
		cmd_args = mash_parse(cmd_line);
		status = mash_exec(cmd_args);

		if (status) {
			exit(EXIT_FAILURE);
		}

		free(cmd_args);
		free(cmd_line);
	}

	(void)status;

	
	return 0;
}
