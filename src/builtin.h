#ifndef BUILTIN_H
#define BUILTIN_H /* Prevent accidental double inclusion */
#include "common.h"
#include "dirent.h"

void mash_echo(char** args, int* position);
void mash_pwd(void);
void mash_cd(char** args, int* position);
void mash_ls(char** args, int* position);
void mash_mv(char** args, int* position);
void mash_rm(char** args, int* position);
void mash_touch(char** args, int* position);
void mash_mkdir(char** args, int* position);

#endif
