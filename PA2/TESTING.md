# PA2 Testing (Emilia Bourgeois)
## Max threads
During my testing it seems like any number of cars over about 500 spits out something like ```Thread id=508 creation failed``` so now you cannot enter more than 500 cars because of threading limitations.
## Three cars on road 
When I was attempting to code this project it would sometimes show three cars on the road even though that shouldn't be possible with an input of two max. After some deducing I figured out that creating three ```pthread_mutex_t``` like so to control each function fixed it.
```C
pthread_mutex_t one = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t two = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t three = PTHREAD_MUTEX_INITIALIZER;
```
## Random outputs
I have not solved this issue, but the "solution" to a desired input like ```./b2bsim 10 2 41 0``` just randomly selects cars sometimes. It still works perfectly normal on each run however, so I don't see much of an issue, but I probably would have fixed this if I had more time. It's not the random seed being unimplemented either, because the cars always have the same destinations.
```
[*] Adding car 8: to Bridger [waiting: Bozeman=0 / Bridger=2]
[8]------------------------
Direction  : Bridger
Waiting    : Bozeman=1 / Bridger=1
On road    : 1
Cars passed: 7
---------------------------
[*] Adding car 9: to BOZEMAN [waiting: Bozeman=1 / Bridger=1]
[7]------------------------
Direction  : Bridger
Waiting    : Bozeman=1 / Bridger=0
On road    : 2
Cars passed: 7
---------------------------
[9]------------------------
Direction  : Bozeman
Waiting    : Bozeman=0 / Bridger=0
On road    : 1
Cars passed: 9
---------------------------
```
```
[1]------------------------
Direction  : Bozeman
Waiting    : Bozeman=1 / Bridger=1
On road    : 1
Cars passed: 5
---------------------------
[9]------------------------
Direction  : Bozeman
Waiting    : Bozeman=0 / Bridger=1
On road    : 2
Cars passed: 8
---------------------------
[4]------------------------
Direction  : Bridger
Waiting    : Bozeman=0 / Bridger=0
On road    : 1
Cars passed: 9
---------------------------
```
## Weird threads
For a while I was getting seemingly random results with the same input, like the final count would change randomly. Well turns out I probably wasn't closing all my threads properly because NUMCARS was MAXCARS for a while in the following block. 
```C
for(int i = 0; i < NUMCARS; i++) {
        ac = pthread_join(cars[i], NULL);
```