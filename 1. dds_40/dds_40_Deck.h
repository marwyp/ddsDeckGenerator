#pragma once
#include <iostream>

////////////////////////////////////////////////////////////////
////////////////       myDeckGenerator      ////////////////////
////////////////////////////////////////////////////////////////


// exchanges numbers 0 - 12 to card values
char getValue(int value);

// my quicksort implementation for deck permutations, l - left, r - right
void myQuickSort(int l, int r);

// my quicksort implementations for values, l - left, r - right
void myValuesQuickSort(int l, int r);

// my quicksort implementations for suit, l - left, r - right
void mySuitQuickSort(int l, int r);

// suit to integer
int suitNumber(char suit);

// value to integer
int valueNumber(char value);

// counts amount of each suit - range <left ; right>, result in suitTable
void countSuit(int left, int right);

// suit sort - Spades, Hearts, Diamonds, Clubs
void suitSort();

// shows deck - to check functions
void showDeck();

// generate new deck to deck array
void deckGenerate();

// adds first card of player to myPBN array
void addFirstCard();

// adds rest cards of player to myPBN array
void addRestCards();

//adds space to myPBN array
void addSpace();

// adds dots if needed at the and of player cards to myPBN array
void addLastDots();

// create myPBN char array, using addFirstCard(), addRestCards(), addSpace(), addLastDots() functions
void createPBN(int position);

// shows myPBN array
void showPBNString(int position);




////////////////////////////////////////////////////////////////
//////////////////       myDeckAnalyser      ///////////////////
////////////////////////////////////////////////////////////////



// counts number of each suit of each player's cards, using "countPlayerPoints()" function
void suitAmount(int position);

// returns: 4 - Ace, 3 - King, 2 - Queen, 1 - Jack, 0 - rest
int worthOfValue(int value);

// counts points of each player
void countPoints(int position);

// gets contracts to each player
void getContracts(ddTableResults * table);

// saves result to csv file
void saveToCsvFile(std::string fileName, int position);

// adds headers to csv file
void addHeadersToFile(std::string fileName);