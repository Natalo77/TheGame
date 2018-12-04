#include <stdlib.h>
#include <iostream>
#include <limits>
#include <ctime>
#include <algorithm>
#include "theGameHeader.h"
using namespace std;

void swapInt(int* a, int* b) 
{
	//N.B. You must make sure *a and *b are not pointing to the same address
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

void insertionSortIntArray(int* array, int size) 
{
	/*
		Works by going up an array and moving each element smaller than
		the previous to its correct place by moving it backwards and checking again.
	*/

	int z;
	for (int x = 0; x < size; x++) 
	{
		//for each element in the array to be sorted.
		z = x;
		while (z > 0 && array[z] < array[z - 1])  //if (it is not the first element) and (the element directly previous is smaller)
		{	
			//swap them.
			swapInt(array + z, array + z - 1);
			z--;
			//then decrement the array element being looked at 
		}
	}
}

void initialiseDeck(Deck* deck, int deckSize) 
{
	/*
		Initialise a deck array for the game to use with its initial 2..99 values and then shuffle it
	*/
	deck->cards = new int[deckSize];	//create a new int array of deckSize for the value of deck.cards
	deck->deckSize = deckSize;			//set the size of deck.deckSize to deckSize
	deck->cntTop = 0;					//set the position for the top of the deck to 0
	
	//for each position in the array
	for (int i = 0; i < deckSize; i++) 
	{
		//Give each position its initial value
		deck->cards[i] = i + 2;
	}
	//Shuffle the deck 1000 times.
	shuffleDeck(deck, 1000);
}

void displayDeck(const Deck* deck) 
{
	/*
		Display the current deck status:
		"Current deck size: [deckSize]
	*/
	cout << "Current deck size: " << deck->deckSize << endl;
}

bool isThereAValidPlay(TheGame* game, Player* player)
{
	/*
		Check to see if the current player has a valid play.
		For each card in the player's hand:
			Check it to each play pile.
			If it can be played there, return true.
		If the end of the function is reached then no card can be played so return false.
	*/
	for (int card = 0; card < player->handSize; card++)
	{
		for (int pile = 0; pile < 4; pile++)
		{
			if (isPlayValid(player->cards[card], &game->playPiles[pile], AUTOMATED)) { return true; }
		}
	}
	return false;
}

void cleanupDeck(Deck* deck) 
{
	/*
		free the memory taken by the deck's cards pointer.
	*/
	delete[] deck->cards;
}

void shuffleDeck(Deck* deck, int times) 
{
	for (int x = 0; x < times; x++)
	{
		/*
			For each shuffle, choose two random places within
			the array until the second random place does not
			equal the first.
			Then swap the values of those two positions within the array.
		*/
		int randomPlace = rand() % 98;
		int randomPlace2;
		do 
		{
			randomPlace2 = rand() % 98;
		} while (randomPlace == randomPlace2);
		//Use swap int with the pointers to the positions to swap the values.
		swapInt(deck->cards + randomPlace, deck->cards + randomPlace2);
	}
}

void initialisePlayPile(PlayPile* pile, bool ascending) {
	/*
		set the pile's size to 0, it's top card (index 0) depending on whether or not it needs to be ascending,
		set it's value of ascending and set the top card's index position to 0
	*/
	pile->pileSize = 0;
	pile->isAscending = ascending;
	pile->cards[0] = ascending ? 1 : 100;
	pile->topCardIndex = 0;
}

void initialisePlayer(Player* player, int handSize, Deck* deck, string name, PlayerType playerType)
{
	/*
		Set the values for a player struct.
		Draw them an initial hand based on a predetermined value. (see initialise game)
		Display the number of cards the player drew from the deck.
	*/
	player->maxHandSize = handSize;		//The maximum hand size at any point for the player.
	player->handSize = 0;				//The player's current hand size.
	player->name = name;				//The player's name.
	player->playerType = playerType;	//The players type. (see enum PlayerType in theGameHeader.h)

	int ncards = playerDrawCardsFromDeck(player, deck, handSize, handSize);
	cout << "Player has drawn " << ncards << " from the deck.\n";
}

TheGame* createNewGame(int numPlayers) 
{
	/*
		create a new game pointer and initialise it with the specified number of players.
	*/
	TheGame* game = new TheGame();
	initialiseGame(game, numPlayers);
	return game;
}

int getHandSize(int numPlayers)
{
	/*
		  1 Player  = 8 cards per hand.
		  2 Players = 7 cards per hand.
	    3-5 Players = 6 cards per hand.
	*/
	switch (numPlayers)
	{
	case 1: return 8; break;
	case 2: return 7; break;
	case 3: 
	case 4:
	case 5: return 6; break;
	}
}

void initialiseGame(TheGame* game, int numPlayers)
{
	/*
		Perform each step for starting a game:
		1. create a new random number sequence.
		2. Initialise the deck.
		3. Initialise each play pile.
		4. Initialise each player.
	*/

	/*
		1.
		create a new random seed.
	*/
	srand(time(NULL));

	/*
		2.
		initialise the deck with the location of the draw deck and a size of 98.
		set the number of players and set the turns to 0;
	*/
	initialiseDeck(&(game->drawDeck), 98);
	game->numPlayers = numPlayers;
	game->turns = 0;

	/*
	3.
		initialise each play pile.
	*/
	initialisePlayPile(&(game->playPiles[0]), true);
	initialisePlayPile(&(game->playPiles[1]), true);
	initialisePlayPile(&(game->playPiles[2]), false);
	initialisePlayPile(&(game->playPiles[3]), false);

	/*
	4.
		initialise the number of players specified using:
			each array position in the location of the game's players array,
			a max hand size determined from the number of players,
			the location of the game's draw deck. (To draw cards from),
			the name of each player,
			the type of each player.
	*/
	string name;
	PlayerType playerType;
	int handsize = getHandSize(numPlayers);
	for (int i = 0; i < numPlayers; i++)
	{
		//For each player, get a name, a player type, and initialize them with other determined values.
		name = getPlayerName(i);
		playerType = getPlayerType(i);
		initialisePlayer(&(game->players[i]), handsize, &(game->drawDeck), name, playerType);
	}
	//Once done, state that the game is ready to play
	cout << "Game loaded:" << endl;
	system("PAUSE");
}

void displayGame(const TheGame* game)
{
	/*
		Clear the screen, then display the current deck and playpile states.
	*/
	std::system("CLS");
	displayDeck(&(game->drawDeck));
	printf("Play Piles:\n1:A[%d], 2:A[%d], 3:D[%d], 4:D[%d]\n",
		game->playPiles[0].cards[game->playPiles[0].topCardIndex],
		game->playPiles[1].cards[game->playPiles[1].topCardIndex],
		game->playPiles[2].cards[game->playPiles[2].topCardIndex],
		game->playPiles[3].cards[game->playPiles[3].topCardIndex]);
}

void cleanupGame(TheGame* game)
{
	/*
		call the cleanupDeck function with the location of the game's draw deck.
	*/
	cleanupDeck(&(game->drawDeck));
	delete game;
}

string getPlayerName(int playerNumIndex)
{
	/*
		Get and return a string
		Conditions:
			string cannot be ""
	*/
	string name;
	cout << "What is the name of Player " << playerNumIndex + 1 << "?" << endl;
	cin >> name;
	while (1)
	{
		if (cin.fail() || name == "")
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error: Please enter a name." << endl;
			cin >> name;
		}
		else if (!cin.fail()) { break; }
	}
	return name;
}

