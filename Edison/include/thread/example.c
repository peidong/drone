/* 
 * WHAT THIS EXAMPLE DOES
 * 
 * We create a pool of 4 threads and then add 40 tasks to the pool(20 task1 
 * functions and 20 task2 functions). task1 and task2 simply print which thread is running them.
 * 
 * As soon as we add the tasks to the pool, the threads will run them. It can happen that 
 * you see a single thread running all the tasks (highly unlikely). It is up the OS to
 * decide which thread will run what. So it is not an error of the thread pool but rather
 * a decision of the OS.
 * 
 * */

#include <stdio.h>
#include "thpool.h"

int globali=0;

void task1(){
	printf("Thread #%u working on task1\n", (int)pthread_self());
    int i;
    for (i=1; i<51; i++){
        printf("task1's i=%i\n", i);
        globali += i;
    }
}

void task2(){
	printf("Thread #%u working on task2\n", (int)pthread_self());
    int i;
    for (i=51; i<101; i++){
        printf("task2's i=%i\n", i);
        globali += i;
    }
}

void task3(){
	printf("Thread #%u working on task3\n", (int)pthread_self());
    int i=1;
    while(1){
        printf("task3's i=%i\n", i);
        globali += i;
        i++;
    }
}
void task4(){
	printf("Thread #%u working on task4\n", (int)pthread_self());
    int i=1;
    while(1){
        printf("task4's i=%i\n", i);
        globali += i;
        i++;
    }
}

int main(){
	
	puts("Making threadpool with 4 threads");
	threadpool thpool = thpool_init(10);

	puts("Adding 40 tasks to threadpool");
    thpool_add_work(thpool, (void*)task3, NULL);
    thpool_add_work(thpool, (void*)task4, NULL);
    thpool_wait(thpool);
    printf("\n\nGlobal i = %i\n", globali);

    puts("Killing threadpool");
    thpool_destroy(thpool);
	
	return 0;
}
