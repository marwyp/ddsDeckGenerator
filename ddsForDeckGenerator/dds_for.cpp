/*
   DDS, a bridge double dummy solver.

   Copyright (C) 2006-2014 by Bo Haglund /
   2014-2016 by Bo Haglund & Soren Hein.

   See LICENSE and README.
*/


// Test program based on example program with CalcAllTablesPBN function.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "dll.h"
#include "dds_for_Deck.h"

// enum for cmd validation
enum class validation{
  OK,
  INVALID_ARGUMENT,
  LESS_THAN_1,
  NOT_ENAUGH_INFO
};

// var for cmd validation
validation val;

// variables for cmd progress validation
int step;
bool enable;
std::string info;

int main(int argc, char *argv[])
{
  // default values
  enable = false;
  val = validation::NOT_ENAUGH_INFO;
  int for_amount = 0;
  std::string fileName = "result.csv";

  // cmd 
  std::vector<std::string> args(argv, argv + argc);
  for(size_t i = 0; i < args.size(); i++){
    // -a - amount section
    if(args[i] == "-a"){
      if(i + 1 < args.size()){
        if(args[i + 1][0] != '-'){
          try{
            for_amount = std::stoi(args[i + 1]);
            if(for_amount > 0){
              val = validation::OK;
            }else{
              val = validation::LESS_THAN_1;
            }
          }catch(std::invalid_argument &error){
            val = validation::INVALID_ARGUMENT;
          }   
        }
      }
    // -n - csv file name section
    }else if(args[i] == "-n"){
      if(i + 1 < args.size()){
        if(args[i + 1][0] != '-'){
          fileName = args[i + 1];
        }
      }
    // -p - progress section
    }else if(args[i] == "-p"){
      // step
      if(i + 1 < args.size()){
        if(args[i + 1][0] != '-'){
          try{
            step = std::stoi(args[i + 1]);
            if(step > 0){
              enable = true;
            }
          }catch(std::invalid_argument &error){
            std::cout << "invalid argument: progress disabled" << std::endl;
            enable = false;
          }   
        }
      }
      // info
      if(i + 2 < args.size()){
        if(args[i + 2][0] != '-'){
          info = args[i + 2];
        }
      }
    }
  }

  // main apk
  if(val == validation::OK){
    addHeadersToFile("wynik.csv");

    for(int number = 0; number < for_amount; number++){
      extern char myPBN[1][80];
      deckGenerate();       // new deck
      myQuickSort(0, 51);   // deck permutation
      suitSort();           // suit sort each player cards
      createPBN();          // create myPBN char array
      //showDeck();
      //showPBNString();

      suitAmount();
      countPoints();

      ddTableDealsPBN DDdealsPBN;
      ddTablesRes tableRes;
      allParResults pres;

      int mode = 0; // No par calculation
      int trumpFilter[DDS_STRAINS] = {0, 0, 0, 0, 0}; // All
      int res;
      char line[80];
      bool match;

  #if defined(__linux) || defined(__APPLE__)
    SetMaxThreads(0);
  #endif

      DDdealsPBN.noOfTables = 1;

      for (int handno = 0; handno < DDdealsPBN.noOfTables; handno++)
      {
        strcpy(DDdealsPBN.deals[handno].cards, myPBN[handno]);
      }


      res = CalcAllTablesPBN(&DDdealsPBN, mode, trumpFilter,
                            &tableRes, &pres);

      if (res != RETURN_NO_FAULT)
      {
        ErrorMessage(res, line);
        printf("DDS error: %s\n", line);
      }

      for (int handno = 0; handno < DDdealsPBN.noOfTables; handno++)
      {
        getContracts(&tableRes.results[handno]);
      }
      saveToCsvFile(fileName);
      if(enable){
        showProgress(number, step, info);
      }
    }

  // options
  }else if(val == validation::NOT_ENAUGH_INFO){
    std::cout << "Options: " << std::endl;
    std::cout << "-a <amount>\t\t amount of games to generate, number > 0, obligatory" << std::endl;
    std::cout << "-n <name>\t\t name of destinantion file, optional - default: result.csv" << std::endl;
    std::cout << "-p <step> <info>\t shows progress in making csv file, optional - disabled by default";
    std::cout << ", step - obligatory, info - optional" << std::endl;
  
  // less than 1 error
  }else if(val == validation::LESS_THAN_1){
    std::cout << "amount of games to generate must be higher than 0" << std::endl;

  // invalid argument error
  }else if(val == validation::INVALID_ARGUMENT){
    std::cout << "invalid argument in amount of games" << std::endl;
  }
  
}

