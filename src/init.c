#include "init.h"

#define BUF_SIZE 64
#define TOK_DELIM " \t\r\n\a\""
// Function written harishankarv, I really liked it so I decided to use it.
char*
mash_readline(void)
{
	char* line = NULL;
	size_t bufsize = 0;  // have getline allocate a buffer for us
	if(getline(&line, &bufsize, stdin) == -1) {
		if(feof(stdin)) {
			exit(EXIT_SUCCESS);
		} else {
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}
	return line;
}

char**
mash_parse(char* cmd_line) {
	int bufsize = BUF_SIZE;
	int position = 0;
	char** tokens = malloc(bufsize * sizeof(char*));
	char* token;

	if(!tokens) {
		fprintf(stderr, "mash: allocation error\n");
		exit(EXIT_FAILURE);
	}

	// Duplicate the cmd_line to avoid modifying the original
	char* cmd_copy = strdup(cmd_line);

	while((token = strsep(&cmd_copy, TOK_DELIM)) != NULL) {
		if(strlen(token) > 0) {
			tokens[position] = token;
			position++;

			if(position >= bufsize) {
				bufsize += 64;
				tokens = realloc(tokens, bufsize * sizeof(char*));
				if(!tokens) {
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
	int position = 0;
	char* cmd;
	char* first_arg = args[0];

	if(first_arg == NULL) {
		return 0;
	} else if (first_arg[0] == '.' && first_arg[1] == '/') {
		size_t len = strlen(first_arg);
        char* new_arg = (char*)malloc(len - 1); // Allocate memory for the new string
        if (new_arg == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return -1;
        }
        strcpy(new_arg, first_arg + 2);
		args[0] = new_arg;
		start_new_process(args);
		free(new_arg);
	} else {
		while(args[position] != NULL) {
		cmd = args[position];
		if(strcmp(cmd, "exit") == 0) {
			return 1;
		} else if(strcmp(cmd, "echo") == 0) {
			mash_echo(args, &position);
		} else if(strcmp(cmd, "pwd") == 0) {
			mash_pwd();
		} else if(strcmp(cmd, "cd") == 0) {
			mash_cd(args, &position);
		} else if(strcmp(cmd, "ls") == 0) {
			mash_ls(args, &position);
		} else if(strcmp(cmd, "mv") == 0) {
			mash_mv(args, &position);
		} else if(strcmp(cmd, "rm") == 0) {
			mash_rm(args, &position);
		} else if(strcmp(cmd, "touch") == 0) {
			mash_touch(args, &position);
		} else if(strcmp(cmd, "mkdir") == 0) {
			mash_mkdir(args, &position);
		} else {
			fprintf(stdout, "MASH: %s: command not found\n", args[position]);
		}
		position++;
	}

	
	}

	return 0;
}

int
init(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	char* cmd_line;
	char** cmd_args;
	int status;

	for(;;) {
		fprintf(stdout, "MASH> ");
		cmd_line = mash_readline();
		cmd_args = mash_parse(cmd_line);
		status = mash_exec(cmd_args);

		if(status) {
			exit(EXIT_FAILURE);
		}

		free(cmd_args);
		free(cmd_line);
	}

	(void)status;


	return 0;
}
