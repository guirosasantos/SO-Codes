#include "schedule_rr_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>

struct node *priority_lists[9];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// add a task to the list
void add(Task *task)
{
   pthread_mutex_lock(&lock);
   insert(&priority_lists[task->priority - 1], task);
   pthread_mutex_unlock(&lock);
   printf("inserted task %s on list %d\n", task->name, task->priority);
}

void remove_task(Task *task)
{
   delete (&priority_lists[task->priority - 1], task);
   free(task->name); // Free the memory allocated for the name
   free(task);       // Free the memory allocated for the task
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
               usleep(500000);
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
                  current = priority_lists[j];
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

   newTask->name = (char *)malloc(strlen(name) + 1);
   strcpy(newTask->name, name);

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
   if (file < 0)
   {
      perror("open");
      return NULL;
   }

   while (1)
   {
      Task *task = (Task *)malloc(sizeof(Task));
      task->name = (char *)malloc(100);                    // allocate memory for the name
      read(file, task->name, 100);                         // read the name
      read(file, &task->tid, sizeof(task->tid));           // read the tid
      read(file, &task->priority, sizeof(task->priority)); // read the priority
      read(file, &task->burst, sizeof(task->burst));       // read the burst
      read(file, &task->deadline, sizeof(task->deadline)); // read the deadline

      add(task);
   }

   close(file);
   return NULL;
}

void create_tasks_from_file(char *filename)
{
   FILE *file = fopen(filename, "r");
   if (file == NULL)
   {
      printf("Could not open file %s\n", filename);
      return;
   }

   char line[256];
   while (fgets(line, sizeof(line), file))
   {
      char *name = strtok(line, ", ");
      int tid = atoi(strtok(NULL, ", "));
      int priority = atoi(strtok(NULL, ", "));
      int burst = atoi(strtok(NULL, ", "));
      int deadline = atoi(strtok(NULL, ", "));

      create_task(name, tid, priority, burst, deadline);
   }

   fclose(file);
}
