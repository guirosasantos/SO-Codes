#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "task.h"
#include <stdlib.h>

Task CreateTask()
{
    Task *task = (Task *)malloc(sizeof(Task));
    
    char* name = (char *)malloc(100);
    int tid, priority, burst, deadline;

    printf("Enter task name: ");
    scanf("%99s", name);
    task->name = name;
    printf("Enter task tid: ");
    scanf("%d", &task->tid);
    printf("Enter task priority: ");
    scanf("%d", &task->priority);
    printf("Enter task burst: ");
    scanf("%d", &task->burst);
    printf("Enter task deadline: ");
    scanf("%d", &task->deadline);
    return *task;
}

int main()
{
    int file;
    char *pipefile = "/tmp/pipefile";

    printf("Creating tasks\n");
    mkfifo(pipefile, 0666);
    file = open(pipefile, O_WRONLY);
    printf("Pipe created\n");

    while (1)
    {
        Task task = CreateTask();
        printf("\n Sending task %s with priority %d to the scheduler\n", task.name, task.priority);
        task.name[99] = '\0'; 
        int name_length = strlen(task.name) + 1;
        write(file, &name_length, sizeof(name_length));
        write(file, task.name, name_length);
        write(file, &task.tid, sizeof(task.tid));
        write(file, &task.priority, sizeof(task.priority));
        write(file, &task.burst, sizeof(task.burst));
        write(file, &task.deadline, sizeof(task.deadline));
    }
    sleep(1);

    close(file);
    unlink(pipefile);

    return 0;
}