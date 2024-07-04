#include "mash_process.h"

int
start_new_process(char **args) {
    pid_t childPid;
    switch (childPid = fork()) {
        case -1:
            perror("fork");
            return -1;
        case 0:
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                _exit(EXIT_FAILURE);
            }
            break;
        default:
            wait(NULL);
            break;
    }
    return EXIT_SUCCESS;
}
