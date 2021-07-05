// EOPSY Lab Task-7: The Return of Dining Philosophers
// Keshav Dandeva [302333] 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 		5					//Number of philosophers
#define LEFT 	( i + N - 1 ) % N 	//Left of philosophers
#define RIGHT	( i + 1 ) % N 		//Right of philosophers

#define THINKING	0 		//State of philosophers
#define HUNGRY		1 
#define EATING		2

#define	up		pthread_mutex_unlock 	//mutex unlock
#define	down	pthread_mutex_lock 		//mutex lock

pthread_mutex_t m; 			//declaring mutex variables
int state[N];	
pthread_mutex_t s[N];
pthread_t philosopher[N];

void grab_forks(int i);		//declaring functions
void put_away_forks(int i);
void test(int i);
void* start_dining(void* num);

int main(int argc, char** argv){

	int i;
	pthread_mutex_init(&m, NULL); 	//initialising mutex m 

	int* num = (int*) malloc(N*sizeof(int)); 	//allocating memory dynamically using malloc()

	for(i = 0; i < N; i++){		//loop for number of philosophers

		state[i]=THINKING;		//setting the state to thinking
		pthread_mutex_init(&s[i], NULL); //initialsing the mutex array
		down(&s[i]); 			//locking the mutex array

		num[i] = i;
		
		if(pthread_create(&philosopher[i], NULL, start_dining, (void*)(num+i))){ //creating pthreads (main part of code)
			printf("ERROR: cannot create pthread");	//returning error if pthread_create does not return 0
			return 1;
		}
	}

	for(i = 0; i < N; i++){
		pthread_join(philosopher[i],NULL); //waiting for the thread to terminate
	}
	
	pthread_mutex_destroy(&m);	//destroying the mutex object m

	for(i = 0; i < N; i++){
		pthread_mutex_destroy(&s[i]);  //destroying the mutex array
	}
	
	pthread_exit(NULL); //terminating the calling thread
	return 0;
}

void grab_forks( int i ){
	
	down(&m);		  //locking the mutex as the philosopher is going to eat and so others cant access
	printf("Philosopher[%d] wants to grab forks to eat.\n", i);
	state[i]=HUNGRY;  //changing state to hungry
	test(i);		  //check if neighbours are not eating
	up(&m); 		  //unlock the mutex
	down(&s[i]); 	  //locking the fork for the philosopher to eat
}

void put_away_forks( int i ){

	down(&m); 			//locking the mutex as the philosopher is going to put away forks
	state[i]=THINKING;  //changing state to thinking
	printf("Philosopher[%d] puts away forks after eating.\n", i); 
	test(LEFT); 		//checking if left neighbour is hungry
	test(RIGHT); 		//checking if right neighbour is hungry
	up(&m); 			//unlocking the mutex so others can access
}

void test(int i) { 		//function to check if the neighbours are eating or not
	
	if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
		
		state[i] = EATING;	//changing to state to eating
		up(&s[i]); 			//unlocking the mutex
	}
}

void* start_dining(void* num){

	int eat_count = 0;			//to keep count of meals of philosophers
	int round_of_meals = 5;		//to set a total number of meals for philosophers
	int* i = (int*)num;

	while(round_of_meals){

		printf("Philosopher[%d] is thinking.\n", *i);
        sleep(3); //sleep during thinking time
        printf("Philosopher[%d] is hungry.\n", *i);
        grab_forks(*i);
        printf("Philosopher[%d] is eating now.\n", *i);
        sleep(1); //sleep during eating time
        put_away_forks(*i);
        eat_count += 1;
        printf("Philosopher[%d] succesfuly ate %d time(s) now.\n", *i, eat_count);
        --round_of_meals;
    }
    return NULL;
}