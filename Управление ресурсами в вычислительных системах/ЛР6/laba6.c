#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int Close(int fd)
{
    if (close(fd) == -1)
    {
        perror("Close error ");
        exit(errno);
    }
    return 0;
}
int main()
{
    int flag = 1;
    fprintf(stdout, "Start\n");
    int pip1[2], pip2[2];
    pid_t proc1, proc2, proc3;
    if ((pipe(pip1) < 0) || (pipe(pip2) < 0))
    {
        perror("Сanal creation error\n");
        exit(errno);
    }
    printf("Canals create\n");
    proc1 = fork();
    if (proc1 == -1) 
    {
        perror("Error: open child 1\n");
        sleep(6);
        exit(errno);
    }
    else if (proc1 == 0)
    {
        fprintf(stdout, "Process 1 create\n");
        Close(pip1[0]);
        Close(pip2[0]);
        Close(pip2[1]);
        Close(1);
        fprintf(stdout, "Descriptors are closed\n");
        if (dup(pip1[1]) == -1)
        {
            perror("Error: canal 1\n");
            exit(errno);
        }
        fprintf(stdout, "Output redirected\n");
        if (execlp("ls", "ls", "-al", NULL) == -1)
        {
            perror("Error: 'ls'\n");
            exit(errno);
        }
        exit(0);
    }
    sleep(2);
    fprintf(stdout, "ls -al done\n");
    fprintf(stdout, "Process 1 exit\n");
    proc2 = fork(); 
    if (proc2 == -1)
    {
        perror("Error: open child 2\n");
        sleep(6);
        exit(errno);
    }
    else if (proc2 == 0)
    {
        fprintf(stdout, "Process 2 create\n");
        Close(pip1[1]);
        Close(pip2[0]);
        Close(0);
        fprintf(stdout, "Descriptors are closed\n");
        if (dup(pip1[0]) == -1)
        {
            perror("Error: canal 1\n");
            exit(errno);
        }
        if (dup(pip2[1]) == -1) 
        {
            perror("Error: canal 1\n");
            exit(errno);
        }

        if (execlp("wc", "wc", "-l", NULL) == -1)
        {
            perror("Error: execlp\n");
            return 0;
        }
        exit(0);
    }
    sleep(2);
    fprintf(stdout, "wc -l done\n");
    fprintf(stdout, "Process 2 exit\n");
    proc3 = fork(); 
    if (proc3 == -1)
    {
        perror("Error: open child 3\n");
        sleep(6);
        exit(errno);
    }
    else if (proc3 == 0)
    {
        fprintf(stdout, "Process 3 create\n");
        Close(pip1[0]);
        Close(pip1[1]);
        Close(pip2[1]);
        Close(pip2[0]);
        close(0);
        close(1);
        fprintf(stdout, "Descriptors are closed\n");
        fprintf(stdout, "Input redirected\n");
        int fd_tmp = open("c.txt", O_WRONLY | O_CREAT, 0666);
 	 if (dup2(fd_tmp, STDOUT_FILENO) == -1) 
        {
            perror("Error: out\n");
            exit(errno);
        }

        Close(fd_tmp);
        if (execlp("cat", "cat", "a.txt", "b.txt", NULL) == -1)
        {
            perror("Error: execl\n");
            exit(errno);
        }
        exit(0);
    }
    sleep(2);
    fprintf(stdout, "cat a.txt b.txt > c.txt done\n");
    fprintf(stdout, "Process 3 exit\n");
    Close(pip1[0]);
    Close(pip1[1]);
    Close(pip2[0]);
    Close(pip2[1]);
    fprintf(stdout, "Canals are closed\n");
    wait(&proc1);
    wait(&proc2);
    wait(&proc3);
    fprintf(stdout, "Children process finished\nFinish\n");
    return 0;
}
