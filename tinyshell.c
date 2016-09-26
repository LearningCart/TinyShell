/*
	Jatin Gandhi:
	Tiny Shell (Quick & Dirty)

How to compile?
command to compile:
gcc -lpthread shell.c -o myshell

[<...>]gcc -lpthread shell.c -o myshell

this will generate myshell executable elf on linux environment
Example Output:
[<...>]$ ./myshell 
testshell># ?
Supported commands : help, ?, -?, --?, test1, test2, test3, exit, quit, 
testshell># test1
test1(): 60
testshell># exit
Thank you for using the shell
[<...>]$ 

*/
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <execinfo.h>
#include <sys/prctl.h>


void *command_receiver(void *p);

int totalcommand_buffer = 0;

typedef int (*commandfunc)(void);
typedef struct _command
{
		char *name;
		commandfunc execute;
}command;
#define SHELL_PROMPT	"testshell># "

int help();
int test2();
int test3();
int exitfunc();

int command1()
{
	printf("%s():%d\n",__func__,__LINE__);
}


command command_table[] =
{
		{"help",help},
		{"?",help},
		{"-?",help},
		{"--?",help},
		{"cmd1",command1},
		{"test",test3},
		{"exit",exitfunc},
		{"quit",exitfunc}
};

#define TOTAL_COMMADS	(sizeof(command_table)/sizeof(command_table[0]))

static char command_buffer[255];

static void signal_handler(int sig);

void set_call_trace (void)
{
	struct sigaction s;
	
	sigemptyset(&s.sa_mask);
	
	s.sa_flags     = 0;
	s.sa_handler = signal_handler;
	
	sigaction(SIGSEGV,&s,NULL);
	sigaction(SIGINT,&s,NULL);
	
	return;
}
#include <assert.h>

int main(int argc, char **argv)
{
	pthread_t tid;

	set_call_trace();
	prctl(PR_SET_NAME,"Main Thread",0,0,0);
	if(0 != pthread_create(&tid,NULL,command_receiver, NULL))
	{
		return -1;
	}

	pthread_join( tid, NULL);

	return 0;
}


int test2(void)
{
	printf("%s(): %d\n",__func__,__LINE__);
	return 0;
}
int test3(void)
{
	printf("%s(): %d\n",__func__,__LINE__);
	return 0;
}
int help(void)
{
	int i;
	printf("Supported commands : ");
	for(i = 0; i < TOTAL_COMMADS; i++)
		printf("%s, ", command_table[i].name);

	printf("\n");
	
	return 0;
}

int exitfunc(void)
{
	printf("Shell is over\n");
	pthread_exit(NULL);
}

void process_command(const char *command_bufferstr)
{
	int i;

	if(NULL == command_bufferstr) return;

	totalcommand_buffer++;
		
	for(i = 0; i < TOTAL_COMMADS; i++)
	{
		if(strcmp(command_table[i].name, command_bufferstr) == 0)
		{
			if(NULL != command_table[i].execute)
				command_table[i].execute();
			break;
		}
	}

	if(TOTAL_COMMADS == i)
	{
		printf("Command %s is not supported yet...\n",command_bufferstr);
		help();
	}
	return;
}

void *command_receiver(void *p)
{

	
	prctl(PR_SET_NAME,"command_receiver",0,0,0);

	while(1)
	{
		memset(command_buffer,0,255);
		printf(SHELL_PROMPT);
		scanf("%s",command_buffer);
		process_command(command_buffer);
	}
	return NULL;
}

/* backtrace info */
static void signal_handler(int sig)
{

    char *signal_string;

    void *trace[32];
    size_t size, i;
    char **strings;
  
	signal_string = NULL;

	signal_string = strsignal(sig);
    fprintf( stderr, "\n SIGNAL : %s \n\n",signal_string );
    size    = backtrace( trace, 32 );
    strings = backtrace_symbols( trace, size );
  
    fprintf( stderr, "\nBACKTRACE:\n\n" );
  
    for( i = 0; i < size; i++ ){
  	  fprintf( stderr, "  %s\n", strings[i] );
    }
  
    fprintf( stderr, "\n\n" );
  
    exit(-1);
}