int getPlayerCardChoice(Player* player, int numCardsPlayed)
{
	/*
		Get and return an integer for the display-index (index + 1) of the card the player wishes to play.
		Conditions:
			Must be >= 0 and <= player's handsize.
			0 is only allowed when >=2 cards have been played.
			0 is the only allowed input when the hand is empty. 
	*/
	int whichCard;
	if (player->handSize != 0) { cout << "Please choose a card (1 - " << player->handSize << ") to play:" << endl; }
	//If the player has played 2 cards already or their hand is empty, notify them that they can end their turn.
	if (numCardsPlayed >= 2 || player->handSize == 0) { cout << "If you would like to end your turn, type 0\n" << endl; }
	cin >> whichCard;
	while (1)
	{

		if (cin.fail() || whichCard < 0 || whichCard > player->handSize || (whichCard == 0 && numCardsPlayed < 2))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (whichCard == 0) { cout << "You may only end your turn when you have played two cards, you have played " << numCardsPlayed << "." << endl; }
			else if (player->handSize != 0) { cout << "Error: Enter a number between 1 and " << player->handSize << "." << endl; }
			else { cout << "Error: type 0 to end your turn." << endl; }
			cin >> whichCard;
		}
		else if (!cin.fail()) { break; }
	}
	return whichCard;
}

