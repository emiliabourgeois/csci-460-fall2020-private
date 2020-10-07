/*
 * Simulates cars on a one way road with a max car limit between two destinations
 * ./b2bsim NUMCARS MAXCARS [RANDSEED] [VERBOSITY] 
 * Author: Emilia Bourgeois
 */

// includes 
#include <pthread.h> 				
#include <time.h>					
#include <stdlib.h>	
#include <stdio.h>	
#include <string.h>			
#include <unistd.h>

//define variables for easy comparison
#define TO_BRIDGER 0
#define TO_BOZEMAN 1

//thread mutex's to handle lock and unlocking threads
pthread_mutex_t one = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t two = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t three = PTHREAD_MUTEX_INITIALIZER;

//global initialization, RANDSEED and VERBOSITY are their default values
int MAXCARS;
int NUMCARS;
int RANDSEED = 42;
int VERBOSITY = 1;

int bridger_wait = 0; //cars waiting for bridge
int bozeman_wait = 0;

int bridger_count = 0; // cars on road now
int bozeman_count = 0;

int passed = 0; //cars crossed bridge

int current_direction; //current direction for final print out

int global_id = 0; //stores thread number

void arriveOneWay(int direction, int id){
    //loops indifinetely if the road is full so not all the cars are here at once 
    if(direction == TO_BRIDGER){
        bridger_wait++; //increments cars waiting
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
    //unlocks last thread and locks current thread so we don't go over maxcars
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
    //makes sure road is clear
    if(direction == TO_BRIDGER){
        while(bozeman_count > 0 || bridger_count >= MAXCARS){  
        }
    }
    else if(direction == TO_BOZEMAN){
        while(bridger_count > 0 || bozeman_count >= MAXCARS){  
        }
    }
    
    char d[10]; //prints out direction later
    if(direction == TO_BRIDGER){
        bridger_count++; //removes car from waiting and adds to road
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
    //printout for state of road
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
    sleep(1); //crossing roads takes time!
}

void exitOneWay(int direction, int id){
    //removes cars from road once they're done crossing
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
    passed++; //adds car to passed road for final printout
}

//quick random generator based on parameters, was planning on using this for more things, but that just caused problems
int getRand(int m){
    return rand() % (m + 1);
}


void *oneVehicle(void *vargp){
    //sets id, direction, and direction name 
    int rc; //used to lock/unlock threads
    int id = global_id++;
    int direction = (int) vargp;
    char d[10]; //namje of direction for easy printouts
    if(direction == TO_BOZEMAN){
        strcpy(d, "Bozeman");
    }
    else if(direction == TO_BRIDGER){
        strcpy(d, "Bridger");
    }
    //first of many verbosity printouts to show what's going on behind the scenes
    if(VERBOSITY == 1) {
        fprintf(stdout, "Thread %d (arrive) arriving at oneway going to %s...\n", id, d);
        fprintf(stdout, "Thread %d (arrive) trying to acquire lock...\n", id);
    }
    //mutex one for arriveOneWay()
    rc = pthread_mutex_lock(&one);
    if(rc){
        fprintf(stdout, "Lock failed\n");
        exit(-1);
    }
    //locks threads so each car arrives individually
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (arrive) acquired lock...\n", id);
    
    arriveOneWay(direction, id);
    
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (arrive) released lock\n", id);
    //unlocks if successful
    rc = pthread_mutex_unlock(&one);
    if(rc){
        fprintf(stdout, "Unlock failed\n");
        exit(-1);
    }

    //runs when car is on oneway
    onOneWay(direction, id);
    if(VERBOSITY == 1)
        fprintf(stdout, "Thread %d (exit) trying to acquire lock...\n", id);
    //lock for exitoneway so only one car leaves at a time
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
    //sets currrent direction for later
    current_direction = direction;

    return NULL;
}

int main(int argc, char* argv[]) {

    //checks command line arguements
    if(argc < 3){
        fprintf(stdout, "Missing command line arguements:\n./b2bsim NUMCARS MAXCARS [RANDSEED] [VERBOSITY]\n");
        exit(-1);
    }
    //uses command line arguements
    NUMCARS = atoi(argv[1]);
    MAXCARS = atoi(argv[2]);
    //more than 500 threads refuses to work on my VM
    if(NUMCARS >= 500){
        fprintf(stdout, "Please enter a number less than 500!\n");
        exit(-1);
    }
    //fun
    if(MAXCARS >= NUMCARS){
        fprintf(stdout, "Half of the simulation is pointless now, but I guess you can do what you want\n");
    }  
    //sets randseed and verbosity
    if(argc >= 4){
        RANDSEED = atoi(argv[3]);
    }  
    if(argc >= 5) {
        VERBOSITY = atoi(argv[4]);
    }  

    //header and variable printouts
    fprintf(stdout, "======================================\n");
    fprintf(stdout, "Welcome to the Bridger Pass Simulator!\n");
    fprintf(stdout, "--------------------------------------\n");
    fprintf(stdout, "Simulation Configuration:\n");
    fprintf(stdout, "NUMCARS     = %d\n", NUMCARS);
    fprintf(stdout, "MAXCARS     = %d\n", MAXCARS);
    fprintf(stdout, "RANDSEED    = %d\n", RANDSEED);
    fprintf(stdout, "VERBOSITY   = %d\n", VERBOSITY);
    fprintf(stdout, "======================================\n");
    
    //sets random seed
    srand(RANDSEED);

    ///cars, array of threads
    pthread_t cars[NUMCARS];

    //air conditioning for threads
    int ac;
    //creates threads based on NUMCARS and has each run oneVehicle, also passes in random direction
    for(int i = 0; i < NUMCARS; i++){
        ac = pthread_create(&cars[i], NULL, oneVehicle, (void *) getRand(1));
        if(ac){
            fprintf(stdout, "Thread id=%d creation failed\n", i);
            exit(-2);
        }
    }
    //closes threads by joining them with NULL
    for(int i = 0; i < NUMCARS; i++) {
        ac = pthread_join(cars[i], NULL);
        if(ac){
            fprintf(stdout, "Closing thread %d failed!\n", i);
            exit(-2);
        }
    }
    //final state printout
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

    return 0;
}
