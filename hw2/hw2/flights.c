/* 
 * CS61C Summer 2013
 * Name:Fei Liu
 * Login:cs61c-ew
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "flights.h"
#include "timeHM.h"
typedef struct flight flight_t;
struct flight{
  char* depart;
  timeHM_t* leave;
  timeHM_t* arrive;
  int cost;  
  flight_t* next;
  flight_t* prev;
  int first; // check if this is the head of doubly linked list
};  
struct airport {
char * name;
flight_t * flights;
airport_t* next;
airport_t* prev;
int first;  //Check head
};
 
struct flightSys {
airport_t* airports;  //system handle
};
void flight_delete(flight_t* a){
     if (a!=NULL){
       flight_t *temp1=a,*temp2;
       do{
          temp2=temp1->next;
          
          free(temp1->depart);
          free(temp1->leave);
          free(temp1->arrive);
           free(temp1);
          temp1=temp2;

       } while(temp1!=a);//traverse the doubly list
       
     }
}
 void airport_delete(airport_t* a){
     if (a!=NULL){

         airport_t *temp1=a;
         if (a->next==a||a->next==NULL)
           a->next=a->prev=NULL;
         else {
           a->next->prev=a->prev;
           a->prev->next=a->next;
           a->next=NULL;
           a->prev=NULL;
         }// airport level relationship
         
         
          // within airport clear up
          if (temp1->flights==NULL){
           free(temp1->name);
           free(temp1);    
          }
          else{
          free(temp1->name);
          flight_delete(temp1->flights);
          free(temp1);
          }
     }else 
       fprintf(stderr,"pass null to  airport_delete\n");
 }

/*
   This should be called if memory allocation failed.
 */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(EXIT_FAILURE);
}


/*
   Creates and initializes a flight system, which stores the flight schedules of several airports.
   Returns a pointer to the system created.
 */
flightSys_t* createSystem() {
    // Replace this line with your code
    flightSys_t * s=(flightSys_t*)malloc(sizeof(flightSys_t));
    if (!s)
      allocation_failed();
    s->airports=NULL;   // starts with nothing
    return s;
}


/*
   Frees all memory associated with this system; that's all memory you dynamically allocated in your code.
 */
void deleteSystem(flightSys_t* s) {
    // Replace this line with your code
    if (s!=NULL) {
      if (s->airports!=NULL) {
        airport_t* temp;
        int index=1;    //find the head of the list
      while(s->airports&&index){
        temp=s->airports;
        s->airports=s->airports->next;
        if (temp==s->airports)
          index=0;   //hit the head can do with do while but....
        airport_delete(temp);
      }
       free(s);
       s=NULL;
      } else {
        free(s);//if no airport just delete the system
        s=NULL;
        fprintf(stderr,"Current FlightSys contains no airport!\n");
        // you can delete this test print
      }
    }else
      fprintf(stderr,"a NULL pointer is passed in to flight_delete");
    // you can to delete this test print
}


/*
   Adds a airport with the given name to the system. You must copy the string and store it.
   Do not store "name" (the pointer) as the contents it point to may change.
 */
void addAirport(flightSys_t* s, char* name) {
    // Replace this line with your code
    if (name!=NULL){
        airport_t* NewAirp=(airport_t*)malloc(sizeof(airport_t));
      if (!NewAirp){
        allocation_failed();
      }                             
      // prepare everything for new airport
        NewAirp->name=(char*) malloc(strlen(name)+1);
        if (!NewAirp->name){
          allocation_failed();
        }
        strcpy(NewAirp->name,name);
        NewAirp->flights=NULL;
        NewAirp->first=0; // key difference below is about this first
        NewAirp->next=NewAirp;
        NewAirp->prev=NewAirp; // things are different with base case
      if(s->airports){
        NewAirp->next=s->airports->next;
        NewAirp->prev=s->airports;
        s->airports->next->prev=NewAirp;
        s->airports->next=NewAirp;
        s->airports=NewAirp;
      }else{
        s->airports=NewAirp;
        s->airports->first=1;
      }
    }else
      fprintf(stderr,"the char string passed in is NULL\n");
}

   
/*
   Returns a pointer to the airport with the given name.
   If the airport doesn't exist, return NULL.
 */
airport_t* getAirport(flightSys_t* s, char* name) {
    // Replace this line with your code
    if (s){
            airport_t*  temp=s->airports;
        do{
          if (!strcmp(temp->name,name))
            return temp;
          else temp=temp->next;
        }while (temp!=s->airports);
        return NULL;
    }else{
      // you can delete this test function
      fprintf(stderr,"Flightsystem passed in getAirport doesn't exist!\n");
      return NULL;
    }
}


