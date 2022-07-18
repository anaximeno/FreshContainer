#include "../include/common.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>

void write_rule(const char* path, const char* value) {
    int fp = open(path, O_WRONLY | O_APPEND);
    write(fp, value, strlen(value));
    close(fp);
}

void limit_process_creation() {
    char* pid = (char*) malloc(20 * sizeof(char));

    mkdir(CGROUP_FOLDER, S_IRUSR | S_IWUSR);

    sprintf(pid, "%d", getpid());

    // registering the process id to our cgroup
    write_rule(concat(CGROUP_FOLDER, "cgroup.procs"), pid);
    // notify the kernel to cleanup the folder when the process is released
    write_rule(concat(CGROUP_FOLDER, "notify_on_release"), "1");
    // defining the maximum number of processes on our child process
    write_rule(concat(CGROUP_FOLDER, "pids.max"), "5");

    free(pid);
}

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

    int res = execvp(name, _args);

    if (res < 0) {
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void setup_root(const char* folder) {
    chroot(folder);
    chdir("/");
}

void setup_variables() {
    clearenv(); /* Clears the env variables */
    setenv("TERM", "xterm-256color", 0);
    setenv("PATH", "/bin/:/sbin/:/usr/bin:/usr/sbin", 0);
}

int run_sh(void *args) {
    return run("/bin/sh");
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
