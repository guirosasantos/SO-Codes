#include "list.h"
#define MAX_PRIORITY 1
#define MIN_PRIORITY 10
#define QUANTUM 10

// add a task to the list 
void add(Task *task);

// invoke the scheduler
void schedule();
