/**
 * @file main.c
 * @author Andrew Lauricella (allauric@asu.edu)
 * @brief A simple program to take in data from flights.txt and output the max flow of the flights
 * @version 0.1
 * @date 2022-12-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


//LIBRARY INCLUSIONS
////////////////////
#include <algorithm>
#include <fstream>
#include <string.h>
#include <string>
#include "util.h"


//DATA STRUCTURES
/////////////////

struct Node * HEAD;
struct Node * TAIL;
std::vector<struct Node *> AIRPORTS; 
std::vector<struct Flight * > ALL_FLIGHTS;
std::vector<struct Flight * > REJECTED_FLIGHTS;
std::vector<struct Flight * > TERMINATORS;
std::vector<struct Flight_Plan * > VALID_PLANS;
int TOTAL_MAX_FLOW = 0;

//Los Angeles, San Francisco, Phoenix, Seattle, Denver, Atlanta, Chicago, Boston, Washington D.C., New York City
std::string recognized_airports[10] = {"LAX", "SFO", "PHX", "SEA", "DEN", "ATL", "ORD", "BOS", "IAD", "JFK"};

//American Airlines, Delta Airlines, United Airlines
std::string recognized_airlines[3] = {"AA", "DL", "UA"}; 


//METHOD PROTOTYPES+
///////////////////


/***************************
 * BEGIN METHOD DEFINITIONS
 **************************/

void Graph::create_graph(){

    //create all recognized airports as a new Node struct
    struct Node * LAX = new struct Node("LAX");
    AIRPORTS.push_back(LAX);
    struct Node * SFO = new struct Node("SFO");
    AIRPORTS.push_back(SFO);
    struct Node * PHX = new struct Node("PHX");
    AIRPORTS.push_back(PHX);
    struct Node * SEA = new struct Node("SEA");
    AIRPORTS.push_back(SEA);
    struct Node * DEN = new struct Node("DEN");
    AIRPORTS.push_back(DEN);
    struct Node * ATL = new struct Node("ATL");
    AIRPORTS.push_back(ATL);
    struct Node * ORD = new struct Node("ORD");
    AIRPORTS.push_back(ORD);
    struct Node * BOS = new struct Node("BOS");
    AIRPORTS.push_back(BOS);
    struct Node * IAD = new struct Node("IAD");
    AIRPORTS.push_back(IAD);
    struct Node * JFK = new struct Node("JFK");
    AIRPORTS.push_back(JFK);

    //set the HEAD (source) Node as LAX
    //set the FOOT (terminal) Node as JFK
    HEAD = LAX;
    TAIL = JFK;


    std::ifstream raw_data("flights.txt");

    std::string src, dst = "";
    int depart_time, arrival_time, capacity = 0;
    int flight_count = 0;

    while(raw_data >> src >> dst >> depart_time >> arrival_time >> capacity){

        struct Node * temp1 = NULL;
        struct Node * temp2 = NULL;

        for(auto item = AIRPORTS.begin(); item != AIRPORTS.end(); item++){
            if(!src.compare((*item)->NAME)){
                temp1 = (*item);
                break;
            }    
        }

        for(auto item = AIRPORTS.begin(); item != AIRPORTS.end(); item++){
            if(!dst.compare((*item)->NAME)){
                temp2 = (*item);
                break;
            }    
        }

        if(!(depart_time >= 0 && depart_time < 24 && arrival_time >= 0 && arrival_time <= 24 && capacity > 0)){
            depart_time, arrival_time, capacity = -1;
        }

        if((temp1 != NULL && temp2 != NULL) && (depart_time != -1 && arrival_time != -1 && capacity != -1)){
            struct Flight * newFlight = new Flight(temp1, temp2, depart_time, arrival_time, capacity);

            temp1->AVAILABLE_FLIGHTS.push_back(newFlight);
            ALL_FLIGHTS.push_back(newFlight);

            flight_count++;
        }else{
            struct Flight * newFlight = new Flight(temp1, temp2, depart_time, arrival_time, capacity);
            REJECTED_FLIGHTS.push_back(newFlight);
        }

    }    

}