/*
   Print each airport name in the order they were added through addAirport, one on each line.
   Make sure to end with a new line. You should compare your output with the correct output
   in flights.out to make sure your formatting is correct.
 */
void printAirports(flightSys_t* s) {
    // Replace this lin with your code
    if (s){
      airport_t* temp=s->airports;
      // this is where I need first so that the order is exactly the same as
      // input
      while(temp->first==0){
          temp=temp->next;
        }
      do{
         fprintf(stdout,"%s\n",temp->name);
         temp=temp->next;
        } while(temp->first==0);
     
    } else
      fprintf(stderr,"There is nothing to print it's a Null Pointer\n");
}


/*
   Adds a flight to src's schedule, stating a flight will leave to dst at departure time and arrive at arrival time.
 */
void addFlight(airport_t* src, airport_t* dst, timeHM_t* departure, timeHM_t* arrival, int cost) {
    // Replace this line with your code
    if (dst&&src&&dst->name&&src->name) {
      flight_t * temp=(flight_t*)malloc(sizeof(flight_t));
       if(!temp) 
         allocation_failed();
      temp->depart=(char*)malloc(strlen(dst->name)+1);     //malloc contents
      temp->leave=(timeHM_t*)malloc(sizeof(timeHM_t));
      temp->arrive=(timeHM_t*)malloc(sizeof(timeHM_t));
      if (!temp->depart||!temp->leave||!temp->arrive)
        allocation_failed();

      strcpy(temp->depart,dst->name);
      *temp->leave=*departure;
      *temp->arrive=*arrival;
      temp->cost=cost;
      temp->next=temp;
      temp->prev=temp;
      temp->first=0;
      // similiar to add_airport, base case is a little different
      if (src->flights==NULL){
        temp->first=1;
        src->flights=temp;
      }
      else {
        temp->next=src->flights->next;
        temp->prev=src->flights;
        src->flights->next->prev=temp;
        src->flights->next=temp;
        src->flights=temp;
      }
    }else fprintf(stderr, "pass in addFlight invalide inputs!\n");
    
}


/*
   Prints the schedule of flights of the given airport.

   Prints the airport name on the first line, then prints a schedule entry on each 
   line that follows, with the format: "destination_name departure_time arrival_time $cost_of_flight".

   You should use printTime (look in timeHM.h) to print times, and the order should be the same as 
   the order they were added in through addFlight. Make sure to end with a new line.
   You should compare your output with the correct output in flights.out to make sure your formatting is correct.
 */
void printSchedule(airport_t* s) {
    // Replace this line with your code
    if (s){
      if (s->flights!=NULL){
        flight_t* temp=s->flights;
         while(temp->first==0){
            temp=temp->next;
         }   
         fprintf(stdout,"%s\n",s->name);
         do{
            fprintf(stdout,"%s ",temp->depart);
            printTime(temp->leave);
            fprintf(stdout," ");
            printTime(temp->arrive);
            fprintf(stdout," $%d\n",temp->cost);

            temp=temp->next;
         }while(temp->first==0);
      }else{
        fprintf(stderr,"Airport %s doesn't have any schedule\n",s->name);
      }
    }else
      fprintf(stderr,"Pass in Printschedule a NUll pointer\n");
}


/*
   Given a src and dst airport, and the time now, finds the next flight to take based on the following rules:
   1) Finds the cheapest flight from src to dst that departs after now.
   2) If there are multiple cheapest flights, take the one that arrives the earliest.

   If a flight is found, you should store the flight's departure time, arrival time, and cost in departure, arrival, 
   and cost params and return true. Otherwise, return false. 

   Please use the function isAfter() from time.h when comparing two timeHM_t objects.
 */
bool getNextFlight(airport_t* src, airport_t* dst, timeHM_t* now, timeHM_t* departure, timeHM_t* arrival, int* cost) {
    // Replace this line with your code
    if (src&&dst&&now&&departure&&arrival&&cost){  
    flight_t * temp=src->flights;       // check valide src or dst
    if (!temp) return false;    //check if no flights
    int holdCost=-1;     // fool proof initialization used to check if
                        // there exists a feasible flight
    do{
      // check name and time
      if (!strcmp(dst->name,temp->depart)&&(isAfter(temp->leave,now))){
             // initialization
             if (holdCost<0){
               holdCost=temp->cost;
               *cost=holdCost;
               *departure=*temp->leave;
               *arrival=*temp->arrive;
             }else if (holdCost>temp->cost){ //better deal
               holdCost=temp->cost;
               *cost=holdCost;
               *departure=*temp->leave;
               *arrival=*temp->arrive;    // deep copy
             
             }
      }
     
         temp=temp->next;
    }while(temp!=src->flights); // doubly linked list traverse.
    
    return holdCost<0?false:true;    // there may be flights but no good time
    }
    else return false;
}
