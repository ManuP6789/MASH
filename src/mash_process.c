#include "mash_process.h"

int
start_new_process(char **args) {
    (void) args;
    pid_t childPid;
    switch (childPid = fork()) {
        case -1:
            perror("fork");
            return -1;
        case 0:
            printf("running child process\n");
            _exit(EXIT_SUCCESS);
        default:
            printf("parent\n");
            wait(NULL);
            break;
    }
    return EXIT_SUCCESS;
}