int getPlayerPileChoice(Player* player, int whichCard)
{
	/*
		Get and return an integer for the display-index (index + 1) of the pile the player wishes to play to.
		Conditions:
			Must be >= 0 and <= 4.
	*/
	int whichPile;
	cout << "Please choose a play pile to place " << player->cards[whichCard - 1] << " into:" << endl;
	cout << "If you would like to cancel, type 0" << endl;

	cin >> whichPile;
	while (1)
	{
		if (cin.fail() || whichPile < 0 || whichPile > 4)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error: Enter a number between 0 and 4." << endl;
			cin >> whichPile;
		}
		else if (!cin.fail()) { break; }
	}
	return whichPile;
}

PlayerType getPlayerType(int playerNumIndex)
{
	/*
		Get and return an enum PlayerType for player type.
		Conditions:
			Data entry must be 1 or 0
	*/
	PlayerType playerType;
	int playerTypeInput;
	cout << "Is this player a human or an AI? (0 = human, 1 = AI)" << endl;
	cin >> playerTypeInput;
	while (1)
	{
		if (cin.fail() || playerTypeInput < 0 || playerTypeInput > 1)
		{
			cin.clear();
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
			cout << "Error: Please enter 0 for human or 1 for AI" << endl;
			cin >> playerTypeInput;
		}
		else if (!cin.fail()) { break; }
	}
	//conver the integer input to an enum(PlayerType) datatype to return
	playerType = PlayerType(playerTypeInput);
	return playerType;
}

char getPlayerNewGameChoice()
{
	/*
		Get and return a character for if the player would like to start a new game or not.
		Conditions:
			Must be either y or n, any case.
	*/
	char newGame;
	cout << "Would you like to play again? (Y/N)" << endl;
	cin >> newGame;
	while (1)
	{
		if (cin.fail() || (newGame != 'y' && newGame != 'Y' && newGame != 'n' && newGame != 'N'))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error: Please enter 'y' or 'n'." << endl;
			cin >> newGame;
		}
		else if (!cin.fail()) { break; }
	}
	return newGame;
}

int getNumPlayersInGame()
{
	/*
		Get and return an integer for the number of players in the new game.
		conditions:
			must be >= 1 and <= 5
	*/
	int numPlayers;	//The number of players the player would like for the game.
	cout << "How many players are playing this game? (1-5)" << endl;
	cin >> numPlayers;
	while (1)
	{
		if (cin.fail() || numPlayers < 1 || numPlayers > 5)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error: Please enter a number between 1 and 5." << endl;
			cin >> numPlayers;
		}
		else if (!cin.fail()) { break; }
	}
	return numPlayers;
}

void displayPlayerHand(Player* player, int playerNum)
{
	/*
		Sort the array of cards.
		Display the player name who's hand is about to be shown, then wait for an input.
			The input ensures that another players hand is not shown to the wrong person.
		then, for each card in the players hand, display it in the format: (omit curved brackets)
		"(card number):[(card value)] "
	*/
	int numCards = player->handSize;	//The number of cards in the player's hand.
	int* cards = player->cards;			//The address of the player's hand-of-cards array

	insertionSortIntArray(cards, player->handSize);
	cout << "Player " << playerNum + 1 << ": " << player->name << endl;
	system("PAUSE");

	for (int i = 0; i < numCards; i++) {
		cout << i + 1 << ":[" << *(cards + i) << "] ";
	}
	cout << endl;
}

