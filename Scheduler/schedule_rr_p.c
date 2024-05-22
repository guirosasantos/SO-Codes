#include "schedule_rr_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

struct node *priority_lists[9];

// add a task to the list
void add(Task *task)
{
   insert(&priority_lists[task->priority - 1], task);
   printf("inserted task %s on list %d\n", task->name, task->priority);
}

void remove_task(Task *task)
{
   delete (&priority_lists[task->priority - 1], task);
}

// invoke the scheduler
void schedule()
{
   while (1)
   {
      for (int i = 0; i <= 9; i++)
      {
         struct node *current = priority_lists[i];
         while (current != NULL)
         {
            int quantum = 0;
            while (quantum < QUANTUM && current->task->burst > 0)
            {
               printf("Running task %s with priority %d with remaining burst %d\n", current->task->name, current->task->priority, current->task->burst);
               current->task->burst--;
               quantum++;
               sleep(1);
            }
            if (current->task->burst == 0)
            {
               struct node *temp = current;
               remove_task(temp->task);
            }
            if (current->next == NULL)
            {
               current = priority_lists[i];
            }
            else
            {
               current = current->next;
            }
            // Check if there are any new tasks in a higher priority list
            for (int j = i - 1; j >= 0; j--)
            {
               if (priority_lists[j] != NULL)
               {
                  i = j; // start from the higher priority list
               }
            }

            printf("\n");
         }
      }
   }
}

void initialize_priority_lists()
{
   for (int i = 0; i < 10; i++)
   {
      priority_lists[i] = NULL;
   }
}

Task *create_task(char *name, int tid, int priority, int burst, int deadline)
{
   Task *newTask = (Task *)malloc(sizeof(Task));
   newTask->name = name;
   newTask->tid = tid;
   newTask->priority = priority;
   newTask->burst = burst;
   newTask->deadline = deadline;
   add(newTask);
   return newTask;
}

void *read_tasks(void *arg)
{
   int file;
   char *pipefile = "/tmp/pipefile";

   file = open(pipefile, O_RDONLY);
   if (file < 0) {
      perror("open");
      return NULL;
   }

   while (1)
   {
      Task *task = (Task *)malloc(sizeof(Task));
      task->name = (char *)malloc(100); // allocate memory for the name
      read(file, task->name, 100); // read the name
      read(file, &task->tid, sizeof(task->tid)); // read the tid
      read(file, &task->priority, sizeof(task->priority)); // read the priority
      read(file, &task->burst, sizeof(task->burst)); // read the burst
      read(file, &task->deadline, sizeof(task->deadline)); // read the deadline

      add(task);
   }

   close(file);
   return NULL;
}

void create_tasks()
{
   create_task("Task1", 1, 1, 1, 1);
   create_task("Task2", 2, 1, 2, 2);
   create_task("Task3", 3, 1, 4, 4);
   create_task("Task4", 4, 1, 8, 8);
   create_task("Task5", 5, 1, 16, 16);
   create_task("Task6", 6, 1, 32, 32);
   printf("Adding tasks to second list\n");
   create_task("Task11", 11, 2, 1, 1);
   create_task("Task12", 12, 2, 2, 2);
   create_task("Task13", 13, 2, 4, 4);
   create_task("Task14", 14, 2, 8, 8);
   create_task("Task15", 15, 2, 16, 16);
   create_task("Task16", 16, 2, 32, 32);
   ("Adding tasks to third list\n");
   create_task("Task21", 21, 3, 1, 1);
   create_task("Task22", 22, 3, 2, 2);
   create_task("Task23", 23, 3, 4, 4);
   create_task("Task24", 24, 3, 8, 8);
   create_task("Task25", 25, 3, 16, 16);
   create_task("Task26", 26, 3, 32, 32);
   ("Adding tasks to fourth list\n");
   create_task("Task31", 31, 4, 1, 1);
   create_task("Task32", 32, 4, 2, 2);
   create_task("Task33", 33, 4, 4, 4);
   create_task("Task34", 34, 4, 8, 8);
   create_task("Task35", 35, 4, 16, 16);
   create_task("Task36", 36, 4, 32, 32);
   ("Adding tasks to fifth list\n");
   create_task("Task41", 41, 5, 1, 1);
   create_task("Task42", 42, 5, 2, 2);
   create_task("Task43", 43, 5, 4, 4);
   create_task("Task44", 44, 5, 8, 8);
   create_task("Task45", 45, 5, 16, 16);
   create_task("Task46", 46, 5, 32, 32);
}

int main()
{
   pthread_t read_tasks_thread;
   
   printf("Priority-based Round Robin Scheduler\n");
   printf("=====================================\n");
   printf("Creating list of tasks\n");

   initialize_priority_lists();

   printf("Adding tasks to first list\n");
   create_tasks();

   pthread_create(&read_tasks_thread, NULL, read_tasks, NULL);

   printf("\n\nScheduling tasks\n");
   schedule();

   pthread_join(read_tasks_thread, NULL);

   return 0;
}