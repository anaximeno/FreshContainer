#ifndef _FRESH_CONTAINER_COMMON_LIB
#define _FRESH_CONTAINER_COMMON_LIB

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sched.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define CGROUP_FOLDER "/sys/fs/cgroup/pids/container/"

#define STACK_SIZE 65536

#define concat(a, b) (a"" b)

/* Writes the value inside the file located in path. */
void write_rule(const char* path, const char* value);

/* Limits the resources of the container by configuring its cgroup folder. */
void limit_process_creation();

/* Returns a pointer to the allocated space for the stack memory which will
 * be used to create the new subprocesses.
 */
char* stack_memory();

/* Created a new subprocess by cloning the parent process context. */
int clone_process(int (func) (void*), int flags);

/* Runs a specific script inside the container.
 * It depends on the scripts available on the container environment.
 */
int run(const char* name);

/* Used to set up the root dir of the container. */
void setup_root(const char* folder);

/* Clears and sets up important environment variable for the container. */
void setup_variables();

/* Creates the container, running it in a isolated and resource limited environment. */
int jail(void* args);

#endif
