#ifndef BUILTIN_H
#define BUILTIN_H /* Prevent accidental double inclusion */
#include "common.h"
#include "dirent.h"

void mash_echo(char** cmd, int position, int no_newline);
void mash_pwd(void);
void mash_cd(char** args, int* position);
void mash_ls(char** args, int* position);

#endif
