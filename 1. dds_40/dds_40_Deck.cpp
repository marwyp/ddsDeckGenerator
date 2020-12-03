//============================================================================
// Name        : dds_40_Deck.cpp
// Author      : Marek Wypich
// Version     : V1.0
// License     : GNU GPL
// Description : library for DDS Double Dummy Solver
//============================================================================


#include <iostream>
#include <random>
#include <fstream>
#include "dll.h"


////////////////////////////////////////////////////////////////
////////////         variables and structures      /////////////
////////////////////////////////////////////////////////////////

// amount of decks to genearte, max: 40
const int numberOfDecks = 40;

// PBN string
char myPBN[numberOfDecks][80];

// card structure
struct card{
	char suit;
	char value;
	double random_number;
};

// deck of 52 cards
card deck[52];

// array for amount of each suit in someone's cards
int suitTable[4];

// pointer to myPBN array
char *tablePointer;

// pointer to deck array
int deckPointer;

// contract structure
struct myContractType
{
    int noTrump;
    int spades;
    int hearts;
    int diamonds;
    int clubs;
};

// player information
struct playerStats{
    int spadesAmount;
    int heartsAmount;
    int diamondsAmount;
    int clubsAmount;
    int points;
};

// playerStats for each game
struct game{
    playerStats north;
    playerStats east;
    playerStats south;
    playerStats west;
};

// information about each game
game games[numberOfDecks];

// contracts of one particular game
myContractType north, east, south, west;


////////////////////////////////////////////////////////////////
///////////////////        functions      //////////////////////
////////////////////////////////////////////////////////////////

// exchanges numbers 0 - 12 to card values
char getValue(int value){
  switch(value){
    case 0:{
      return '2';
      break;
    }
    case 1:{
      return '3';
      break;
    }
    case 2:{
      return '4';
      break;
    }
    case 3:{
      return '5';
      break;
    }
    case 4:{
      return '6';
      break;
    }
    case 5:{
      return '7';
      break;
    }
    case 6:{
      return '8';
      break;
    }
    case 7:{
      return '9';
      break;
    }
    case 8:{
      return 'T';
      break;
    }
    case 9:{
      return 'J';
      break;
    }
    case 10:{
      return 'Q';
      break;
    }
    case 11:{
      return 'K';
      break;
    }
    case 12:{
      return 'A';
      break;
    }
    default:{
      return 'E';
    }
  }
}

// my quicksort implementation for deck permutations, l - left, r - right
void myQuickSort(int l, int r)  // l - left, r - right
{
    int i, j;
    card m;          // m - middle
    i = l;
    j = r;
    m = deck[(l + r) / 2];
    while (i < j) {
        while (deck[i].random_number < m.random_number) {
            i++;
        }
        while (deck[j].random_number > m.random_number) {
            j--;
        }
        if (i <= j) {
            std::swap(deck[i], deck[j]);
            i++;
            j--;
        }
    }
    if (i < r) {
        myQuickSort(i, r);
    }
    if (j > l) {
        myQuickSort(l, j);
    }
}

// suit to integer
int suitNumber(char suit){
    switch (suit)
    {
        case 'S':{
            return 1;
            break;
        }
        case 'H':{
            return 2;
            break;
        }
        case 'D':{
            return 3;
            break;
        }
        case 'C':{
            return 4;
            break;
        }
        default:{
            return 54237;
            break;
        }
    }
}

// value to integer
int valueNumber(char value){
    switch (value)
    {
        case '2':{
            return 14;
            break;
        }
        case '3':{
            return 13;
            break;
        }
        case '4':{
            return 12;
            break;
        }
        case '5':{
            return 11;
            break;
        }
        case '6':{
            return 10;
            break;
        }
        case '7':{
            return 9;
            break;
        }
        case '8':{
            return 8;
            break;
        }
        case '9':{
            return 7;
            break;
        }
        case 'T':{
            return 6;
            break;
        }
        case 'J':{
            return 5;
            break;
        }
        case 'Q':{
            return 4;
            break;
        }
        case 'K':{
            return 3;
            break;
        }
        case 'A':{
            return 2;
            break;
        }
        default:{
            return 0;
        }
    }
}