bool playerPlayCardToPile(Player* player, int whichCard, PlayPile* pile, ProcessCalledFrom calledFrom)
{
	/*
		If this was called from an AUTOMATED process, then it was an AI that called it:
			This means the card has already been checked for validity so the check doesn't need to be ran a second time.
		If this wasn't called from an AUTOMATED process, then it was called by USERINPUT, so the validity needs to be checked.
			so check if the specified card value and pile is an acceptable play
		If either are true:
			Set the value of the top card of the specified pile to the value of the card being played.
			Increment the pile size.
			Set the value of the played card position to 100, to indicate it has been played.
			Sort the player hand array so that the newly created 100 filters to the end of the array.
			Decrement the player's hand size as a card has just been played.
			Return true that the card was played.
		If the if is not called, then the card is not played, so return false that the card was not played.
	*/
	//calledFrom is checked first so that isPlayValid is only called when a human player attempted the play.
	if (calledFrom == AUTOMATED || isPlayValid(player->cards[whichCard], pile, calledFrom))
	{
		//Dealing with the card play.
		int cardNum = player->cards[whichCard];
		pile->topCardIndex++;
		pile->cards[pile->topCardIndex] = cardNum;
		pile->pileSize++;
		player->cards[whichCard] = 100;
		//sort the array so the 100 value is filtered to the end.
		insertionSortIntArray(player->cards, player->handSize);
		player->handSize--;
		return true;
	}
	return false;
}

bool isPlayValid(int card, PlayPile* pile, ProcessCalledFrom calledFrom)
{
	/*
		Check if a given card can be played to a given pile.
		If the card to be played meets any of the three conditions specified in the game rules:
			return true.
		If it is not valid, check if this function was called as a result of a user input.
			If it was, provide the option of a reminder of the game rules.
			If not, skip over that as it will have been an automated checker that called the function.
		If the end of the function is reached, the play is not valid so return false.
	*/
	bool isAscending = pile->isAscending;			//Is the pile the attempt is being made to ascending?
	int topCard = pile->cards[pile->topCardIndex];	//The top value of the pile for the attempt.

	if ((abs(topCard - card) == 10) || (isAscending && card > topCard) || (!isAscending && card < topCard)) { return true; }
	else if (calledFrom == USERINPUT) { displayRules(); }

	return false;
}

void displayRules()
{
	/*
		Ask if the player would like to view the card placement rules.
		If yes display them.
	*/
	char choice;
	cout << "Invalid placement, would you like to see the rules for placement? (Y/N)\n" << endl;
	cin >> choice;
	while (1)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error: Please enter 'y' or 'n'," << endl;
		}
		if (!cin.fail()) { break; }
	}
	if (choice == 'Y' || choice == 'y')
	{
		cout << "Rules for placement:\n" << "1. If pile is ascending, played-card must be greater than the current top card.\n"
			<< "2. If pile is descending, played-card must be less than the current top card.\n"
			<< "3. Rules 1 and 2 can be ignored if the difference between the played-card and current top card is exactly 10.\n";
		system("PAUSE");
	}
}

int playerDrawCardsFromDeck(Player* player, Deck* deck, int maxHandSize, int numToDraw)
{
	/*
		If the deck is not empty then
			working backwards through the player's hand
			Replace the specified number of cards with new cards from the deck.
			each time a card is drawn, incremenent numCardDrawn.
			Then return the number of cards drawn at the end of operation.
	*/
	int numCardDrawn = 0;
	if (deck->cntTop < 97)
	{
		for (int x = player->maxHandSize - 1; x > player->maxHandSize - 1 - numToDraw; x--)
		{
			//Deal with drawing a card to the player's hand
			player->cards[x] = deck->cards[deck->cntTop];
			deck->cntTop += 1;
			numCardDrawn++;
			deck->deckSize--;
		}
	}
	player->handSize += numCardDrawn;
	return numCardDrawn;
}

bool aiCheckAndPlayReverser(TheGame* game, Player* player)
{
	/*
	For each card in the ai's hand:
		For each pile:
			If it's a reverser and can be played.
			break out of the for loop.
		if a reverser was found, play it and break out of the for loop.
	Return the result of whether or not a reverser was found and played.
	*/

	int cardIndex;			//The index of the card being compared to the piles.
	int cardValue;			//The value of the card being compared to the piles.
	PlayPile* playPile;		//The pile being checked.
	int topCardIndex;		//The index of the top card of the pile being checked.
	int topCard;			//The value of the top card of the pile being checked.
	bool pileIsAscending;	//Is the pile being checked ascending?
	bool reverser = false;	//Has a reverser been found?

	//For each card in the ai hand.
	for (int card = 0; card < player->handSize; card++)
	{
		cardIndex = card;
		cardValue = player->cards[cardIndex];
		//For each pile.
		for (int pile = 0; pile < 4; pile++)
		{
			topCardIndex = game->playPiles[pile].topCardIndex;
			topCard = game->playPiles[pile].cards[topCardIndex];
			pileIsAscending = game->playPiles[pile].isAscending;
			//If it's a reverser and can be played.
			if ((cardValue == topCard - 10 && pileIsAscending) || (cardValue == topCard + 10 && !pileIsAscending))
			{
				//get the address of the playpile to play it to and break.
				reverser = true;
				playPile = &(game->playPiles[pile]);
				break;
			}
		}
		//If it was a reverser and could be played, play it here.
		if (reverser == true) { playerPlayCardToPile(player, cardIndex, playPile, AUTOMATED); break; }
	}
	return reverser;
}

