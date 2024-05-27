#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule_rr_p.h"
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

int main()
{
   pthread_t read_tasks_thread;

   printf("Priority-based Round Robin Scheduler\n");
   printf("=====================================\n");
   printf("Creating list of tasks\n");

   initialize_priority_lists();

   printf("Adding tasks to first list\n");
   create_tasks_from_file("rr-schedule_pri.txt");

   pthread_create(&read_tasks_thread, NULL, read_tasks, NULL);

   printf("\n\nScheduling tasks\n");
   schedule();

   pthread_join(read_tasks_thread, NULL);

   return 0;
}