// my quicksort implementation for values, l - left, r - right
void myValuesQuickSort(int l, int r)
{
    int i, j; 
    card m;     // m - middle
    i = l;
    j = r;
    m = deck[(l + r) / 2];
    while (i < j) {
        while (valueNumber(deck[i].value) < valueNumber(m.value)) {
            i++;
        }
        while (valueNumber(deck[j].value) > valueNumber(m.value)) {
            j--;
        }
        if (i <= j) {
            std::swap(deck[i], deck[j]);
            i++;
            j--;
        }
    }
    if (i < r) {
        myValuesQuickSort(i, r);
    }
    if (j > l) {
        myValuesQuickSort(l, j);
    }
}

// my quicksort implementation for suit, l - left, r - right
void mySuitQuickSort(int l, int r)
{
    int i, j; 
    card m;     // m - middle
    i = l;
    j = r;
    m = deck[(l + r) / 2];
    while (i < j) {
        while (suitNumber(deck[i].suit) < suitNumber(m.suit)) {
            i++;
        }
        while (suitNumber(deck[j].suit) > suitNumber(m.suit)) {
            j--;
        }
        if (i <= j) {
            std::swap(deck[i], deck[j]);
            i++;
            j--;
        }
    }
    if (i < r) {
        mySuitQuickSort(i, r);
    }
    if (j > l) {
        mySuitQuickSort(l, j);
    }
}

// counts amount of each suit - range <left ; right>, result in suitTable
void countSuit(int left, int right){
    for(int i = 0; i < 4; i++){
        suitTable[i] = 0;
    }

    for(int i = left; i <= right; i++){
        if(deck[i].suit == 'S'){
            suitTable[0]++;
        }else if(deck[i].suit == 'H'){
            suitTable[1]++;
        }
        else if(deck[i].suit == 'D'){
            suitTable[2]++;
        }else{
            suitTable[3]++;
        }

    }
}

// suit sort - Spades, Hearts, Diamonds, Clubs
void suitSort(){

    // north
    mySuitQuickSort(0, 12);

    // east
    mySuitQuickSort(13, 25);

    // south
    mySuitQuickSort(26, 38);

    // west
    mySuitQuickSort(39, 51);
}

// shows deck - to check functions
void showDeck(){
    for(int i = 0; i < 52; i++){
        if(i%13 == 0){
            std::cout << std::endl;
        }
        std::cout << deck[i].random_number << "\t\t";
        std::cout << deck[i].suit << "\t\t";
        std::cout << deck[i].value << std::endl;
    }
}

// generate new deck to deck array
void deckGenerate(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);

    for(int i = 0; i<13; i++){

        // current value of card which will be added to each suit
        char currentValue = getValue(i);

        // spades
        deck[i].suit = 'S';
        deck[i].value = currentValue;
        deck[i].random_number = dis(gen);
        //std::cout<< deck[i].random_number <<std::endl;

        // hearts
        deck[i+13].suit = 'H';
        deck[i+13].value = currentValue;
        deck[i+13].random_number = dis(gen);
        //std::cout<< deck[i+13].random_number <<std::endl;

        // diamonds
        deck[i+26].suit = 'D';
        deck[i+26].value = currentValue;
        deck[i+26].random_number = dis(gen);
        //std::cout<< deck[i+26].random_number <<std::endl;

        // clubs
        deck[i+39].suit = 'C';
        deck[i+39].value = currentValue;
        deck[i+39].random_number = dis(gen);
        //std::cout<< deck[i+39].random_number <<std::endl;
    }
}

// adds first card of player to myPBN array
// checks for the existence of suit, if it doesn't exist, adds dot
void addFirstCard(){
    if(deck[deckPointer].suit == 'S'){
      *tablePointer = deck[deckPointer].value;
      tablePointer++;
      deckPointer++;
    }else{
        *tablePointer = '.';
        tablePointer++;
        if(deck[deckPointer].suit == 'H'){
            *tablePointer = deck[deckPointer].value;
            tablePointer++;
            deckPointer++;
        }else{
            *tablePointer = '.';
            tablePointer++;
            if(deck[deckPointer].suit == 'D'){
                *tablePointer = deck[deckPointer].value;
                tablePointer++;
                deckPointer++;
            }else{
                *tablePointer = '.';
                tablePointer++;
                *tablePointer = deck[deckPointer].value;
                tablePointer++;
                deckPointer++;
            }
        }
    }
}

