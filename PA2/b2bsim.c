/*
 * TODO: Write a brief summary of your program here.
 *
 * Author: Emilia Bourgeois
 */

// includes (add more as you need them!)
#include <pthread.h> 				
#include <time.h>					
#include <stdlib.h>	
#include <stdio.h>	
#include <string.h>			
#include <unistd.h>

#define TO_BRIDGER 0
#define TO_BOZEMAN 1

#define MAX 1000
pthread_mutex_t one = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t two = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t three = PTHREAD_MUTEX_INITIALIZER;
int MAXCARS;
int NUMCARS;
int RANDSEED = 42;
int VERBOSITY = 1;

int bridger_wait; //cars waiting for bridge
int bozeman_wait;

int bridger_count; // cars on road now
int bozeman_count;

int passed; //cars crossed bridge

int on_road;

int current_direction;

int global_id = 0;

void arriveOneWay(int direction, int id){
    if(direction == TO_BRIDGER){
        bridger_wait++;
        fprintf(stdout, "[*] Adding car %d: to Bridger [waiting: Bozeman=%d / Bridger=%d]\n", id, bozeman_wait, bridger_wait);
        while(bozeman_count > 0 || bridger_count >= MAXCARS){  
        }
    }
    else if(direction == TO_BOZEMAN){
        bozeman_wait++;
        fprintf(stdout, "[*] Adding car %d: to BOZEMAN [waiting: Bozeman=%d / Bridger=%d]\n", id, bozeman_wait, bridger_wait);
        while(bridger_count > 0 || bozeman_count >= MAXCARS){
        }
    }
}

void onOneWay(int direction, int id){
    int rc;
    rc = pthread_mutex_unlock(&two);
    if(rc){
        fprintf(stdout, "Unlock failed\n");
        exit(-1);
    }
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (oneway) released lock\n", id);
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (oneway) trying to acquire lock...\n", id);
    rc = pthread_mutex_lock(&two);
    if(rc){
        fprintf(stdout, "Lock failed\n");
        exit(-1);
    }
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (oneway) locked\n", id);
    if(direction == TO_BRIDGER){
        while(bozeman_count > 0 || bridger_count >= MAXCARS){  
        }
    }
    else if(direction == TO_BOZEMAN){
        while(bridger_count > 0 || bozeman_count >= MAXCARS){  
        }
    }
    
    char d[10];
    if(direction == TO_BRIDGER){
        bridger_count++;
        bridger_wait--;
        if(VERBOSITY == 1)
            fprintf(stdout, "Car %d on road going to Bridger\n", id);
        strcpy(d, "Bridger");
    }
    else if(direction == TO_BOZEMAN){
        bozeman_count++;
        bozeman_wait--;
        if(VERBOSITY == 1)
            fprintf(stdout, "Car %d on road going to Bozeman\n", id);
        
        strcpy(d, "Bozeman");
    }
    fprintf(stdout, "[%d]------------------------\n", id);
    fprintf(stdout, "Direction  : %s\n", d);
    fprintf(stdout, "Waiting    : Bozeman=%d / Bridger=%d\n", bozeman_wait, bridger_wait);
    if(direction == TO_BRIDGER){
        fprintf(stdout, "On road    : %d\n", bridger_count);
    }
    else if(direction == TO_BOZEMAN){
        fprintf(stdout, "On road    : %d\n", bozeman_count);
    }
    fprintf(stdout, "Cars passed: %d\n", passed);
    fprintf(stdout, "---------------------------\n");
    sleep(1);
}

void exitOneWay(int direction, int id){
    if(direction == TO_BRIDGER){
        if(VERBOSITY == 1)
            fprintf(stdout, "Car %d left road going to Bridger\n", id);
        bridger_count--;
    }
    else if(direction == TO_BOZEMAN){
        if(VERBOSITY == 1)
            fprintf(stdout, "Car %d left road going to Bozeman\n", id);
        bozeman_count--;
    }
    passed++;
}

int getRand(int m){
    return rand() % (m + 1);
}


