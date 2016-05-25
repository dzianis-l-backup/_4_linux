//1->2   2->(3,4,5)   4->6    3->7  5->8

//1->(2,3) SIGUSR1   3->4 SIGUSR2   4->(5,6,7) SIGUSR1   
//7->8 SIGUSR1   8->1 SIGUSR2


#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

pid_t procids[9];
int usr1count = 0;
int usr2count = 0;
char* progname;

void proc1usrhandler(int,siginfo_t *, void *);
void proc2usrhandler(int,siginfo_t *, void *);
void proc3usrhandler(int,siginfo_t *, void *);
void proc4usrhandler(int,siginfo_t *, void *);
void proc5usrhandler(int,siginfo_t *, void *);
void proc6usrhandler(int,siginfo_t *, void *);
void proc7usrhandler(int,siginfo_t *, void *);
void proc8usrhandler(int,siginfo_t *, void *);

void proc2termhandler(int,siginfo_t *, void *);
void proc3termhandler(int,siginfo_t *, void *);
void proc4termhandler(int,siginfo_t *, void *);
void proc5termhandler(int,siginfo_t *, void *);
void proc6termhandler(int,siginfo_t *, void *);
void proc7termhandler(int,siginfo_t *, void *);
void proc8termhandler(int,siginfo_t *, void *);
void printtime();

int setsigaction(int sig, void (*handler) (int,siginfo_t *, void *));
int setsigactionIGN(int);