// adds rest cards of player to myPBN array
void addRestCards(){
    int thirteenPointer = 1;
    while(thirteenPointer != 13){
      if(suitNumber(deck[deckPointer - 1].suit) == suitNumber(deck[deckPointer].suit)){
          *tablePointer = deck[deckPointer].value;
          tablePointer++;
          deckPointer++;
          thirteenPointer++;
      }else{
          int difference = suitNumber(deck[deckPointer].suit) - suitNumber(deck[deckPointer - 1].suit);
          for(int i = 0; i < difference; i++){
              *tablePointer = '.';
              tablePointer++;
          }
          *tablePointer = deck[deckPointer].value;
          tablePointer++;
          deckPointer++;
          thirteenPointer++;
      }
    }
}

//adds space to myPBN array
void addSpace(){
    *tablePointer = ' ';
    tablePointer++;
}

// adds dots if needed at the and of player cards to myPBN array
void addLastDots(){
    int lastSuitNumber = suitNumber(deck[deckPointer - 1].suit);
    for(int i = 0; i < 4 - lastSuitNumber; i++){
        *tablePointer = '.';
        tablePointer++;
    }
}

// create myPBN char array, using addFirstCard(), addRestCards(), addSpace(), addLastDots() functions
void createPBN(int position){
  // sets deck pointer to 0
  deckPointer = 0;

  // the begining of myPBN array
  myPBN[position][0] = 'N';
  myPBN[position][1] = ':';
  tablePointer = &myPBN[position][0];
  //std::cout << *tablePointer << std::endl;
  tablePointer+=2;

  // adds cards of north player to myPBN array
  addFirstCard();
  addRestCards();
  addLastDots();
  addSpace();

  // adds cards of east player to myPBN array
  addFirstCard();
  addRestCards();
  addLastDots();
  addSpace();

  // adds cards of south player to myPBN array
  addFirstCard();
  addRestCards();
  addLastDots();
  addSpace();

  // adds cards of west player to myPBN array
  addFirstCard();
  addRestCards();
  addLastDots();

}

void showPBNString(int position){
    printf("%s\n", myPBN[position]);
}


////////////////////////////////////////////////////////////////
//////////////////       myDeckAnalyser      ///////////////////
////////////////////////////////////////////////////////////////


// counts number of each suit of each player's cards, using "countSuit()" function
void suitAmount(int position){

    // counts number of each suit in north cards
    countSuit(0, 12);
    games[position].north.spadesAmount = suitTable[0];
    games[position].north.heartsAmount = suitTable[1];
    games[position].north.diamondsAmount = suitTable[2];
    games[position].north.clubsAmount = suitTable[3];

    // counts number of each suit in east cards
    countSuit(13, 25);
    games[position].east.spadesAmount = suitTable[0];
    games[position].east.heartsAmount = suitTable[1];
    games[position].east.diamondsAmount = suitTable[2];
    games[position].east.clubsAmount = suitTable[3];

    // counts number of each suit in south cards
    countSuit(26, 38);
    games[position].south.spadesAmount = suitTable[0];
    games[position].south.heartsAmount = suitTable[1];
    games[position].south.diamondsAmount = suitTable[2];
    games[position].south.clubsAmount = suitTable[3];

    // counts number of each suit in west cards
    countSuit(39, 51);
    games[position].west.spadesAmount = suitTable[0];
    games[position].west.heartsAmount = suitTable[1];
    games[position].west.diamondsAmount = suitTable[2];
    games[position].west.clubsAmount = suitTable[3];
}

// returns: 4 - Ace, 3 - King, 2 - Queen, 1 - Jack, 0 - rest
int worthOfValue(int value){
    switch (value){
        case 'A':{
            return 4;
            break;
        }
        case 'K':{
            return 3;
            break;
        }
        case 'Q':{
            return 2;
            break;
        }
        case 'J':{
            return 1;
            break;
        }
        default:{
            return 0;
            break;
        }
    }
}

