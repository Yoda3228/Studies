#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <math.h>
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
        perror(" Argument expected\n");
        exit(1);
    }
    FILE* file = fopen("./N.txt", "r");
    if (!file)
    {
        perror(" Can't open N.txt\n");
        exit(1);
    }
    int N = -1;
    if (!fscanf(file, "%i", &N) || N == -1)
    {
        perror(" Can't read N.txt\n");
        fclose(file);
        exit(1);
    }
    fclose(file);
    int num_steps = 1;
    if (!sscanf(argv[1], "%i", &num_steps))
    {
        perror(" Can't read argument\n");
        exit(1);
    }
    if (num_steps <= 0)
    {
        perror(" Bad argument\n");
        exit(1);
    }
    double h = M_PI / num_steps;
    pid_t child_pid;
    struct flock lock, lock_info;
    double data[2];
    int flag = 0, temp;
    const char* temp_file = "/tmp/summ";
    int fd;
    fd = open(temp_file, O_RDWR | O_TRUNC | O_CREAT, 0644);
    if (fd == -1)
    {
        perror(" Can't open temp file\n");
        exit(1);
    }
    close(fd);
    switch (child_pid = fork())
    {
    case (pid_t)-1:
        perror(" Fork error\n");
        break;
    case (pid_t)0:
    {
        child_pid = getpid();
        if ((fd = open(temp_file, O_RDWR)) == -1)
        {
            perror(" Can't open temp file\n");
            exit(1);
        }
        while (flag != -1)
        {
            lock.l_len = 0;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_type = F_WRLCK;
            while (fcntl(fd, F_SETLK, &lock) == -1)
            {
                fcntl(fd, F_GETLK, &lock_info);
            }
            lseek(fd, 0, SEEK_SET);
            if((read(fd, &flag, sizeof(int)))==-1)
			 perror(" Can't read\n");
            if (flag == 1)
            {
                if((read(fd, data, 2 * sizeof(double)))==-1)
				 perror(" Can't read\n");
                printf("f(%lf) = %lf\n", data[0], data[1]);
                lseek(fd, 0, SEEK_SET);
                temp = 0;
               if( (write(fd, &temp, sizeof(int)))==-1)
				    perror(" Can't write\n");
            }
            lock.l_len = 0;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            usleep(1000);
        }
        break;
    }

    default:
    {
        int fd = open(temp_file, O_RDWR);
        for (int i = 0; i <= num_steps; i++)
        {
            double x = h * i;
            usleep(1000);
            lock.l_len = 0;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_type = F_WRLCK;
            while (fcntl(fd, F_SETLK, &lock) == -1)
            {
                fcntl(fd, F_GETLK, &lock_info);
            }
            lseek(fd, 0, SEEK_SET);
            if((read(fd, &flag, sizeof(int)))==-1)
			 perror(" Can't read\n");
            if (flag == 0)
            {
                lseek(fd, 0, SEEK_SET);
                temp = 1;
                 if( (write(fd, &temp, sizeof(int)))==-1)
					   perror(" Can't write\n");
                data[0] = x;
                data[1] = f(x, N);
                if( ( write(fd, data, 2 * sizeof(double)))==-1)
					  perror(" Can't write\n");
            }
            lock.l_len = 0;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
        }
        usleep(1000);
        lock.l_len = 0;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_type = F_WRLCK;
        while (fcntl(fd, F_SETLK, &lock) == -1)
        {
            fcntl(fd, F_GETLK, &lock_info);
        }
        lseek(fd, 0, SEEK_SET);
        if( (read(fd, &flag, sizeof(int)))==-1)
			 perror(" Can't read\n");
        if (flag == 0)
        {
            lseek(fd, 0, SEEK_SET);
            temp = -1;
            if( ( write(fd, &temp, sizeof(int)))==-1)
			  perror(" Can't write\n");
        }
        lock.l_len = 0;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        wait(&temp);
        break;
    }
    }
    close(fd);
    return 0;
}
