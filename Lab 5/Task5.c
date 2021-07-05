// EOPSY Lab Task-5: Dining Philosophers
// Keshav Dandeva [302333] 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void start_dining();

int Philosopher_Nr; 
int SemaphoreSet_ID;

int main(int argc, char const *argv[]) {

	SemaphoreSet_ID = semget(1, 5, 0644 | IPC_CREAT); 		//Creating a new set. Setting key=1, np of sems=5, semflg = 644 (rw- r-- r--) | 
															//IPC_CREAT (to create a new memory segment for shared memory)
	if(SemaphoreSet_ID == -1) {
		perror("Allocation Error: Semaphores\n");
		exit(1);
	}

	int i = 0;

	while(i <= 4){											//Initiliasing semaphores
		semctl(SemaphoreSet_ID, i++, SETVAL, 1);			
	}

	i = 0;
	int ForkValue;

	while(i <= 4) {											//Creating processes
		ForkValue = fork();
		if(ForkValue == 0) {
			Philosopher_Nr = i;
			start_dining();
			return 0;
		}
		else if(ForkValue < 0) {							//If process couldn't be created, terminating it
			kill(-2,SIGTERM);
			perror("Process Error: Process could not be created\n");
			exit(1);
		}
		++i;
	}

	while(1) {
		int wstatus;
		ForkValue = wait(&wstatus);							//Waiting till the process terminates
		if(ForkValue < 0)
			break;
	}

	i=0;

	if (semctl (SemaphoreSet_ID, 0, IPC_RMID, 1)<0)			//Removing the semaphore set if deallocation error i.e. negative value return by semctl
		printf("Deallocation Error: Semaphores\n");

	return 0;
}

void grab_forks(int left_fork_id) {
	
	int right_fork_id = left_fork_id-1;				//Giving right fork id 1 less than left fork or 4 if left fork id is 0
	if(right_fork_id == -1) {
		right_fork_id = 4;
	}

	printf("Philosopher[%d] wants to pick up: Left Fork[%d] and Right Fork[%d]\n", Philosopher_Nr,left_fork_id, right_fork_id);
	
	struct sembuf sb[2] = {							//creating semaphore buffer struct 
		{right_fork_id,-1,0}, {left_fork_id,-1,0}	//semaphore nr, sem_op < 0: decreasing sem by -1, sem flag = 0 (no additional flags)  
	};

	semop(SemaphoreSet_ID, sb, 2);					//performing operation on 2 semaphores
}

void put_away_forks(int left_fork_id) {

	int right_fork_id = left_fork_id-1;				//Giving right fork id 1 less than left fork or 4 if left fork id is 0
	if(right_fork_id<0) {
		right_fork_id=4;
	}

	printf("Philosopher[%d] puts away: Left Fork[%d] and Right Fork[%d]\n", Philosopher_Nr, left_fork_id, right_fork_id);

	struct sembuf sb[2] = {							//creating semaphore buffer struct 													
		{right_fork_id,1,0}, {left_fork_id,1,0}		//semaphore nr, sem_op > 0: increasing sem by 1, sem flag = 0 (no additional flags)
	};

	semop(SemaphoreSet_ID, sb, 2);					//performing operation on 2 semaphores
}

void start_dining() {

	int eat_count = 0;			//Counter for every meal for each philosopher
	bool hungry = false;		
	int round_of_meals = 5;		//To set total no of meals for each philosopher

	while(round_of_meals) {
		if(hungry) {

			printf("Philosopher[%d] is hungry.\n", Philosopher_Nr);
			grab_forks(Philosopher_Nr);
			printf("Philosopher[%d] is eating now.\n", Philosopher_Nr);
			sleep(1);
			eat_count += 1;
			printf("Philosopher[%d] successfully ate %d time(s).\n",Philosopher_Nr,eat_count);
			put_away_forks(Philosopher_Nr);
			hungry = false;
			--round_of_meals;
		}
		else {

			printf("Philosopher[%d] is thinking.\n", Philosopher_Nr);
			sleep(2);
			hungry = true;
		}
	}
}
