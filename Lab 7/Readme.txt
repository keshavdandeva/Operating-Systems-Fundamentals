To compile the file use command:
gcc -pthread -o Task7 Task7.c

To run:
./Task7

---------------------------------

Questions and Answers:

Ques 1.	Would it be sufficient just to add to the old algorithm from task5
	additional mutex variable to organize critical sections in functions
	grab_forks() and put_away_forks() for making changes to values of two mutexes
	indivisably?  If not, why?

Answer: It would not be sufficient to just add mutex variable to the algorithm of task5 as
	the mutex cannot change more than one mutex value per one call. So if a philosopher 
	enters mutex and tries to grab the forks, and can't since someone did it already,
	then another philosopher wants to put away the forks being used, but can't, since 
	the first philosopher is in the mutex. 
	In task5 algorithm, using semaphores, we could block two forks at the same time and 
	thus prevent this deadlock.

Ques 2. Why m mutex is initialized with 1 and mutexes from the array s are
	initialized with 0's?
	Note: this is in "semaphore logic", so 1 is "open" and 0 is "closed".

Answer: The mutex m is used for philosopher's state. It is initialised to 1 as to be accessible
	and active from the beginning. It is used for philosophers attempt to grab forks and put
	away forks (basically to enter the critical section).
	The mutexes from the array s are initialised with 0's as it represents the shared memory.
	These mutexes would only be unlocked at appropriate time preventing deadlock.