void Graph::debug_flight_data(){
    int count = 0;
    for(auto item = ALL_FLIGHTS.begin(); item != ALL_FLIGHTS.end(); item++){
        count++;
        printf("%s %s %d %d %d\n", (*item)->SOURCE->NAME.c_str(), (*item)->DESTINATION->NAME.c_str(), (*item)->DEPART_TIME, (*item)->ARRIVAL_TIME, (*item)->CAPACITY);
    }

    printf("\n");

    for(auto item = REJECTED_FLIGHTS.begin(); item != REJECTED_FLIGHTS.end(); item++){
        count++;
        printf("%s %s %d %d %d\n", (*item)->SOURCE->NAME.c_str(), (*item)->DESTINATION->NAME.c_str(), (*item)->DEPART_TIME, (*item)->ARRIVAL_TIME, (*item)->CAPACITY);
    }

    printf("DONE\nTOTAL COUNT = %d\n", count);
}

void Graph::debug_airport_data(){
    int count = 0;
    for(auto airp = AIRPORTS.begin(); airp != AIRPORTS.end(); airp++){

        for(auto item = (*airp)->AVAILABLE_FLIGHTS.begin(); item != (*airp)->AVAILABLE_FLIGHTS.end(); item++){
            count++;
            printf("%s %s %d %d %d\n", (*item)->SOURCE->NAME.c_str(), (*item)->DESTINATION->NAME.c_str(), (*item)->DEPART_TIME, (*item)->ARRIVAL_TIME, (*item)->CAPACITY);
        }
        
    }

    printf("\n");

    for(auto item = REJECTED_FLIGHTS.begin(); item != REJECTED_FLIGHTS.end(); item++){
        count++;
        printf("%s %s %d %d %d\n", (*item)->SOURCE->NAME.c_str(), (*item)->DESTINATION->NAME.c_str(), (*item)->DEPART_TIME, (*item)->ARRIVAL_TIME, (*item)->CAPACITY);
    }

    printf("DONE!\nTOTAL COUNT = %d\n", count);
}

bool compare_depart_times(struct Flight * f1, struct Flight * f2){
    return (f1->DEPART_TIME < f2->DEPART_TIME);
}

void Graph::sort_flights(){

    for(auto item = AIRPORTS.begin(); item != AIRPORTS.end(); item++){
        sort((*item)->AVAILABLE_FLIGHTS.begin(), (*item)->AVAILABLE_FLIGHTS.end(), compare_depart_times);
    }
}

void Graph::create_valid_itineraries(){
    ///*
    for(auto f1 = HEAD->AVAILABLE_FLIGHTS.begin(); f1 != HEAD->AVAILABLE_FLIGHTS.end(); f1++){
        //if destination is jfk, create and add itinerary
        //else iterate through destination's available flights and repeat
        if((*f1)->DESTINATION == TAIL){
            VALID_PLANS.push_back(new Flight_Plan((*f1)));
        }else{

            for(auto f2 = (*f1)->DESTINATION->AVAILABLE_FLIGHTS.begin(); f2 != (*f1)->DESTINATION->AVAILABLE_FLIGHTS.end(); f2++){
                //check for f2 depart time is <= f1 arrival time

                if((*f2)->DEPART_TIME >= (*f1)->ARRIVAL_TIME){
                    if((*f2)->DESTINATION == TAIL){
                        VALID_PLANS.push_back(new Flight_Plan((*f1), (*f2)));
                    }else{

                        for(auto f3 = (*f2)->DESTINATION->AVAILABLE_FLIGHTS.begin(); f3 != (*f2)->DESTINATION->AVAILABLE_FLIGHTS.end(); f3++){
                            if((*f3)->DEPART_TIME >= (*f2)->ARRIVAL_TIME){
                                if((*f3)->DESTINATION == TAIL){
                                    VALID_PLANS.push_back(new Flight_Plan((*f1), (*f2), (*f3)));
                                }
                            }
                        }
                    }
                }
            }

        }
    }
    //*/
    
}

