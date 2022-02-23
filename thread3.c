#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>
#include <stdlib.h>
/* compile with gcc -ofname thread-template.c -lpthread */
/* Solve with mutex locks */

struct rusage mytime;

struct shared_count
{
     int val;     /* shared variable to store result*/
}*counter;

int getpid();
int temp;
sem_t mutex;
int jump=0;

/****************************************************************
* This function increases the val of shared variable "counter"
  by one 3000000 times
****************************************************************/
void * thrd1(void *arg)
{
	int itr = 0;	
	int c1 = 0;
	for (itr;itr < 3000000;)
    {
      
           
             sem_wait(&mutex);       //semaphore lock
             itr++;
            c1++;
            counter->val = counter->val + 1;
    	    if( counter->val % 100 == 0){
    	        jump++;
    	        c1 += 100;
    	        counter->val += 100;
    	        itr += 100;
    	        
    	    } 
    	    if(itr > 3000000){
    	            c1 = c1 - (itr - 3000000);
    	            counter->val = counter->val - (itr - 3000000);
    	        }
    	    sem_post(&mutex);        //semaphore unlock
           
    

    }
	printf("thread1 counter  =  %d, shared counter = %d, No of jumps = %d \n", counter->val, c1 ,jump); 
    return(NULL);
}


/****************************************************************
This function increases the val of shared variable "counter"
by one 3000000 times
****************************************************************/
void * thrd2(void *arg)
{
	int itr = 0;	
        int count;
        int c2 = 0;
	for (itr;itr < 3000000;)
	   {
            

	    sem_wait(&mutex);                  //semaphore lock
        itr++;
        c2++;
        counter->val = counter->val + 1;
        sem_post(&mutex);                 //semaphore unlock
        }
      
	printf("thread2 counter = %d, shared counter = %d \n", counter->val, c2); 
    return(NULL);
}

/****************************************************************
This function increases the val of shared variable "counter"
by one 3000000 times
****************************************************************/
void * thrd3(void *arg)
{
	int itr = 0;	
        int count;
        int c3 = 0;
	for (itr;itr < 3000000;)
	   {
            

	    sem_wait(&mutex);                  //semaphore lock
        itr++;
        c3++;
        counter->val = counter->val + 1;
        sem_post(&mutex);                 //semaphore unlock
        } 
      
	printf("thread3 counter = %d, shared counter = %d \n", counter->val, c3); 
    return(NULL);
}

/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main()
{
        int         r=0;
        pthread_t	t1[1];     /* process id for thread 1 */
        pthread_t	t2[1];      /* process id for thread 2 */
        pthread_t	t3[1];      /* process id for thread 3 */
        pthread_attr_t	attr[1];     /* attribute pointer array */

	    ////semaphore initialize
	    sem_init(&mutex, 0, 1);   
         counter = (struct shared_count *) malloc(sizeof(struct shared_count));

	/* initialize shared memory to 0 */
	counter->val = 0 ;
           printf("1 - I am here %d in pid %d\n",r,getpid());
    
	fflush(stdout);
    /* Required to schedule thread independently.
    Otherwise use NULL in place of attr. */
        pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);  /* system-wide contention */ 

	/* end to schedule thread independently */

    /* Create the threads */

    pthread_create(&t2[0], &attr[0], thrd2, NULL);
    pthread_create(&t1[0], &attr[0], thrd1, NULL);
    pthread_create(&t3[0], &attr[0], thrd3, NULL);


    /* Wait for the threads to finish */
    pthread_join(t2[0], NULL);
    pthread_join(t1[0], NULL);
     pthread_join(t3[0], NULL);

            sem_destroy(&mutex);          //Destroy semaphore

	        printf("from parent counter  =  %d\n", counter->val);
    getrusage(RUSAGE_SELF, &mytime);
    printf("Time used is sec: %d, usec %d\n",mytime.ru_utime.tv_sec,
           mytime.ru_utime.tv_usec);
    printf("System Time used is sec: %d, usec %d\n",mytime.ru_stime.tv_sec,
           mytime.ru_stime.tv_usec);
		printf("---------------------------------------------------------------------------\n");
		printf("\t\t	End of simulation\n");

		exit(0);
		
}

