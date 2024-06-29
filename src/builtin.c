#include "builtin.h"

#define PATH_MAX        4096    /* # chars in a path name including nul */

void
mash_echo(char** args, int* position) {
	bool NO_NEWlINE = 0;
	*position += 1;

	if (args[*position] == NULL) {
		printf("\n");
		return;
	}

	if(strcmp(args[*position], "-n") == 0) {
		NO_NEWlINE = 1;
		*position += 1;
	}

	for(int i = *position; args[i] != NULL; i++) {
		printf("%s ", args[i]);
	}

	if(!NO_NEWlINE) {
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
mash_cd(char** args, int* position) {
	*position += 1;
	char* dirpath = args[*position];
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
		*position += 1;
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
		} else if(dp->d_name[0] == '.') {
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

	num_cols = num_cols / (max_file_len + 6); // padding of 8 between files

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
		} else {
			filenames[i] = strdup(dp->d_name);
			i++;
		}

	}

	qsort(filenames, file_count, sizeof(char*), compare_strings);

	int num_rows = (file_count + (num_cols - 1)) / num_cols;

	// Checks when terminal is too big
	if(num_rows * num_cols > file_count + num_rows) {
		num_cols = file_count / num_rows;
		if((num_cols * num_rows) < file_count) {
			num_cols++;
		}
	}

	int extra_prints = num_rows - ((num_rows * num_cols) - file_count);

	for(int i = 0; i < num_rows; i++) {
		for(int j = 0; j < num_cols; j++) {
			char* curr_file = filenames[((j * num_rows) + i) % file_count];
			int last_col = num_cols - 1;

			if(extra_prints && j == last_col) {
				printf("%-*s ", max_file_len + 6, curr_file);
				extra_prints--;
			} else if(!extra_prints && j == last_col){
				printf("%-*s ", max_file_len + 6, "");
			} else {
				printf("%-*s ", max_file_len + 6, curr_file);
			}

			// Prints newline, not if reached last row
			if(((j + 1) % num_cols == 0)) {
				printf("\n");
			}
		}

	}

	if(errno != 0) {
		fprintf(stderr, "readdir");
	}

	if(closedir(dirp) == -1) {
		fprintf(stderr, "closedir");
	}

	free(filenames);
}

void
mash_touch(char** args, int* position) {
	int fd;
	*position += 1;
	char* filename = args[*position];
	if(filename == NULL) {
		fprintf(stderr, "touch: filename\n");
		return;
	}

	// Try to open the file, creating it if it does not exist
	fd = open(filename, O_WRONLY | O_CREAT, 0666);
	if(fd == -1) {
		perror("Error opening/creating file");
	}

	// Close the file descriptor immediately since we only want to create it if it doesn't exist
	close(fd);

	// Update the file's access and modification times
	if(utime(filename, NULL) == -1) {
		perror("Error updating timestamps");
	}

}

void
mash_rm(char** args, int* position) {
	*position += 1;
	char* dirpath = args[*position];
	if(remove(dirpath) != 0) {
		printf("rm: no such file or directory: %s\n", dirpath);
	}
}

void
mash_mv_usage(void) {
	fprintf(stderr, "usage: mv source target\n");
	fprintf(stderr, "       mv source ... directory\n");
}

void
mash_mv(char** args, int* position) {
	*position += 1;
	char* oldpath = args[*position];
	if(oldpath == NULL) {
		mash_mv_usage();
		return;
	}
	*position += 1;
	char* newpath = args[*position];

	if(newpath == NULL) {
		mash_mv_usage();
		return;
	}

	if(rename(oldpath, newpath) != 0) {
		printf("mv: rename %s to %s: No such file or directory\n", oldpath, newpath);
	}
}

void
mash_mkdir(char** args, int* position) {
	*position += 1;
	char* directory_name = args[*position];
	if(directory_name == NULL) {
		fprintf(stderr, "usage: mkdir directory_name ...\n");
		return;
	}
	mode_t mode = 0755; // rwxr-xr-x

	if(mkdir(directory_name, mode) == -1) {
		perror("Error creating directory");
		return;
	}
}
