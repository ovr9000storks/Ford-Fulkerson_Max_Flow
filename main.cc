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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include "util.h"


//GLOBAL VARIABLES
//////////////////
Graph graph;


//METHOD PROTOTYPES
///////////////////
int import_flight_info();
void calculate_max_flow();
void analyze_results();

/***************************
 * BEGIN METHOD DEFINITIONS
 **************************/


/**
 * @brief main function, calls the methods to import the data, calculate the max flow, then analyze the reults
 * 
 * @param argc : program argument count
 * @param argv : program arguments, argv[1] is the text file to import data from. If none is specified, defaults to flights.txt
 * @return int : standard return, no significance
 */
int main(int argc, char * argv[]){

    import_flight_info();
    analyze_results();

    return 0;
}

int import_flight_info(){

    graph.create_graph();
    graph.sort_flights();
    graph.create_valid_itineraries();
    graph.calculate_max_flow();


    return 0;
}

void analyze_results(){
    
}