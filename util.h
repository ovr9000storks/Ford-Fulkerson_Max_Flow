#ifndef __UTIL_H__
#define __UTIL_H__


//LIBRARY INCLUSIONS
////////////////////
#include <string.h>
#include <string>
#include <vector>


//DATA STRUCTURES
/////////////////

extern struct Node * HEAD;
extern struct Node * TAIL;
extern std::vector<struct Node *> AIRPORTS;

struct Flight{
    struct Node * SOURCE;
    struct Node * DESTINATION;
    int DEPART_TIME;
    int ARRIVAL_TIME;
    int CAPACITY;
    int FILLED_SEATS;

    int FLIGHT_ID;

    Flight(){
        SOURCE = NULL;
        DESTINATION = NULL;
        DEPART_TIME, ARRIVAL_TIME, CAPACITY, FILLED_SEATS, FLIGHT_ID = -1;
        
    }

    Flight(struct Node * src, struct Node * dst, int dpt_time, int arvl_time, int capacity){

        
        SOURCE = src;
        DESTINATION = dst;
        
        DEPART_TIME = dpt_time;
        ARRIVAL_TIME = arvl_time;
        CAPACITY = capacity;
        FILLED_SEATS = 0;
    }   
};


struct Flight_Plan{
    struct Flight * FIRST;
    struct Flight * SECOND;
    struct Flight * THIRD;
    int PLAN_MAX_CAP;
    int FLOW;

    Flight_Plan(){
        FIRST = NULL;
        SECOND = NULL;
        THIRD = NULL;
        PLAN_MAX_CAP = 0;
        FLOW = 0;
    }

    Flight_Plan(struct Flight * first){
        FIRST = first;
        SECOND = NULL;
        THIRD = NULL;
        PLAN_MAX_CAP = first->CAPACITY;
        FLOW = 0;
    }

    Flight_Plan(struct Flight * first, struct Flight * second){
        FIRST = first;
        SECOND = second;
        THIRD = NULL;
        PLAN_MAX_CAP = std::min((first->CAPACITY), (second->CAPACITY));
        FLOW = 0;
    }

    Flight_Plan(struct Flight * first, struct Flight * second, struct Flight * third){
        FIRST = first;
        SECOND = second;
        THIRD = third;
        PLAN_MAX_CAP = std::min(std::min((first->CAPACITY), (second->CAPACITY)), (third->CAPACITY));
        FLOW = 0;
    }
};

struct Node{
    std::string NAME;
    std::vector<struct Flight * > AVAILABLE_FLIGHTS;

    Node(){
        NAME = "N/A";
        AVAILABLE_FLIGHTS.clear();
    }
    
    Node(std::string name){
        NAME = name;
        AVAILABLE_FLIGHTS.clear();
    }
};



//METHOID PROTOTYPES
////////////////////
void create_graph();
void debug_flight_data();
void debug_airport_data();
void sort_flights();
void create_valid_itineraries();
void debug_itinerary_data();
void calculate_max_flow();
void sort_paths();
void capacity_all_flights();
//bool compare_depart_times(struct Flight * f1, struct Flight * f2);


class Graph {
    public:
        void create_graph();
        void debug_flight_data();
        void debug_airport_data();
        void sort_flights();
        void create_valid_itineraries();
        void debug_itinerary_data();
        void calculate_max_flow();
        void sort_paths();
        void capacity_all_flights();
    private:
        //bool compare_depart_times(struct Flight * f1, struct Flight * f2);
};

#endif