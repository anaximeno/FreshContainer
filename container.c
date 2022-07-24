#include "include/common.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    int result;

    printf("PID: %d\n", getpid());

    limit_process_creation();

    result = clone_process(jail, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD);

    if (result < 0)
        printf("The cloning didn't work, try run using root.\n");

    return EXIT_SUCCESS;
}
