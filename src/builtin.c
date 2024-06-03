#include "builtin.h"

#define PATH_MAX        4096    /* # chars in a path name including nul */

void
mash_echo(char** cmd, int position, int NO_NEWLINE) {
	for(int i = position; cmd[i] != NULL; i++) {
		printf("%s ", cmd[i]);
	}

	if(!NO_NEWLINE) {
		printf("\n");
	}
}

void
mash_pwd(void) {
	char cwdbuf[PATH_MAX];
	if(getcwd(cwdbuf, PATH_MAX) != NULL) {
		printf("%s\n", cwdbuf);
	} else {
		if(errno == ERANGE) {
			fprintf(stderr, "Error: Path is too big\n");
		} else {
			perror("getcwd");
		}
	}
}

void
mash_cd(char* dirpath) {
	if(chdir(dirpath) != 0) {
		printf("cd: no such file or directory: %s\n", dirpath);
	}
}

int
compare_strings(const void* a, const void* b) {
	const char** strA = (const char**)a;
	const char** strB = (const char**)b;
	return strcmp(*strA, *strB);
}

void
mash_ls(char** args, int* position) {
	DIR* dirp;
	struct dirent* dp;
	char* dirpath;
	char cwdbuf[PATH_MAX];
	char* flags = args[*position + 1];
	int file_count = 0;
	int max_file_len = 0;
	int num_cols;
	struct ttysize w;

	if(flags == NULL) {
		dirpath = getcwd(cwdbuf, PATH_MAX);
		if(dirpath == NULL) {
			if(errno == ERANGE) {
				fprintf(stderr, "Error: Path is too big\n");
			} else {
				perror("getcwd");
			}
		}
	} else {
		*position = *position + 1;
		if(strcmp(flags, "-g") == 0) {
			fprintf(stderr, "I am in -g");
		} else if(strcmp(flags, "-n") == 0) {
			fprintf(stderr, "I am in -n");
		} else if(strcmp(flags, "-o") == 0) {
			fprintf(stderr, "I am in -o");
		} else {
			dirpath = flags;
		}
	}

	fprintf(stderr, "flag1\n");
	dirp = opendir(dirpath);

	if(dirp == NULL) {
		fprintf(stderr, "ls: %s: No such file or directory\n", dirpath);
		return;
	}
	errno = 0;

	// Count # of files in given directory
	while((dp = readdir(dirp)) != NULL) {
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
			continue;
		}
		int len = strlen(dp->d_name);
		if(len > max_file_len) {
			max_file_len = len;
		}
		file_count++;
	}

	// moves the directory stream back to the beginning so that the next call to readdir()
	rewinddir(dirp);

	// Get size of terminal
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	num_cols = w.ts_cols;

	num_cols = num_cols / (max_file_len + 7); // padding of 8 between files
	if(num_cols == 0) {
		num_cols = 1;
	}

	char** filenames = malloc(sizeof(char*) * file_count);
	int i = 0;

	// Second pass: store filenames
	while((dp = readdir(dirp)) != NULL) {
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
			continue;
		} else if(dp->d_name[0] == '.') {
			continue;
		}
		filenames[i] = strdup(dp->d_name);
		i++;
	}

	qsort(filenames, file_count, sizeof(char*), compare_strings);

	int num_rows = (file_count + (num_cols - 1)) / num_cols;

	// Print filenames in grid format
	for(i = 0; i < file_count; i++) {
		printf("%-*s ", max_file_len + 6, filenames[(i * num_rows) % file_count]);

		if((i + 1) % num_cols == 0) {
			printf("\n");
		}
	}
	if(file_count % num_cols != 0) {
		printf("\n");
	}

	if(errno != 0) {
		fprintf(stderr, "readdir");
	}

	if(closedir(dirp) == -1) {
		fprintf(stderr, "closedir");
	}
}
