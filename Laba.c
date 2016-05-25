//1->2   2->(3,4,5)   4->5    3->6  6->7

//1->(2,3,4) SIGUSR1   2->5,6 SIGUSR2   6->7 SIGUSR1
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
#include <limits.h>


pid_t pidA[9];
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

int setpgidproc(char * name,int child,int parent);

int setsigaction(int sig, void (*handler) (int,siginfo_t *, void *));
int setsigactionIGN(int);
int createFile(char* name);
int accessFile(char* name,int pid, int i);
int main(int argc, char* argv[])
{
   int t = 0;
   char path_res[PATH_MAX] = "./pids.txt";
   if(createFile(path_res)==-1){
	   return 1;
   }

   progname = strrchr(argv[0], '/') + 1;
   pidA[0] = getpid();
   if(setsigactionIGN(SIGUSR1) == -1)
   {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
   }
   accessFile(path_res,getpid(),0);
   pid_t pid;

    if((pid = fork()) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }    
    else if(pid == 0)
    {
        //process 1
    	//printf("1 created");
        pidA[1] = getpid();
        if(setsigaction(SIGUSR2,proc1usrhandler) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
        accessFile(path_res,getpid(),1);
        
        if((pid = fork()) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }    
        else if(pid == 0)
        {
            //process 2
        	//printf("2 created");
            pidA[2] = getpid();
            accessFile(path_res,getpid(),2);
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
                //proc 3
                pidA[3] = getpid();
               // printf("3 created");
                accessFile(path_res,getpid(),3);
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
                sleep(1);
                if((pid = fork()) == -1)
                {
                    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    exit(1);
                }    
                else if(pid == 0)
                {
                    //proc 6
                	//printf("6 created");
                	accessFile(path_res,getpid(),6);
                    if(setsigaction(SIGUSR2,proc6usrhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    if(setsigaction(SIGTERM,proc6termhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
		    setpgidproc("./pids.txt",6,5);
                    if((pid = fork()) == -1)
                    {
                         fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                         exit(1);
                    }
                    else if(pid == 0){
                    	//proc 7
                    	//printf("7 created");
                    	pidA[7] = getpid();
                    	accessFile(path_res,getpid(),7);
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
            setpgidproc("./pids.txt",7,7);
                    	//proc 8
                    	if((pid = fork()) == -1)
                    	{
                               fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                    	       exit(1);
                    	}
                    	else if(pid == 0){
                    	// 8
                    		//printf("8 created");
                    		pidA[8] = getpid();
                    		accessFile(path_res,getpid(),8);
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
                                setpgidproc("./pids.txt",8,7);
                        	while(1);
                    	}//8

                    	pidA[8]=pid;//from 7
                    	while(1);
                    }//7
                    pidA[7]=pid;//from 6

                    while(1);
                }//6
                pidA[6]=pid;//from 3

					while(1);
            }//3

            pidA[3]=pid;//from 2


            if((pid = fork()) == -1)
            {
                fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                exit(1);
            }    
            else if(pid == 0)
            {
                //proc 4
               
            	//printf("4 created");
                pidA[4] = getpid();
                accessFile(path_res,getpid(),4);

                if(setsigaction(SIGUSR1,proc4usrhandler) == -1)
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
                    //proc 5
                	//printf("5 created");
                    pidA[5] = getpid();
                    accessFile(path_res,getpid(),5);
                
                    if(setsigaction(SIGUSR2,proc5usrhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                    if(setsigaction(SIGTERM,proc5termhandler) == -1)
                    {
                        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
                        exit(1);
                    }
                     setpgidproc("./pids.txt",5,5);
                    while(1);
                }//5
                pidA[5]=pid;//from 4


                while(1);
            }//4
            pidA[4]=pid;//from 2

            sleep(4);
            setpgidproc("./pids.txt",2,2);
            setpgidproc("./pids.txt",3,2);
            setpgidproc("./pids.txt",4,2);

            
            while(1);
        }//2

       pidA[2]=pid;//from 1

       while(1);
    }//1c



            

    else{
    	pidA[1]=pid;//from 0
        //proc 0

        char c;

        scanf("%c",&c);

        if(kill(pidA[1],SIGUSR2) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
        if(wait(NULL) == -1)
        {
            fprintf(stderr, "%s: wait %s\n", progname, strerror(errno));
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
   
    printf("1 %d %d получил USR2 от %d", getpid(),getppid(), inf->si_pid);
    printtime();
    usr2count++;
    if(usr2count == 102)
    {
        if(kill(getpidfile("./pids.txt",2),SIGTERM) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        } 
        if(waitpid(getpidfile("./pids.txt",2) , NULL, 0) == -1)
        {
            fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
        exit(0);
    }
    else
    {
        printf("1 %d %d послал USR1", getpidfile("./pids.txt",1),getpidfile("./pids.txt",0));
        printtime();
        if(kill(-getpidfile("./pids.txt",2),SIGUSR1) == -1)
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
        printf("2 %d %d послал USR2", getpid(),getppid());
        printtime();
	if(kill(-getpidfile("./pids.txt",5),SIGUSR2) == -1)
	{
	    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
            exit(1);
        }
	usr2count++;
}
void proc3usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("3 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();

}
void proc4usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("4 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();

}
void proc5usrhandler(int sig_no,siginfo_t *inf, void *a)
{
	printf("5 %d %d получил USR2 от %d", getpid(), getppid(), inf->si_pid);
	printtime();

}
void proc6usrhandler(int sig_no,siginfo_t *inf, void *a)
{
	
	printf("6 %d %d получил USR2 от %d", getpid(), getppid(), inf->si_pid);
        printtime();
	printf("6 %d %d послал USR1", getpid(), getppid());
	printtime();
	if(kill(getpidfile("./pids.txt",7),SIGUSR1) == -1)
	{
	    fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	    exit(1);
	}
	usr1count++;
}
void proc7usrhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("7 %d %d получил USR1 от %d", getpid(), getppid(), inf->si_pid);
    printtime();
    printf("7 %d %d послал USR1", getpid(), getppid());
    printtime();
    if(kill(getpidfile("./pids.txt",8),SIGUSR1) == -1)
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
    if(kill(getpidfile("./pids.txt",1),SIGUSR2) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    usr2count++;
}
void proc2termhandler(int sig_no,siginfo_t *inf, void *a)
{
    int i;
    for(i = 3; i < 5; i++)//3,4
    {
        if(kill(getpidfile("./pids.txt",i),SIGTERM) == -1)
        {
            fprintf(stderr, "%s: %s", progname, strerror(errno));
            exit(1);
        } 
        if(waitpid(getpidfile("./pids.txt",i) , NULL, 0) == -1)
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
    if(kill(getpidfile("./pids.txt",6),SIGTERM) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    } 
    if(waitpid(getpidfile("./pids.txt",6) , NULL, 0) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
void proc4termhandler(int sig_no,siginfo_t *inf, void *a)
{
    if(kill(getpidfile("./pids.txt",5),SIGTERM) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    } 
    if(waitpid(getpidfile("./pids.txt",5) , NULL, 0) == -1)
    {
        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
        exit(1);
    }
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
void proc5termhandler(int sig_no,siginfo_t *inf, void *a)
{
	 printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
	 exit(0);
}

void proc6termhandler(int sig_no,siginfo_t *inf, void *a)
{
	if(kill(getpidfile("./pids.txt",7),SIGTERM) == -1)
	    {
	        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	        exit(1);
	    }
	    if(waitpid(getpidfile("./pids.txt",7) , NULL, 0) == -1)
	    {
	        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	        exit(1);
	    }
	    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
	    exit(0);
}
void proc7termhandler(int sig_no,siginfo_t *inf, void *a)
{
	if(kill(getpidfile("./pids.txt",8),SIGTERM) == -1)
	    {
	        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	        exit(1);
	    }
	    if(waitpid(getpidfile("./pids.txt",8) , NULL, 0) == -1)
	    {
	        fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	        exit(1);
	    }
	    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
	    exit(0);
}
void proc8termhandler(int sig_no,siginfo_t *inf, void *a)
{
    printf("%d %d завершил работу после %d сигнала SIGUSR1 и %d сигнала SIGUSR2\n", getpid(), getppid(), usr1count, usr2count);
    exit(0);
}
int createFile(char *name){
	int t=0;
	 FILE* f =  fopen(name, "w+");//truncated
	 /*for(t=0;t<8;++t){
		 int offset = fseek(f,t*2,SEEK_SET);
		 long i = ftell(f);
		 char str[16];
		 int wrote = sprintf(str,"%d\r\n",0);

		 int wr = fwrite(str,sizeof(int),1,f);
	 }*/
	 fseek(f,0,SEEK_SET);

	 if ( fflush (f) == EOF)
	 {
	       fprintf(stderr, "%s: close flush error %s \n", progname, strerror(errno) );
	       return -1;
	 }
	   if ( fclose (f) == EOF)
	   {
	           fprintf(stderr, "%s: close flush error %s  \n", progname, strerror(errno));
	           return -1;
	   }
	   return 0;
}
int accessFile(char *name,int pid,int i){

	FILE* f =  fopen(name, "r+");//truncated
	int shift = fseek(f,0L,SEEK_SET);
	char str[16];
	sprintf(str,"%d\r\n",pid);//-%d,i
	int size = sizeof(str);

	shift = fseek(f,10*i,SEEK_SET);
	shift = ftell(f);
	shift = fputs(str,f);




	if ( fflush (f) == EOF)
	{
	        fprintf(stderr, "%s: close flush error %s  \n", progname, strerror(errno) );
	        return -1;
	}
	printf("Num-%d get-%d file-%d\n",i, getpid(), getpidfile("./pids.txt",i));
	if ( fclose (f) == EOF)
		{
		        fprintf(stderr, "%s: close flush error %s  \n", progname, strerror(errno) );
		        return -1;
		}
	return 0;
}
int setpgidproc(char *name,int c,int p){
	FILE* f =  fopen(name, "r");//truncated
	int shift;
	char str[16];
	shift = fseek(f,0L,SEEK_SET);
	shift = fseek(f,10*c,SEEK_SET);
	shift = fgets(str,10,f);
	/*int k=0;
	for(;k<c;++k){
		shift = fgets(str,16,f);
	}*/
	int cpid = atoi(str);
	printf("process - %d; pid - %d\r\n",c,cpid);

	shift = fseek(f,0L,SEEK_SET);

	shift = fseek(f,10*p,SEEK_SET);
	shift = fgets(str,10,f);
	/*k=0;
	for(;k<p;++k){
		shift = fgets(str,16,f);
	}*/
	int ppid = atoi(str);
	printf("process - %d; ppid - %d\r\n",p,ppid);
	if ( fclose (f) == EOF)
	{
	        fprintf(stderr, "%s: close flush error %s  \n", progname, strerror(errno) );
	        return -1;
	}
	if(setpgid(cpid,ppid)==-1)	{
		fprintf(stderr, "%s: %s\n", progname, strerror(errno));
	    exit(1);
	}
	return 0;

}
pid_t getpidfile(char *name,int i){
	FILE* f =  fopen(name, "r");//truncated
	int shift;
	char str[16];
	shift = fseek(f,0L,SEEK_SET);
	shift = fseek(f,10*i,SEEK_SET);
	shift = fgets(str,10,f);
	/*int k=0;
	for(;k<c;++k){
		shift = fgets(str,16,f);
	}*/
	int pid = atoi(str);
	return pid;
}

