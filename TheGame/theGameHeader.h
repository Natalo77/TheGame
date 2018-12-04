#pragma once
#include <string>

//Header file for the game
//This contains declaration of function signatures, structs and enums


//Enums
enum PlayerType
{
	HUMAN = 0,
	AI = 1
};

enum ProcessCalledFrom
{
	USERINPUT = 0,
	AUTOMATED = 1
};


//Structs
struct Deck {
	int* cards;	//an array of cards 
	int deckSize;	//the size of the deck
	int cntTop;	//the index to the top card;
};

struct PlayPile {
	int isAscending; //true = ASCENDING false = DESCENDING pile
	int pileSize; //the current size of the pile
	int cards[99]; //The array for the values of cards in the play pile.
	int topCardIndex; //The top card in the pile.
};

struct Player {
	int cards[8];	//player's hand
	int maxHandSize; //the maximum hand size of a player
	int handSize;	//the size of the current hand
	std::string name;    //Player's name
	PlayerType playerType; //0 = human, 1 = AI
};

struct TheGame {
	Deck drawDeck;		//the drawdeck
	PlayPile playPiles[4];	//4 play piles
	Player players[5];	//maximum 5 players
	int numPlayers;	//number of players
	int turns;			//the number of turns
};


//Swapping and sorting.
void swapInt(int* a, int* b);

void insertionSortIntArray(int* array, int size);


//Deck related functions
void initialiseDeck(Deck* deck, int deckSize);

void displayDeck(const Deck* deck);

bool isThereAValidPlay(TheGame* game, Player* player);

void cleanupDeck(Deck* deck);

void shuffleDeck(Deck* deck, int times);


//Play pile related functions
void initialisePlayPile(PlayPile* pile, bool ascending);


//Player related functions
void initialisePlayer(Player* player, int handSize, Deck* deck, std::string name, PlayerType playerType);


//Game related functions
TheGame* createNewGame(int numPlayers);

int getHandSize(int numPlayers);

void initialiseGame(TheGame* game, int numPlayers);

void displayGame(const TheGame* game);

void cleanupGame(TheGame* game);


//Player input functions.
std::string getPlayerName(int playerNumIndex);

int getPlayerCardChoice(Player* player, int numCardsPlayed);

int getPlayerPileChoice(Player* player, int whichCard);

PlayerType getPlayerType(int playerNumIndex);

int getNumPlayersInGame();

char getPlayerNewGameChoice();


//Player turn functions.
void displayPlayerHand(Player* player, int playerNum);

bool playerPlayCardToPile(Player* player, int whichCard, PlayPile* pile, ProcessCalledFrom calledFrom);

bool isPlayValid(int card, PlayPile* pile, ProcessCalledFrom calledFrom);

void displayRules();

int playerDrawCardsFromDeck(Player* player, Deck* deck, int maxHandSize, int numToDraw);


//AI Functions
bool aiCheckAndPlayReverser(TheGame* game, Player* player);

void aiPlayBestValueCard(TheGame* game, Player* player);

void aiPlayCard(TheGame* game, Player* player);


//Game loop related functions.
int playerProcessTurn(TheGame* game, Player* player);

int getNumberOfEmptySets(TheGame* game);

bool isGameEnded(TheGame* game, int numCardsPlayed);

int calculateGameScore(const TheGame* game);

char gameOverScoreAndNewGame(TheGame* game);








