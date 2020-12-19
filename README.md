# ddsDeckGenerator

### This is library for dds Double Dummy Solver
* [DDS - GitHub](https://github.com/dds-bridge)
* Make sure you don't violate its license [DDS - License](https://github.com/dds-bridge/dds/blob/develop/LICENSE)



### To compile code on Linux:
* Make sure ddl.h and libdds.so are in directory
* Use make or make dds_40/dds_for
* Problems with compiling:
	* boost problems:
		* Make sure you've installed boost/thread.hpp
		* Use *sudo apt install libboost-thread-dev* if not
		* Read more [stackoverflow](https://stackoverflow.com/questions/59561902/boost-thread-hpp-no-such-file-or-directory)
	* cannot open shared object file: No such file or directory:
		* *sudo apt install libdds-dev* helps
		* more [link](https://itsfoss.com/solve-open-shared-object-file-quick-tip/), look at **Alternate method to fix ‘cannot open shared object file’ error** paragraph

### Functions:
* *char getValue(int Value)*
	* exchanges numbers from 0 - 12 to cards values
	* 0 is 2 - 12 is Ace, E as error for other value
* *void myQuickSort(int l, int r)*
	* sorts cards in deck array from smallest random_value to highest
	* l, r - indexes in array
* *void myValuesQuickSort(int l, int r)*
	* sorts cards in deck array from smallest value to highest
	* l, r - indexes in array
* *void mySuitQuickSort(int l, int r)*
	* sorts cards in deck array
	* order: Spades, Hearts, Diamonds, Clubs
	* l, r - indexes in array
* *int suitNumber(char suit)*
	* convert suit to integer
* *int valueNumber(char Value)*
	* convert value to integer
* *void countSuit(int left, int right)
	* counts amount of each suit
	* range <left ; right> - indexes in deck array
	* result in suitTable
* *void suitSort()*
	* sorts cards of each player by suit
	* uses mySuitQuickSort function
* *void showDeck()*
	* shows deck array
* *void deckGenerate()*
	* generates new deck to deck array
* *void addFirstCard()*
	* adds first card of player to myPBN array
* *void addRestCards()*
	* adds rest cards of player to myPBN array
* *void addSpace()*
	* adds space to myPBN array
* *void addLastDots()*
	* adds dots if needed at the and of player cards to myPBN array
* *void createPBN(int position)* in deck_40, *void createPBN()* in deck_for
	* create myPBN char array
	* using addFirstCard(), addRestCards(), addSpace(), addLastDots() functions
	* int position - position in games array
* *void showPBNString(int position)* in deck_40, *void showPBNString()* in deck_for
	* shows myPBN array
	* int position - position in games array
* *void suitAmount(int position)* in deck_40, *void suitAmount()* in deck_for
	* counts number of each suit of each player's cards
	* using "countSuit()" function
	* int position - position in games array
* *int worthOfValue(int value)*
	* returns: 4 - Ace, 3 - King, 2 - Queen, 1 - Jack, 0 - rest
* *void countPoints(int position)* in deck_40, *void countPoints()* in deck_for
	* counts points of each player
	* int position - position in games array
* *void getContracts(ddTableResults * table)*
	* gets contracts to each player
	* uses dds library function
* *void saveToCsvFile(std::string fileName, int position)* in deck_40, *void saveToCsvFile(std::string fileName)* in deck_for
	* saves result to csv file
	* int position - position in games array
* *void addHeadersToFile(std::string fileName)*
	* adds headers to csv file
	* clears all lines in chosen file
* *void showProgress(int current, int step, std::string text)* in deck_for
	* shows progress while making csv
