#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#define SIGNAL_BEGIN SIGUSR1    
#define SIGNAL_END SIGUSR2      

void end(int)
{
    exit(0);
}
void begin(int)
{
    double data[2];
    while (read(0, data, 2 * sizeof(double)) > 0)
        printf("f(%lf) = %lf\n", data[0], data[1]);
    signal(SIGNAL_END, end);
    sigpause(SIGNAL_END);
}
int main()
{
    signal(SIGNAL_BEGIN, begin);
    sigpause(SIGNAL_BEGIN);
    return 0;
} 
