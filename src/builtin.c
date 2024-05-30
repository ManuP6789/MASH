#include "builtin.h"

void
mash_echo(char** cmd, int position) {
	for (int i = position + 1; cmd[i] != NULL; i++) {
		fprintf(stdout, "%s ", cmd[i]);
	}
	fprintf(stdout, "\n");
}