int main(int argc, char* argv[])
{
    progname = strrchr(argv[0], '/') + 1;    


    procids[0] = getpid();
    if(setsigactionIGN(SIGUSR1) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    pid_t pid; 
    if((pid = fork()) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }    
    else if(pid == 0)
    {
        //proc 1
        procids[1] = getpid();
        
        if(setpgid( procids[1], procids[1]) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
        
        if(setsigaction(SIGUSR2,proc1usrhandler) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
        
        if((pid = fork()) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }    
        else if(pid == 0)
        {
            //proc 2
            procids[2] = getpid();
          
            if(setsigaction(SIGUSR1,proc2usrhandler) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }
            if(setsigaction(SIGTERM,proc2termhandler) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }
            
            if((pid = fork()) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }  
            else if(pid == 0)
            {
                //proc 5
                procids[5] = getpid();
               
                if(setsigaction(SIGUSR1,proc5usrhandler) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }
                if(setsigaction(SIGTERM,proc5termhandler) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }
                
                if((pid = fork()) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }    
                else if(pid == 0)
                {
                    //proc 8  
                    procids[8] = getpid();
                    if(setpgid( procids[8], procids[0]) == -1)
                    {
                        fprintf(stderr, "%s: %s %d %d \n", progname, strerror(errno),procids[0], procids[8]);
                        exit(1);
                    }
                    if(setsigaction(SIGUSR1,proc8usrhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    if(setsigaction(SIGTERM,proc8termhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    
                    while(1);
                }
                procids[8] = pid;  
                while(1);
            }
            procids[5] = pid;
            
            //group 2
            if(setpgid( procids[5], procids[5]) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }
            
            if((pid = fork()) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }    
            else if(pid == 0)
            {
                //proc 4
               
                if(setpgid( procids[4], procids[4]) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }
                procids[4] = getpid();
                if(setsigaction(SIGUSR2,proc4usrhandler) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }
                if(setsigaction(SIGTERM,proc4termhandler) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }
                if((pid = fork()) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }    
                else if(pid == 0)
                {
                    //proc 6
                    procids[6] = getpid();
                   
                    if(setsigaction(SIGUSR1,proc6usrhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    if(setsigaction(SIGTERM,proc6termhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    //group 2
                    if(setpgid( procids[6], procids[5]) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                       
                    
                    while(1);
                }
                procids[6] = pid;     
                while(1);
            }
            procids[4] = pid;
  
            if((pid = fork()) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }    
            else if(pid == 0)
            {
                //proc 3
                procids[3] = getpid();
                
                if(setsigaction(SIGUSR1,proc3usrhandler) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }
                if(setsigaction(SIGTERM,proc3termhandler) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }
                if((pid = fork()) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }    
                else if(pid == 0)
                {
                    //proc 7
                    procids[7] = getpid();
                    
                    if(setsigaction(SIGUSR1,proc7usrhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    if(setsigaction(SIGTERM,proc7termhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                               
                    //group 2
                    if(setpgid( procids[7], procids[5]) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    procids[8] = -procids[0];
                    while(1);
                }
                procids[7] = pid;
                while(1);
            }
            procids[3] = pid;
           
            //groupe 1
            if(setpgid( procids[2], procids[2]) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }
            if(setpgid( procids[3], procids[2]) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }
            while(1);
            
        }
        //proc 1
        procids[2] = pid;
        
        while(1);
    }
    else
    {
        //proc 0
        procids[1] = pid;
        char c;
        scanf("%c",&c);
        if(kill(procids[1],SIGUSR2) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
        if(wait(NULL) == -1)
        {
            fprintf(stderr, "%s: waite %s\n", progname, strerror(errno));
            exit(1);
        }
        return 0;
    }
}

int setsigaction(int sig, void (*handler) (int,siginfo_t *, void *))
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    return sigaction(sig,&act,NULL);
}

int setsigactionIGN(int sig)
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    return sigaction(sig,&act,NULL);
}

void printtime()
{
    struct timeval mt;
    gettimeofday(&mt,NULL);
    struct tm* t;
    time_t ptime;
    ptime= time(NULL);
    t =gmtime(&ptime);
    printf("	%ld \n", mt.tv_usec/1000);
}

void proc1usrhandler(int sig_no,siginfo_t *inf, void *a)
{
   
    printf("1 %d %d получил USR2 от %d", procids[1],procids[0], inf->si_pid);
    printtime();
    usr2count++;
    if(usr2count == 102)
    {
        if(kill(procids[2],SIGTERM) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        } 
        if(waitpid(procids[2] , NULL, 0) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
        exit(0);
    }
    else
    {
        printf("1 %d %d послал USR1", procids[1],procids[0]);
        printtime();
        if(kill(-procids[2],SIGUSR1) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
    }
    usr1count++;
}
void proc2usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("2 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();
}
void proc3usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("3 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();
    printf("3 %d %d послал USR2", getpid(), getppid());
    printtime();
    if(kill(procids[4],SIGUSR2) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    usr2count++;
}
void proc4usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("4 %d %d получил USR2 от %d", getpid(), getppid(), inf->si_pid);
    printtime();
    printf("4 %d %d послал USR1", getpid(), getppid());
    printtime();
    if(kill(-procids[5],SIGUSR1) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    usr1count++;
}
void proc5usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("5 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();
}
void proc6usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("6 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();
}
void proc7usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("7 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();
    printf("7 %d %d послал USR1", getpid(), getppid());
    printtime();
    if(kill(procids[8],SIGUSR1) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    usr1count++;
}
void proc8usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("8 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();   
    printf("8 %d %d послал USR2", getpid(), getppid());
    printtime();
    if(kill(procids[1],SIGUSR2) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    usr2count++;
}
void proc2termhandler(int sig_no,siginfo_t *inf, void *a)
{
    int i;
    for(i = 3; i < 6; i++)
    {
        if(kill(procids[i],SIGTERM) == -1)
        {
            fprintf(stderr, "%s: %s", progname, strerror(errno));
            exit(1);
        } 
        if(waitpid(procids[i] , NULL, 0) == -1)
        {
            fprintf(stderr, "%s: %s", progname, strerror(errno));
            exit(1);
        }
    }
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
void proc3termhandler(int sig_no,siginfo_t *inf, void *a)
{
    if(kill(procids[7],SIGTERM) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    } 
    if(waitpid(procids[7] , NULL, 0) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
void proc4termhandler(int sig_no,siginfo_t *inf, void *a)
{
    if(kill(procids[6],SIGTERM) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    } 
    if(waitpid(procids[6] , NULL, 0) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
void proc5termhandler(int sig_no,siginfo_t *inf, void *a)
{
    if(kill(procids[8],SIGTERM) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    } 
    if(waitpid(procids[8] , NULL, 0) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}

void proc6termhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
void proc7termhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
void proc8termhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}

