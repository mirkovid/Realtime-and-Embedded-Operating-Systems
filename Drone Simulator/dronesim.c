/* File dronesim.c
 * A basic drone simulation system for CSE 321 Project
 * Author : Tejas Vyas 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/*
 * Drone States:
 * State(-1): Rest State (Drone at Base DECOY) -> Initialization of DECOY System 
 * State(0) : Initial State (Take Off) -> droneInit
 * State(1) : In Air (On route to the destination) -> droneEnRoute
 * State(2) : Target Reached (Delivery Address reached - Drop off package) -> droneAddressReached
 * State(3) : Delivery Completed (Delivered Package; Returning to base DECOY) -> droneTaskFin
 * State(-1) : Reached base DECOY ready for next assignment.
 */

#define droneInit     0
#define droneEnRoute 1
#define droneAddressReached 2
#define droneTaskFin  3

pthread_mutex_t lock;
int currState = -1;

void *CreateDrone(void *);
void funcDroneInit(int *);
void funcDroneEnRoute(int *);
void funcDroneAddressReached(int *);
void funcDroneTaskFin(int *);

/**
 * CreateDrone -> Initializes new drone instance. Beginning State.
 * @param	arg data for a new Drone initialization.
 * @return	Exits p-thread
 */
void *CreateDrone(void *arg)
{
    int *data = (int *) arg;
    funcDroneInit(data); 
    pthread_exit(0);
}
/**
 * CreateDrone -> Initializes new drone instance. Beginning State.
 * @param	data gives us station information
 */
void funcDroneInit(int *data)
{
    //If any other drone has been dispatched, wait until it takes off, then initialize takeoff
    if (currState == droneInit) {
        sleep(1);
    } else {
        sleep(1);
        currState = droneInit;
        pthread_mutex_unlock(&lock);
        printf("> A drone from DECOY %ld is taking off for its destination address.\n", data[1] + 1);
        pthread_mutex_lock(&lock);
        funcDroneEnRoute(data);
    }
}

void funcDroneEnRoute(int *data)
{
    // Wait if there is already a Drone in the air to avoid collision
    if (currState == droneEnRoute) {
        sleep(2);
    } else {
        currState = droneEnRoute;
        pthread_mutex_unlock(&lock);
        printf("> Drone from DECOY %ld is on route to its designated delivery address. Collision Detection - ON.\n", data[1] + 1);
        pthread_mutex_lock(&lock);
        funcDroneAddressReached(data);
    }
}

void funcDroneAddressReached(int *data)
{
    // Wait if there is already a Drone in the air to avoid collision
    if (currState == droneAddressReached) {
        sleep(1);
    } else {
        currState = droneAddressReached;
        pthread_mutex_unlock(&lock);
		int x = rand() % 100;
		int y = rand() % 100;
        printf("> Drone from DECOY %ld has reached his destination coordinates(%d, %d). Delivering payload now...\n", data[1] + 1,x,y);
        pthread_mutex_lock(&lock);
        funcDroneTaskFin(data);
    }
}

void funcDroneTaskFin(int *data)
{
    //Check whether any other drone is landing, if so, wait until it is done, then land.
    if (currState == droneTaskFin) {
        sleep(2);
    } else {
        currState = droneTaskFin;
        pthread_mutex_unlock(&lock);
        printf("> Drone from DECOY %ld delivered payload, returning back to base.\n", data[1] + 1);
    }
}

/*
 * Main function to initialize DECOY simulation
 */
int main (int argc, char *argv[])
{
    char line[256];
    printf("%s", "> Please enter number of drones (threads) for the DECOY simulation: "); //read number of drones to run simulations with
    int numDrone = atoi(fgets(line, sizeof(line), stdin));
    if (numDrone > 1){
		printf("> Since multiple drones are delivering in the air simultaneously, the order of delivery may appear to be arbitrary.\n>\n");
		}
    int rc, a, b, currDrone, numDecoy;
    numDecoy = 0;
	
	pid_t p;
    pthread_t thread[numDrone];
    pthread_mutexattr_t attr;
    
    /* Initialize pthread with set attribute. The drones awaken! */
    pthread_mutexattr_init(&attr);
    currDrone = 0; //currDrone = Drone #
	a = 1; 
	b = 1;
    while(currDrone<numDrone) {
        printf("> Initializing delivery sequence for Drone %ld from DECOY %ld. Current location of the drone (%d,%d)\n", currDrone + 1, numDecoy + 1, a, b); 
		/*DECOY location is assumed as (1,1) [at base]*/
        int droneData[2] = {currDrone, numDecoy};
        /* Thread locked to clear previous drone instances. (Collision Avoidance) */
        pthread_mutex_lock(&lock);
        rc = pthread_create(&thread[currDrone], NULL, CreateDrone, (void *)droneData); 
        if (rc) {
            printf("> ERROR; return code from pthread_create()is %d\n", rc); /*Error Handling - Exit*/
            exit(-1);
        }
		currDrone++;
    }
    
    /* Destroy pthread containing previous attribute. Return of the drones! */
    pthread_mutexattr_destroy(&attr);
	currDrone = 0;
	while(currDrone<numDrone) {
        rc = pthread_join(thread[currDrone], NULL); 
        if (rc) {	
            printf("> ERROR; return code from pthread_join() is %d\n", rc); /*Error Handling - Exit*/
            exit(-1);
        }
        printf("> Drone %ld returned to DECOY %d. Ready for next assignment.\n", currDrone + 1, numDecoy + 1);
		currDrone++;
    }
    
	/*Finish by destroying locked pthread instances.*/
    printf("> All payload deliveries have been completed by the drone for DECOY %d. Location: Base - (%d,%d)\n", numDecoy + 1, a, b);
    pthread_exit(NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
 