void aiPlayBestValueCard(TheGame* game, Player* player)
{
	/*
		AI thinking process for best value:
		For each card.
			For each pile.
				if it can be played, calculate the proposed difference.
					if it can't - set it to 101 (lowest value to never be considered)
				if the proposed difference is equal to the current difference.
					Check both the current best card and the new best card against each card in the hand.
						Store whether any of them are reversers (10 difference between the two) for any card in the hand.
					If the current best card is a reverser for a card in the hand and the new card is not.
						Set the current best card to the new card.
					If they are both reversers for a card in the hand.
						Flip a coin. 50% chance to set the current best card to the new card.
				if the proposed difference is less than the current difference.
					Set the current best card to the new card.
		Once every card has been checked, the current best card is played.
	*/

	int cardIndex;			//The index of the card being compared to the piles' top card.
	int cardValue;			//The value of the card being compared to the piles' top card.
	PlayPile* playPile;		//The pile being checked.
	int topCardIndex;		//The index of the top card of the pile being checked.
	int topCard;			//The value of the top card of the pile being checked.
	bool pileIsAscending;	//Is the pile being checked ascending?

	int difference = 100;			//the difference(Value: lower = better) of the current best value card.
	int bestDifferenceCardIndex;	//The index of the best value card to play.
	int bestDifferencePileIndex;	//The index of pile to play to.
	int proposedDifference = 101;	//The difference of the card that can be played and is being checked for best value.
	for (int card = 0; card < player->handSize; card++)
	{
		cardIndex = card;
		cardValue = player->cards[cardIndex];
		for (int pile = 0; pile < 4; pile++)
		{
			topCardIndex = game->playPiles[pile].topCardIndex;
			topCard = game->playPiles[pile].cards[topCardIndex];
			pileIsAscending = game->playPiles[pile].isAscending;

			if ((cardValue < topCard && !pileIsAscending) || (cardValue > topCard && pileIsAscending))
			{
				proposedDifference = abs(topCard - cardValue);
			}
			else
			{
				proposedDifference = 101;
			}
			if (proposedDifference == difference)
			{
				//check if any are involved in a hand wise reverser.
				bool existingIs = false;
				bool newIs = false;
				int checkingCardValue;
				int existingCardValue = player->cards[bestDifferenceCardIndex];
				for (int cardPos = 0; cardPos < player->handSize; cardPos++)
				{
					checkingCardValue = player->cards[cardPos];
					if (cardValue == checkingCardValue - 10 || cardValue == checkingCardValue + 10)
					{
						newIs = true;
						break;
					}
				}
				for (int cardPos = 0; cardPos < player->handSize; cardPos++)
				{
					checkingCardValue = player->cards[cardPos];
					if (existingCardValue == checkingCardValue - 10 || existingCardValue == checkingCardValue + 10)
					{
						existingIs = true;
						break;
					}
				}
				if (!newIs && existingIs)
				{
					bestDifferenceCardIndex = cardIndex;
					bestDifferencePileIndex = pile;
				}
				if (newIs && existingIs)
				{
					int choice = rand() % 2;
					if (choice == 1)
					{
						bestDifferenceCardIndex = cardIndex;
						bestDifferencePileIndex = pile;
					}
				}
			}
			if (proposedDifference < difference)
			{
				difference = proposedDifference;
				bestDifferenceCardIndex = cardIndex;
				bestDifferencePileIndex = pile;
			}
		}
	}
	playPile = &(game->playPiles[bestDifferencePileIndex]);
	playerPlayCardToPile(player, bestDifferenceCardIndex, playPile, AUTOMATED);
}

