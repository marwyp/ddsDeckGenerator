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
#include "dds_40_Deck.h"

// enum for cmd validation
enum class validation{
  OK,
  INVALID_ARGUMENT,
  LESS_THAN_1,
  MORE_THAN_40,
  NOT_ENAUGH_INFO
};

// var for cmd validation
validation val;

int main(int argc, char *argv[])
{
  // number od decks
  long unsigned int numberOfDecks;

  // default values for cmd
  val = validation::NOT_ENAUGH_INFO;
  numberOfDecks = 0;
  std::string fileName = "result.csv";

  // cmd 
  std::vector<std::string> args(argv, argv + argc);
  for(size_t i = 0; i < args.size(); i++){
    // -a - amount section
    if(args[i] == "-a"){
      if(i + 1 < args.size()){
        if(args[i + 1][0] != '-'){
          try{
            numberOfDecks = static_cast<long unsigned int>(std::stoi(args[i + 1]));
            if(numberOfDecks < 1){
              val = validation::LESS_THAN_1;
            }else if(numberOfDecks > 40){
              val = validation::MORE_THAN_40;
            }else{
              val = validation::OK;
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
    }
  }

  // main apk
  if(val == validation::OK){
    // PBN string
    char **myPBN = new char*[numberOfDecks];
    for(long unsigned int i = 0; i < numberOfDecks; i++){
      myPBN[i] = new char[80];
    }

    // default, empty game
    game emptyGame;

    // information about each game
    std::vector<game> games(numberOfDecks, emptyGame);

    addHeadersToFile(fileName);

    for(long unsigned int i = 0; i < numberOfDecks; i++){
      deckGenerate();       // new deck
      myQuickSort(0, 51);   // deck permutation
      suitSort();           // suit sort each player cards
      createPBN(i, myPBN);          // create myPBN char array
      //showDeck();
      //showPBNString(i, myPBN);

      suitAmount(i, games);
      countPoints(i, games);
    }
    
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

    DDdealsPBN.noOfTables = numberOfDecks;

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
      saveToCsvFile(fileName, static_cast<long unsigned int>(handno), myPBN, games);
    }
    for(long unsigned int i = 0; i < numberOfDecks; i++){
      delete [] myPBN[i];
    }
    delete[] myPBN;
  // options
  }else if(val == validation::NOT_ENAUGH_INFO){
    std::cout << "Options: " << std::endl;
    std::cout << "-a <amount>\t\t amount of games to generate, 0 < number <= 40, obligatory" << std::endl;
    std::cout << "-n <name>\t\t name of destinantion file, optional - default: result.csv" << std::endl;
  
  // less than 1 error
  }else if(val == validation::LESS_THAN_1){
    std::cout << "amount of games to generate must be higher than 0" << std::endl;

  // more than 40 error
  }else if(val == validation::MORE_THAN_40){
    std::cout << "amount of games to generate must be <= 40" << std::endl;

  // invalid argument error
  }else if(val == validation::INVALID_ARGUMENT){
    std::cout << "invalid argument in amount of games" << std::endl;
  }
}

