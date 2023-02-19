#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <math.h>
#define SIGNAL_BEGIN SIGUSR1    
#define SIGNAL_END SIGUSR2      

void child_err(int)
{
    fprintf(stderr, " ERROR: Child unexpectedly changed status.\n");
    exit(2);
}

double f(double x, int N)
{
    double sum = 0;
    int temp = 1, i = 1;
    for (int k = 1; k < N + 1; k++)
    {
       sum += pow (-1, k + 1) * pow(x, 2 * k - 1) / temp;
        i += 2;
        temp *= i * (i - 1);
        
    }
    return sum;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, " Argument expected\n");
        exit(1);
    }
    FILE* file = fopen("./N.txt", "r");
    if (!file)
    {
        fprintf(stderr, " Can't open N.txt\n");
        exit(1);
    }
    int N = -1;
    if (!fscanf(file, "%i", &N) || N == -1)
    {
        fprintf(stderr, " Can't read N.txt\n");
        fclose(file);
        exit(1);
    }
    fclose(file);
    int num_steps = 1;
    if (!sscanf(argv[1], "%i", &num_steps))
    {
        fprintf(stderr, " Can't read argument\n");
        exit(1);
    }
    if (num_steps <= 0)
    {
        fprintf(stderr, " Bad argument\n");
            exit(1);
    }
    double h = M_PI / num_steps;
    pid_t child_pid;
    double data[2];

    int fifo[2];
    if (pipe(fifo) == -1)
    {
        fprintf(stderr, " Pipe creation failed\n");
        exit(1);
    }
    printf("%d", (pid_t)fork());
    switch (child_pid = fork())
    {
    case (pid_t)-1:
        fprintf(stderr, " Fork error\n");
        break;
    case (pid_t) 0:
    {
        dup2(fifo[0], 0);
        execv("./laba4_1", NULL);
        fprintf(stderr, " Child exec failed\n");
        exit(1);
    }
    }
    signal(SIGCHLD, child_err);
    usleep(10000);
    for (int i = 0; i <= num_steps; i++)
    {
        double x = h * i;
        data[0] = x;
        data[1] = f(x, N);
       if(( write(fifo[1], data, 2 * sizeof(double)))==-1)
		    perror(" Can't write\n");
    }
	int flag;
    for(flag = 0; flag < 10; flag++)
          if (kill(child_pid, SIGNAL_BEGIN)!=-1) 
                break;
    if (flag ==10) {
        perror("Signal send error \n");
        return 0;
    }
    usleep(10000);
    signal(SIGCHLD, SIG_IGN);
    for (flag = 0; flag < 10; flag++)
        if (kill(child_pid, SIGNAL_END)!=-1) 
                break;
        if (flag ==10) {
                perror(" Signal sending error\n");
                return 0;
         }    
    int t;
    wait(&t);
    close(fifo[1]);
    close(fifo[0]);
    return t;
}