void aiPlayCard(TheGame* game, Player* player)
{
	/*
		Attempts to play a reverser first.
		If it can, the ai plays the reverser and then breaks the function early.
		If it can't, it plays the best value card it can.
	*/
	if (aiCheckAndPlayReverser(game, player)) { return; }

	//This point is reached if a 'reverser' cannot be played.
	aiPlayBestValueCard(game, player);
}

int playerProcessTurn(TheGame* game, Player* player)
{
	/*
		While there is a valid play on the board and the player has not ended their turn OR they have played 2 or more cards.
			If the current player is an AI
				call the aiPlayCard function until two cards have been played.
				Then break out of the while.
			else
			Until the player does not cancel their card choice.
				get an input for the card the player would like to play and check it against current conditions.
				If the user chose 0
					set endTurn to true and break out of the card choice.
				Get an input for the pile the player would like to play to and check it against current conditions.
				If they didn't choose 0 (to cancel), play the specified card, to the specified pile.
			if they chose to end their turn, break out of the while loop.
			increment the number of cards played.
			if the player is human, redisplay the state of the game.
		If the player chose to end their turn and played at least 2 cards or the player is an AI:
			draw card for their hand back up to the maximum hand size.
		once operation is completed, return the number of cards played.
	*/
	int numCardsPlayed = 0;	//The number of cards played throughout the player's turn
	int whichCard;			//The display-index (index + 1) of the card to be played.
	int whichPile;			//The display-index (index + 1) of the pile 
	bool endTurn = false;	//did the player choose to end their turn?

							//Valid play and not ended turn or human and played at least two cards.
	while (((isThereAValidPlay(game, player) == true && endTurn == false) || (player->playerType == HUMAN && numCardsPlayed >= 2)))
	{
		endTurn = false;
		//AI loop.
		if (player->playerType == AI)
		{
			if (numCardsPlayed == 2) { break; }
			aiPlayCard(game, player);
		}
		else
		{
			//Human loop.
			do
			{
				whichCard = getPlayerCardChoice(player, numCardsPlayed);
				if (whichCard == 0) { endTurn = true; break; }
				whichPile = getPlayerPileChoice(player, whichCard);
				if (whichPile != 0) { playerPlayCardToPile(player, whichCard - 1, &game->playPiles[whichPile - 1], USERINPUT); }
			} while (whichPile == 0);
		}
		if (endTurn) { break; }
		numCardsPlayed++;

		//ReDraw game.
		std::system("CLS");
		displayGame(game);
		if (player->playerType == HUMAN) { displayPlayerHand(player, game->turns % game->numPlayers); }
		if (player->playerType == AI) { system("PAUSE"); }
	}

	//Draw Cards.
	if ((endTurn && numCardsPlayed >= 2) || (player->playerType == AI))
	{
		playerDrawCardsFromDeck(player, &game->drawDeck, player->maxHandSize, numCardsPlayed);
		std::system("CLS");
	}
	return numCardsPlayed;
}

int getNumberOfEmptySets(TheGame* game)
{
	/*
		Get and return the number of player-hands that are empty and if the deck is empty.
		if a player-hand or the deck is empty, add 1 to the rolling total.
		Players are checked first, then the deck.
		Rolling total then returned.
	*/
	int rollingTotal = 0;
	for (int x = 0; x < game->numPlayers; x++)
	{
		//for each player
		if (game->players[x].handSize == 0)
		{
			//if their hand is empty, increment the number of empty sets of cards.
			rollingTotal++;
		}
	}
	if (game->drawDeck.cntTop == 98)
	{
		//if the draw deck is empty, increment the number of empty sets of cards.
		rollingTotal++;
	}
	return rollingTotal;
}