void Graph::debug_itinerary_data(){
    int counter = 0;

    for(auto itin = VALID_PLANS.begin(); itin != VALID_PLANS.end(); itin++){

        counter++;

        if((*itin)->FIRST != NULL){
            printf("PLAN %d (Max capacity: %d):\n%s -> %s | ", counter, (*itin)->PLAN_MAX_CAP, (*itin)->FIRST->SOURCE->NAME.c_str(), (*itin)->FIRST->DESTINATION->NAME.c_str());
            printf("%d\t-\t%d | %d\t-\t%d |", (*itin)->FIRST->DEPART_TIME, (*itin)->FIRST->ARRIVAL_TIME, (*itin)->FIRST->FILLED_SEATS, (*itin)->FIRST->CAPACITY);
        }

        if((*itin)->SECOND != NULL){
            printf("\n%s -> %s | ", (*itin)->SECOND->SOURCE->NAME.c_str(), (*itin)->SECOND->DESTINATION->NAME.c_str());
            printf("%d\t-\t%d | %d\t-\t%d |", (*itin)->SECOND->DEPART_TIME, (*itin)->SECOND->ARRIVAL_TIME, (*itin)->SECOND->FILLED_SEATS, (*itin)->SECOND->CAPACITY);
        }

        if((*itin)->THIRD != NULL){
            printf("\n%s -> %s | ", (*itin)->THIRD->SOURCE->NAME.c_str(), (*itin)->THIRD->DESTINATION->NAME.c_str());
            printf("%d\t-\t%d | %d\t-\t%d |", (*itin)->THIRD->DEPART_TIME, (*itin)->THIRD->ARRIVAL_TIME, (*itin)->THIRD->FILLED_SEATS, (*itin)->THIRD->CAPACITY);
        }

        printf("\n--------------------------------------------\n\n");

    }
}

bool compare_path_flow(struct Flight_Plan * p1, struct Flight_Plan * p2){
    return (p1->PLAN_MAX_CAP < p2->PLAN_MAX_CAP);
}

void Graph::sort_paths(){

    sort(VALID_PLANS.begin(), VALID_PLANS.end(), compare_path_flow);
    
}

void Graph::calculate_max_flow(){

    std::vector<struct Flight_Plan * > PATHS_USED;

    
    for(auto itin = VALID_PLANS.begin(); itin != VALID_PLANS.end(); itin++){
        int path_allowed_flow = (*itin)->FIRST->CAPACITY - (*itin)->FIRST->FILLED_SEATS;

        if((*itin)->SECOND != NULL){
            path_allowed_flow = std::min(path_allowed_flow, (*itin)->SECOND->CAPACITY - (*itin)->SECOND->FILLED_SEATS);
        }

        if((*itin)->THIRD != NULL){
            path_allowed_flow = std::min(path_allowed_flow, (*itin)->THIRD->CAPACITY - (*itin)->THIRD->FILLED_SEATS);
        }

        if(path_allowed_flow > 0){
            (*itin)->FLOW = path_allowed_flow + (*itin)->FLOW;
            (*itin)->FIRST->FILLED_SEATS = path_allowed_flow + (*itin)->FIRST->FILLED_SEATS;

            if((*itin)->SECOND != NULL){
                (*itin)->SECOND->FILLED_SEATS = path_allowed_flow + (*itin)->SECOND->FILLED_SEATS;
            }

            if((*itin)->THIRD != NULL){
                (*itin)->THIRD->FILLED_SEATS = path_allowed_flow + (*itin)->THIRD->FILLED_SEATS;
            }

            PATHS_USED.push_back(*itin);
            VALID_PLANS.erase(itin);

        }

    }

    for(auto itin = ALL_FLIGHTS.begin(); itin != ALL_FLIGHTS.end(); itin++){
        if((*itin)->DESTINATION == TAIL){
            TOTAL_MAX_FLOW += (*itin)->FILLED_SEATS;
        }
    }

    printf("\n========================\nMAX GRAPH FLOW: %d\n========================\n", TOTAL_MAX_FLOW);



}

void Graph::capacity_all_flights(){
    int total = 0;

    for(auto flight = ALL_FLIGHTS.begin(); flight != ALL_FLIGHTS.end(); flight++){
        total += (*flight)->CAPACITY;
    }

    printf("\nMAX CAPACITY: %d\n========================\n", total);
}