// counts points of given player
void countPlayerPoints(int left, int right, playerStats *player){
    int points = 0;
    for(int i = left; i <= right; i++){
        points += worthOfValue(deck[i].value);
    }
    player -> points = points;
}

// counts points of each player
void countPoints(int position){
    countPlayerPoints(0, 12, &(games[position].north));
    countPlayerPoints(13, 25, &(games[position].east));
    countPlayerPoints(26, 38, &(games[position].south));
    countPlayerPoints(39, 51, &(games[position].west));
}

// gets contracts to each player
void getContracts(ddTableResults * table){
    north.noTrump = table->resTable[4][0];
    north.spades = table->resTable[0][0];
    north.hearts = table->resTable[1][0];
    north.diamonds = table->resTable[2][0];
    north.clubs = table->resTable[3][0];

    south.noTrump = table->resTable[4][2];
    south.spades = table->resTable[0][2];
    south.hearts = table->resTable[1][2];
    south.diamonds = table->resTable[2][2];
    south.clubs = table->resTable[3][2];

    east.noTrump = table->resTable[4][1];
    east.spades = table->resTable[0][1];
    east.hearts = table->resTable[1][1];
    east.diamonds = table->resTable[2][1];
    east.clubs = table->resTable[3][1];

    west.noTrump = table->resTable[4][3];
    west.spades = table->resTable[0][3];
    west.hearts = table->resTable[1][3];
    west.diamonds = table->resTable[2][3];
    west.clubs = table->resTable[3][3];
}
// adds headers to csv file
void addHeadersToFile(std::string fileName){
    std::ofstream file;
    file.open(fileName.c_str());

    file << "Cards;";
    file << "Number NS;Number NH;Number ND;Number NC;Points N;";
    file << "Number ES;Number EH;Number ED;Number EC;Points E;";
    file << "Number SS;Number SH;Number SD;Number SC;Points S;";
    file << "Number WS;Number WH;Number WD;Number WC;Points W;";
    file << "NTN;NTS;NTE;NTW;";
    file << "SN;SS;SE;SW;";
    file << "HN;HS;HE;HW;";
    file << "DN;DS;DE;DW;";
    file << "CN;CS;CE;CW;";
    file << "\n";


    file.close();
}

// saves result to csv file
void saveToCsvFile(std::string fileName, int position){
    std::ofstream file;
    file.open(fileName.c_str(), std::ios::app);

    file << "\"" << myPBN[position] << "\";";


    file << games[position].north.spadesAmount << ";" << games[position].north.heartsAmount << ";" << games[position].north.diamondsAmount;
    file << ";" << games[position].north.clubsAmount << ";" << games[position].north.points << ";";

    file << games[position].east.spadesAmount << ";" << games[position].east.heartsAmount << ";" << games[position].east.diamondsAmount;
    file << ";" << games[position].east.clubsAmount << ";" << games[position].east.points << ";";

    file << games[position].south.spadesAmount << ";" << games[position].south.heartsAmount << ";" << games[position].south.diamondsAmount;
    file << ";" << games[position].south.clubsAmount << ";" << games[position].south.points << ";";

    file << games[position].west.spadesAmount << ";" << games[position].west.heartsAmount << ";" << games[position].west.diamondsAmount;
    file << ";" << games[position].west.clubsAmount << ";" << games[position].west.points << ";";


    file << north.noTrump << ";" << south.noTrump << ";" << east.noTrump << ";" << west.noTrump << ";";
    file << north.spades << ";" << south.spades << ";" << east.spades << ";" << west.spades << ";";
    file << north.hearts << ";" << south.hearts << ";" << east.hearts << ";" << west.hearts << ";";
    file << north.diamonds << ";" << south.diamonds << ";" << east.diamonds << ";" << west.diamonds << ";";
    file << north.clubs << ";" << south.clubs << ";" << east.clubs << ";" << west.clubs << ";";
    file << "\n";

    file.close();
}