/**xsh_cycExe.c by Tejas Vyas for CSE 321 Project 2. Credits for the implementation to Bina Ramamurthy and Sergey Cherny */

#include <stddef.h>
#include <kernel.h>
#include <stdio.h>
#include <ctype.h>
//#include <unistd.h>
//#include <times.h>
#include <clock.h>

#define SLOTX 6
#define CYCLEX 4
#define SLOT_T 5000
#define THREAD_COUNT 3
#define STORE_COUNT 8


int tps,cycle=0,slot=0;
//clock_t now, then;
//struct tms n;

/* Task Definitions */

void task1(void) {
  printf("Task 1 running\n");
  sleep(2);
}


void task2(void) {
  printf("Task 2 running\n");
  sleep(2);
}

void task3(void) {
  printf("Task 3 running\n");
  sleep(4);
}

/*void burn() {
   
     clock_t bstart = times(&n);
       

         while ((( now = times(&n)) - then) < SLOT_T * tps / 1000) {
            
              }

                printf (" brn time = %2.2dms\n\n", (times(&n)-bstart)*1000/tps);
                          then = now;
                                  cycle = CYCLEX;
                                          }
*/
void burn(void) { printf (" brn cycle \n ");}
//task table
void (*ttable[SLOTX][CYCLEX])(void) = {
{task1,task1,task2,task2},
{task3,task3,task3,task3},
{task2,task2,task1,task1},
{task3,task3,task3,task3},
{task2,task2,task1,task1},
{task2,task2,burn,burn}};

/*
   // tps = sysconf(_SC_CLK_TCK);
   // printf("clock ticks/sec = %d\n\n", tps);
   // then = times(&n);
    while (1) {
      printf(" \n Starting a new hyperperiod \n");
      for (slot=0; slot <SLOTX; slot++)
        { printf ("\n Starting a new frame \n");
        for (cycle=0; cycle<CYCLEX; cycle++)
          (*ttable[slot][cycle])();
        }}
        return 0;
  }        

*/
/*void store(int times, uchar *test_array, int *shared) {
  int i = 0;
  for (i = 0; i < times; i++) {
    // thrcurrent is a global variable containing the tid of the currently executing thread. 
    test_array[*shared] = thrcurrent;
    *shared = *shared + 1;
    // Call yield again to make sure the thread with the highest priority executes next. 
    yield();
  }
}
*/
shellcmd xsh_cycExe(int nargs, char *args[])
{

      printf(" \n Starting a new hyperperiod - 24  \n");
      for (slot=0; slot <SLOTX; slot++)
        { printf ("\n Starting a new frame - 4 \n");
        for (cycle=0; cycle<CYCLEX; cycle++)
          (*ttable[slot][cycle])();
        }

   uchar test_array[THREAD_COUNT * STORE_COUNT];
   /* Thread IDs of Task 1,2 and 3 */
   tid_typ a_tid, b_tid,c_tid;
   /* Priorities */
   int a_prio = 3, b_prio = 2, c_prio = 1;
   int shared = 0;

   /* Create 3 threads and push them into the ready queue. */
   ready(a_tid = create(task1, INITSTK, a_prio, "Task 1", 3, STORE_COUNT, test_array, &shared), RESCHED_NO);
   ready(b_tid = create(task2, INITSTK, b_prio, "Task 2", 3, STORE_COUNT, test_array, &shared), RESCHED_NO);
   ready(c_tid = create(task3, INITSTK, c_prio, "Task 3", 3, STORE_COUNT, test_array, &shared), RESCHED_NO);

   // Yield the processor and reschedule the processor to the thread with the highest priority.
   //yield();

  // fprintf(stdout,"In this test case, Task A has priority %d.\nTask B has priority %d.\n", a_prio, b_prio);

  if (a_prio > b_prio) fprintf(stdout, "Task 1 has a higher priority than Task 2.\n");
  else if (a_prio < b_prio) fprintf(stdout, "Task 2 has a higher priority than Task 1 .\n");
  else if (a_prio < c_prio) fprintf(stdout, "Task 1 has a higher priority than Task 3.\n");
  else if (a_prio > c_prio) fprintf(stdout, "Task 1 has a higher priority than Task 3.\n");
  else if (b_prio > c_prio) fprintf(stdout, "Task 2 has a higher priority than Task 3.\n");
  else if (b_prio < c_prio) fprintf(stdout, "Task 3 has a higher priority than Task 1.\n");
  else fprintf(stdout, "Task A and Task B have the same priority.\n");

   fprintf(stdout,"Thread ID of Task 1 is %d.\nThread ID of Task 2 is %d.\nThread ID of Task 3 is %d.\n", a_tid, b_tid, c_tid);

   return 0;
}
