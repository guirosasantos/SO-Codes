#include "list.h"
#define MAX_PRIORITY 1
#define MIN_PRIORITY 10
#define QUANTUM 10

// add a task to the list 
void add(Task *task);

// invoke the scheduler
void schedule();

// initialize the priority lists
void initialize_priority_lists();

// create a task
Task *create_task(char *name, int tid, int priority, int burst, int deadline);

// creates a pipe and create a taks from the pipe
void *read_tasks(void *arg);

// create a task from a txt file
void create_tasks_from_file(char *filename);