bool isGameEnded(TheGame* game, int numCardsPlayed) 
{
	/*
		Perform a series of checks and return a boolean for whether or not the game needs to end.
		If there is a valid condition for the game to end:
			Check how many cards the last person played.
			if it was less than two display their hand
			else display the next person's hand (who doesn't have a valid play)
			this is done so players don't feel cheated and can check for themselves.
	*/
	bool ended = false;		//does the game need to end?
	int amountEmpty = 0;	//used for storing the number of sets of card that are empty.

	int cntPlayer = (game->turns - 1) % game->numPlayers;	//The index value of the player who's just taken their turn.
	Player* player = &(game->players[cntPlayer]);			//The player pointer of the player who's just taken their turn.

	int cntNextPlayer = game->turns % game->numPlayers;		//The index value of the player about to take their turn.
	Player* nextPlayer = &(game->players[cntNextPlayer]);	//The player pointer of the player about to take their turn.

	//Obtaining conditions for if the game is ended.
	amountEmpty = getNumberOfEmptySets(game);															//get the number of empty sets of cards in the game.
	bool nextPlayerHasNoValidPlay = !isThereAValidPlay(game, nextPlayer) && nextPlayer->handSize != 0;	//Does (the next player have no valid play) and (their hand is not empty)?
	bool allCardSetsEmpty = amountEmpty == game->numPlayers + 1;																		//Are all the card sets empty?
	bool validReason = numCardsPlayed == player->handSize + 1 || (numCardsPlayed == 0 && player->handSize == 0 && game->turns != 0);	//Is there a valid reason for less than two cards to have been played?
	bool lessThanTwoPlayedLastTurnNoReason = numCardsPlayed < 2 && !validReason;														//Did less than two cards get played with no valid reason?
	
	//If any condition for the game to end is met
	if (nextPlayerHasNoValidPlay || allCardSetsEmpty || lessThanTwoPlayedLastTurnNoReason)
	{
		//Deal with the end of the game.
		displayGame(game);
		if (numCardsPlayed >= 2)	{ displayPlayerHand(nextPlayer, cntNextPlayer); }
		else						{ displayPlayerHand(player, cntPlayer); }
		ended = true;
	}
	return ended;
}

int calculateGameScore(const TheGame* game)
{
	/*
		Calculate and return an integer for the score of the game.
		score = 
			number of cards in the deck
			+
			number of cards in each player's hand
	*/
	int numPlayers = game->numPlayers;
	int score = game->drawDeck.deckSize;
	for (int x = 0; x < numPlayers; x++)
	{
		score += game->players[x].handSize;
	}
	return score;
}

char gameOverScoreAndNewGame(TheGame* game)
{
	/*
		Calculate and display the score along with a game over message.
		then ask the player if they would like to start a new game, and return this answer.
	*/
	char newGame;							//The player's response for whether or not they would like a new game.
	int score = calculateGameScore(game);	//The calculated score for the last played game.
	cout << "Game over, you scored: " << score << endl;
	if (score == 0) { cout << "Congratulations, you win!\n" << endl; }
	newGame = getPlayerNewGameChoice();
	return newGame;
}

int main(int argc, char** argv) 
{
	/*
		The main program.
		Until the user would not like to start a new game
			Get the number of players the user would like.
			Create a new game with the specified number of players.
			While the game hasn't ended.
				Calculate the index of the next player's turn.
				Set the activePlayer pointer to the player at the calculated index.
				Display the game.
				Display the player hand.
				Get a value for the number of cards played from processing the player turn.
				Increment the number of turns.
			Game over - display the score and ask the user if they'd like to start a new game.
			Cleanup the memory taken by the last played game.
		Pause the system before closing the program.
	*/
	char newGame = 'y';	//Used to store whether or not the use would like to play a new game.
	int numCardsPlayed;	//Used when checking if the game is over.
	
	//Do the outer game loop (initialising and deleting pointers)
	do {
		int numPlayers = getNumPlayersInGame();		//The number of players in the game.
		TheGame* game = createNewGame(numPlayers);	//The pointer to the game object.
		numCardsPlayed = 2;		//2 is a temp value used to break into the game loop for the first time.
		
		//While the game has not reach an end, do the game loop
		while (!isGameEnded(game, numCardsPlayed)) {
			int cntPlayer = game->turns % game->numPlayers;		//The index for the next player.
			Player* activePlayer = &(game->players[cntPlayer]);	//The pointer to the active player.

			displayGame(game);
			if (activePlayer->playerType == HUMAN) { displayPlayerHand(activePlayer, cntPlayer); }
			numCardsPlayed = playerProcessTurn(game, activePlayer);

			game->turns++;
		}

		//end game score and new game decision.
		newGame = gameOverScoreAndNewGame(game);
		cleanupGame(game);
	} while (newGame == 'y' || newGame == 'Y');
	std::system("PAUSE");
	return 0;
}
