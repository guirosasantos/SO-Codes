#define MIN_PRIORITY 10
#define MAX_PRIORITY 1
#include "task.h"

// add a task to the list 
void add(Task *task);

// invoke the scheduler
void schedule();

// remove task and free memory
void remove_task(Task *task);