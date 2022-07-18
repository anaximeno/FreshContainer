#ifndef _CONTAINER_LEARN
#define _CONTAINER_LEARN

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>

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

void clone_process(int (func) (void*), int flags) {
    int pid = clone(func, stack_memory(), flags, NULL);
    wait(0);
}

int run(const char* name) {
    // The args array must init with the name of the program
    // and end with a NULL pointer.
    char* _args[] = {(char*) name, NULL};

    execvp(name, _args);

    return EXIT_SUCCESS;
}

void setup_root(const char* folder) {
    // the file system used for test was downloded with the link bellow:
    //  - http://nl.alpinelinux.org/alpine/v3.7/releases/x86_64/alpine-minirootfs-3.7.0-x86_64.tar.gz
    chroot(folder);
    chdir("/");
}

void setup_variables() {
    clearenv();

    setenv("TERM", "xterm-256color", 0);
    setenv("PATH", "/bin/:/sbin/:/usr/bin:/usr/sbin", 0);
}

int run_sh(void *args) {
    run("/bin/sh");
}

int jail(void* args) {
    printf("child process: %d\n", getpid());

    setup_variables();
    setup_root("./root");

    mount("proc", "/proc", "proc", 0, 0);

    clone_process(run_sh, SIGCHLD);

    umount("/proc");
    return EXIT_SUCCESS;
}

int main() {
    printf("parent %d\n", getpid());
    clone_process(jail, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD);
    return EXIT_SUCCESS;
}

#endif
