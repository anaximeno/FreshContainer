#ifndef _CONTAINER_LEARN
#define _CONTAINER_LEARN

#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE 65536 // 16 bits - 2 bytes

char* stack_memory() {
    char* stack = (char*) malloc(STACK_SIZE * sizeof(char));

    if (stack == NULL) {
        printf("Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // return a pointer to the end of the array
    // since the stack grows backward.
    return stack + STACK_SIZE;
}

int run(const char* name) {
    // The args array must init with the name of the program
    // and end with a NULL pointer.
    char* _args[] = {(char*) name, NULL};
    execvp(name, _args);
    return EXIT_SUCCESS;
}

int jail(void* args) {
    // remove all env variables
    clearenv();
    printf("child process: %d\n", getpid());
    run("/bin/bash");
    return EXIT_SUCCESS;
}

int main() {
    printf("Hello, World (parent)\n");
    printf("parent %d\n", getpid());

    // clone this proccess, creating a child process running jail
    int pid = clone(jail, stack_memory(), CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD, NULL);

    if (pid < 0) {
        printf("Could not create a subprocess\n");
        exit(EXIT_FAILURE);
    }

    // wait for the child proccess to finish executing
    wait(0);

    return EXIT_SUCCESS;
}

#endif
