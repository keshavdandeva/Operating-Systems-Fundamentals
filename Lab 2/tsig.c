// EOPSY Lab Task-2
// - Keshav Dandeva [302333] 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_CHILD 5
#define WITH_SIGNALS	// For Section-3 implemntation, use #undef to run without it

void Create_ChildProcess(int i); 	//Function to create child processes
int Counter_ChildProcess = 0;	 	//Counter to keep track of created processes
pid_t ChildProcess_IDs[NUM_CHILD];  //Array of processes pid
 
#ifdef WITH_SIGNALS
void Handle_Sigint(int signal);
bool KeyboardInterruption = false;
void SigTerm_Signal();
void Terminate_ChildProcess(int signal);
#endif

int main(int argc, char const *argv[]){
	
	#ifdef WITH_SIGNALS
	for (int i = 0; i < _NSIG; ++i){
		signal(i, SIG_IGN);
	}
	signal(SIGCHLD, SIG_DFL);
	signal(SIGINT, Handle_Sigint);
	#endif	

	printf("Parent Process[%d] is created successfully.\n", getpid());

	for (int i = 0; i < NUM_CHILD; ++i){
		Create_ChildProcess(i);
		sleep(1);

		#ifdef WITH_SIGNALS
		if (KeyboardInterruption){
			printf("Parent Process[%d] detected keyboard interruption during Child Process creation \n", getpid());
			SigTerm_Signal();
			break;
		}
		#endif
	}

	printf("Parent Process[%d] has no more child processes.\n", getpid());
	printf("%d Child Processes created successfully.\n", Counter_ChildProcess);

	int Counter_ChildProcessTerminations = 0;
	int Wait_ReturnValue = 0;
	while(true){
		Wait_ReturnValue = wait(NULL);

		if (Wait_ReturnValue == -1)
			break;
		else
			Counter_ChildProcessTerminations++;
	}

	printf("%d Child Processes executed successfully.\n", Counter_ChildProcessTerminations);

	#ifdef WITH_SIGNALS
	for (int i = 0; i < _NSIG; ++i){
		signal(i, SIG_DFL);
	}
	#endif

	return 0;
}

void Create_ChildProcess(int i){

	int ForkValue = fork();

	if (ForkValue < 0){
		printf("Child Process[%d] could not be created!\n", getpid());
		#ifdef WITH_SIGNALS
		SigTerm_Signal();
		#endif
		exit(1);
	}

	else if (ForkValue == 0){

		#ifdef WITH_SIGNALS
		signal(SIGINT, SIG_IGN);
		signal(SIGTERM, Terminate_ChildProcess);
		#endif		

		printf("Child Process[%d] created succesfully from Parent Process[%d].\n", getpid(), getppid());
		sleep(10);
		printf("Child Process[%d] terminated succesfully.\n", getpid());
		exit(0);
	}
	else{
		ChildProcess_IDs[i] = ForkValue;
		Counter_ChildProcess++;
	}

}

#ifdef WITH_SIGNALS

void Handle_Sigint(int signal){
	
	printf("Parent Process[%d] has detected keyboard interruption.\n", getpid());
	KeyboardInterruption = true;	
}

void SigTerm_Signal(){

	for (int i = 0; i < NUM_CHILD; ++i){
		if (ChildProcess_IDs[i] != 0){
			printf("Parent Process[%d] is signalling SIGTERM to the Child Process[%d]\n", getpid(), ChildProcess_IDs[i]);
			kill(ChildProcess_IDs[i], SIGTERM);
		}
	}
}

void Terminate_ChildProcess(int signal){

	printf("Child Process[%d] is terminated.\n", getpid());
	exit(1);
}

#endif