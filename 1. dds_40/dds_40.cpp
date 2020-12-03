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

#include "dll.h"
#include "dds_40_Deck.h"

int main()
{
  const int  numberOfDecks = 40;
  extern char myPBN[numberOfDecks][80];

  addHeadersToFile("wynik.csv");

  for(int i = 0; i < numberOfDecks; i++){
    deckGenerate();       // new deck
    myQuickSort(0, 51);   // deck permutation
    suitSort();           // suit sort each player cards
    createPBN(i);          // create myPBN char array
    //showDeck();
    //showPBNString(i);

    suitAmount(i);
    countPoints(i);
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
    saveToCsvFile("wynik.csv", handno);
  }
  
}