void *oneVehicle(void *vargp){
    int rc; 
    int id = global_id++;
    int direction = (int) vargp;
    char d[10];
    if(direction == TO_BOZEMAN){
        strcpy(d, "Bozeman");
    }
    else if(direction == TO_BRIDGER){
        strcpy(d, "Bridger");
    }
    if(VERBOSITY == 1) {
        fprintf(stdout, "Thread %d (arrive) arriving at oneway going to %s...\n", id, d);
        fprintf(stdout, "Thread %d (arrive) trying to acquire lock...\n", id);
    }

    rc = pthread_mutex_lock(&one);
    if(rc){
        fprintf(stdout, "Lock failed\n");
        exit(-1);
    }
    
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (arrive) acquired lock...\n", id);
    
    arriveOneWay(direction, id);
    
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (arrive) released lock\n", id);
    
    rc = pthread_mutex_unlock(&one);
    if(rc){
        fprintf(stdout, "Unlock failed\n");
        exit(-1);
    }

    //while(bridger_count >= MAXCARS || bozeman_count >= MAXCARS){

    //}
    
    
    onOneWay(direction, id);
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (exit) trying to acquire lock...\n", id);
    rc = pthread_mutex_lock(&three);
    if(rc){
        fprintf(stdout, "Lock failed\n");
        exit(-1);
    }
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (exit) acquired lock\n", id);
    exitOneWay(direction, id);
    rc = pthread_mutex_unlock(&three);
    if(rc){
        fprintf(stdout, "Unlock failed\n");
        exit(-1);
    }
    
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (exit) released lock\n", id);
    
    current_direction = direction;

    return NULL;
}

int main(int argc, char* argv[]) {
    NUMCARS = atoi(argv[1]);
    MAXCARS = atoi(argv[2]);  
    if(argc >= 4){
        RANDSEED = atoi(argv[3]);
    }  
    if(argc >= 5) {
        VERBOSITY = atoi(argv[4]);
    }  

    bridger_count = 0;
    bozeman_count = 0;

    bridger_wait = 0;
    bozeman_wait = 0;
    
    passed = 0;
    
    fprintf(stdout, "======================================\n");
    fprintf(stdout, "Welcome to the Bridger Pass Simulator!\n");
    fprintf(stdout, "--------------------------------------\n");
    fprintf(stdout, "Simulation Configuration:\n");
    fprintf(stdout, "NUMCARS     = %d\n", NUMCARS);
    fprintf(stdout, "MAXCARS     = %d\n", MAXCARS);
    fprintf(stdout, "RANDSEED    = %d\n", RANDSEED);
    fprintf(stdout, "VERBOSITY   = %d\n", VERBOSITY);
    fprintf(stdout, "======================================\n");
    
    srand(RANDSEED);

    pthread_t cars[NUMCARS];

    int ac;
    for(int i = 0; i < NUMCARS; i++){
        ac = pthread_create(&cars[i], NULL, oneVehicle, (void *) getRand(1));
        if(ac){
            fprintf(stdout, "Thread id=%d creation failed\n", i);
            exit(-2);
        }
    }

    for(int i = 0; i < NUMCARS; i++) {
        ac = pthread_join(cars[i], NULL);
        if(ac){
            fprintf(stdout, "Closing thread %d failed!\n", i);
            exit(-2);
        }
    }

    fprintf(stdout, "=======================================\n");
    fprintf(stdout, "Final State\n");
    fprintf(stdout, "------------------------\n");
    if(current_direction == TO_BRIDGER){
        fprintf(stdout, "Direction  : Bridger\n");
    }
    else if(current_direction == TO_BOZEMAN){
        fprintf(stdout, "Direction  : Bozeman\n");
    }
    
    fprintf(stdout, "Waiting    : Bozeman=%d / Bridger=%d\n", bozeman_wait, bridger_wait);
    if(current_direction == TO_BRIDGER){
        fprintf(stdout, "On road    : %d\n", bridger_count);
    }
    else if(current_direction == TO_BOZEMAN){
        fprintf(stdout, "On road    : %d\n", bozeman_count);
    }
    fprintf(stdout, "Cars passed: %d\n", passed);
    fprintf(stdout, "=======================================\n");
    // TODO: handle input arguments (print help statement if needed)

    // TODO: initializations for simulation

    // TODO: main loop that drives the simulation
    // In each iteration of the loop...
    //   - initalize/update any info for keeping track of threads (cars)
    //   - create a thread (car) that starts running "oneVehicle"

    // TODO: wait for car threads to finish & cleanup.

    // TODO: display final state of your oneway simulation before exiting.

    return 0;
}
