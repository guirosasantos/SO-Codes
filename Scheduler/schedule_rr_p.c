#include "schedule_rr_p.h"
#include <stdio.h>
#include <stdlib.h>

struct node *priority_lists[MAX_PRIORITY];

// add a task to the list
void add(Task *task)
{
   insert(&priority_lists[task->priority], task);
}

void remove(Task *task)
{
   delete (&priority_lists[task->priority], task);
}

// invoke the scheduler
void schedule() {
   while (1) {
      for (int i = MAX_PRIORITY - 1; i >= MIN_PRIORITY; i--) {
         struct node *current = priority_lists[i];
         while (current != NULL) {
            int quantum = 0;
            while (quantum < QUANTUM && current->task->burst > 0) {
               printf("Running task %s [%d] [%d] [%d]\n", current->task->name, current->task->tid, current->task->priority, current->task->burst);
               current->task->burst--;
               quantum++;
            }
            if (current->task->burst == 0) {
               remove(current->task);
               current = priority_lists[i]; // start from the beginning of the list
            } else {
               current = current->next;
            }
            // Check if there are any new tasks in a higher priority list
            for (int j = i - 1; j >= 0; j--) {
               if (priority_lists[j] != NULL) {
                  i = j; // start from the higher priority list
               }
            }
         }
      }
   }
}

void initialize_priority_lists()
{
   for (int i = 0; i < MAX_PRIORITY; i++)
   {
      priority_lists[i] = NULL;
   }
}

Task *create_task(char *name, int tid, int priority, int burst, int deadline)
{
   Task *newTask = (Task *)malloc(sizeof(Task));
   newTask->name = strdup(name); // duplicate the string to avoid memory issues
   newTask->tid = tid;
   newTask->priority = priority;
   newTask->burst = burst;
   newTask->deadline = deadline;
   add(newTask);
   return newTask;
}