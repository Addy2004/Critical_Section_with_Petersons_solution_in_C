#include <stdio.h>
#include <threads.h>
#include <stdbool.h>

// ANSI MACROS
#define AC_RED "\x1b[31m"   // to print in red color
#define AC_GREEN "\x1b[32m" // to print in green color
#define AC_YELLOW "\x1b[33m" // to print in yellow color
#define AC_NORMAL "\x1b[m"  // to print normally

int turn;     // whose turn to enter critical section
bool flag[2]; // if process is ready to enter critcial section

mutex_lock(int self) {
	flag[self] = 1;   // Process wants to acquire the lock

	turn = 1 - self;  // Give other process a chance to acquire the lock 

	// if other process is in critical section
	while ((flag[1 - self] == 1) && (turn == 1 - self)) {
		; // then do nothing
	}
}

mutex_unlock(int self) {
	flag[self] = 0; // Process wants to leave the critical section
}

void* cs(void* s) {

	int self = (int*)s; // Pointer to Thread ID
	
	// Entry section
	mutex_lock(self);
	printf("%sProcess %d entering the critical section\n\n", AC_YELLOW, self);
	Sleep(3000);

	// Critical section
	printf("%sProcess %d is inside the critical section\n\n", AC_RED, self);
	Sleep(7000);

	// Exit section
	printf("%sProcess %d exiting the critical section\n\n", AC_GREEN, self);
	mutex_unlock(self);

	// Remainder section
	// <other cleanup code here>
}

int main() {
	Sleep(2000);
	printf("Critical Section Problem with Peterson's solution\n");
	Sleep(2000);
	printf("Demonstration will start shortly....\n\n");
	Sleep(3000);

	// define two processes (as threads)
	thrd_t p1, p2;

	// set flags and turn to zero for both processes
	flag[0] = flag[1] = 0;
	turn = 0;

	// let them both try to enter the critical section in cs
	thrd_create(&p1, cs, (void*)0);
	thrd_create(&p2, cs, (void*)1);

	// Wait for the threads to end
	int res;
	thrd_join(p1, &res);
	thrd_join(p2, &res);

	printf("%s\n", AC_NORMAL);
	return 0;
}