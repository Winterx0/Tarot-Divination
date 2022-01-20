#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>

using namespace std;
/* Outline is designed from Black Jack game in book "Beginning C++ Through Game Programming" by Michael Dawson*/
/* 
In this simulation you will enter your name as user input
Then you will pick an option from the menu -
[S]tart a new reading, [R]etrieve last reading, [V]iew up tarot card information or [Q]uit?

Start a new reading draws 3 cards looking like - ACE-CUPS-WORLD
[This is to represent a total of 6 cards, first the rank-suit and the third card is the Major Arcana that adds definition
to the first card]
Cards will be drawn at random - 78 cards
you can choose an option to start new reading, retrieve last reading, view tarot information txt file or quit program.
new readings automatically get saved to output txt file
*/

///////////////////////////////////////////////////////////////////////////////NEW CLASS
//Cards and suits
class Card
{
public:
	enum rank {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, PAGE, KNIGHT, QUEEN, KING};

	enum arcana {FOOL, MAGICIAN, HIGH_PRIESTESS, EMPRESS, EMPEROR, HIEROPHANT, LOVERS, CHARIOT,
	STRENGTH, HERMIT, WHEEL_OF_FORTUNE, JUSTICE, HANGED_MAN, DEATH, TEMPERANCE, DEVIL,
	TOWER, STAR, MOON, SUN, JUDGEMENT, WORLD};

	enum suit {WANDS, CUPS, PENTACLES, SWORDS};

	//overloading operator to send card object to standard output - POLYMORPHISM
	friend ostream& operator<<(ostream& os, const Card& aCard);

	Card(rank r = ACE, suit s = WANDS, arcana a = FOOL, bool ifu = true);

	//flips a card; if face up, becomes face down, vice versa
	void Flip();

private:
	rank m_rank;
	arcana m_arcana;
	suit m_suit;
	bool m_ifu;
};

Card::Card(rank r, suit s, arcana a, bool ifu) : m_rank(r), m_suit(s), m_arcana(a), m_ifu(ifu) {}

void Card::Flip() 
{
	m_ifu = !(m_ifu);
}

///////////////////////////////////////////////////////////////////////////////NEW CLASS
//designed for a collection of cards
class Hand
{
public:
	Hand();
	virtual ~Hand();

	//adds cards to the hand
	void AddCards(Card* tCard);

	//clears hand of all cards
	void Clear();

protected:
	vector<Card*> m_cards;
};

Hand::Hand()
{
	m_cards.reserve(3);
}

Hand::~Hand()
{
	Clear();
}

void Hand::AddCards(Card* tCard)
{
	m_cards.push_back(tCard);
}

void Hand::Clear()
{
	//iterate through vector, freeing all memory on the heap
	vector<Card*>::iterator iter = m_cards.begin();
	for (iter = m_cards.begin(); iter != m_cards.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}
	//clear vector of pointers
	m_cards.clear();
}

///////////////////////////////////////////////////////////////////////////////NEW CLASS - CHILD CLASS
class Player : public Hand
{
	////overloading operator to send player object to standard output - POLYMORPHISM
	friend ostream& operator<<(ostream& os, const Player& aPlayer);
public:
	Player(const string& name = "");

	virtual ~Player();

	//reads hand of cards to file i/o
	bool read(ifstream& afile)
	{

		if (afile.eof())
		{
			return false;
		}
		afile >> m_name;
		return true;
	}

	void write(ofstream& afile) {
		afile << m_name << " " << endl;
	}

protected:
	string m_name;
};

Player::Player(const string& name) : m_name(name) {}

Player::~Player() {}

///////////////////////////////////////////////////////////////////////////////NEW CLASS - CHILD CLASS
class Deck : public Hand
{
public:
	Deck();

	virtual ~Deck();

	//Create tarot deck of 78 cards
	void populate();

	//shuffle cards
	void shuffle();

	//deal one card
	void deal(Hand& tHand);
};

Deck::Deck()
{
	m_cards.reserve(78);
	populate();
}

Deck::~Deck() {}

void Deck::populate()
{
	Clear();
	//create tarot deck

	for (int s = Card::WANDS; s <= Card::SWORDS; ++s)
	{
		for (int r = Card::ACE; r <= Card::KING; ++r)
		{
			for (int a = Card::FOOL; a <= Card::WORLD; ++a)
			{
				AddCards(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(s), static_cast<Card::arcana>(a)));
			}
		}
	}
}

void Deck::shuffle()
{
	random_shuffle(m_cards.begin(), m_cards.end());
}

void Deck::deal(Hand& tHand)
{
	tHand.AddCards(m_cards.back());
	m_cards.pop_back();
	if (m_cards.empty())
	{
		cout << "Out of cards, unable to deal." << endl;
	}
}

///////////////////////////////////////////////////////////////////////////////NEW CLASS
class TarotReading
{
public:
	vector<string> names;
	string name;

	TarotReading();
	~TarotReading();

	//opens menu option
	void openMenu();

	//starts tarot reading simulation
	void start();

	//Searches for previous saved readings from a txt outfile - fstream
	void retrieveData();

	//retrieves Tarot information from a .txt inputfile - fstream
	void viewTarot();

private:
	Deck m_deck;
	Player m_player;
};

TarotReading::TarotReading() 
{
	//create an empty desk
	//seed the random number generator
	srand(static_cast<unsigned int>(time(0)));
	m_deck.populate();
	m_deck.shuffle();
}

TarotReading::~TarotReading() {}

void TarotReading::start()
{
	//closes after menu input to begin function
	cout << "\n";
	system("pause");
	system("CLS");

	//Enter player name
	
	cout << "Enter your name: " << endl;
	cin >> name;
	names.push_back(name);
	cout << "\nHello, " << name << " we will now draw your cards..." << endl;

	//deal 3 cards to player
	for (int i = 0; i < 3; ++i)
	{
		m_deck.deal(m_player);
	}

	cout << endl;

	//display hand
	cout << m_player << endl;

	//creates output file - writes player information to it
	string input;
	ofstream outputfile("SavedTarotReadings.txt");
	if (outputfile.is_open())
	{
		//*m_player->write(outputfile);
		outputfile << name << m_player << endl;
	}
	outputfile.close();

	//remove cards
	m_player.Clear();
	cout << "\nReading has finished...\n" << endl;

	//tests that player data was saved to output file
	cout << "New data has been added to the Output file." << endl;


	//tests that start function has been completed
	char response;
	bool run = true;
	do
	{
		cout << "\nPress [M]enu to go back to menu options or [Q]uit to exit the program..." << endl;
		cin >> response;
		switch (toupper(response))
		{
		case 'M': system("pause"); system("CLS"); TarotReading::openMenu(); break;
		case 'Q': run = false; break;
		}
	} while (run);
}

void TarotReading::retrieveData()
{
	//closes after menu input to begin function
	cout << "\n";
	system("pause");
	system("CLS");
 
	//displays file data to console
	ifstream afile("SavedTarotReadings.txt");
	if (afile.is_open())
	{
		m_player.Clear();
		//EXCEPTION HANDLING
		try {
			while (!afile.eof())
			{
				afile >> name;
				cout << name << " : " << endl;
				cout << "Displaying player and card information" << endl;
			} 
		}
		catch (...) {
		}
		afile.close();
	}
	
	//tests that start function has been completed
	char response;
	bool run = true;
	do
	{
		cout << "\nPress [M]enu to go back to menu options or [Q]uit to exit the program..." << endl;
		cin >> response;
		switch (toupper(response))
		{
		case 'M': system("pause"); system("CLS"); TarotReading::openMenu(); break;
		case 'Q': run = false; break;
		}
	} while (run);
}

void TarotReading::viewTarot()
{
	//closes after menu input to begin function
	cout << "\n";
	system("pause");
	system("CLS");

	//statement regarding loading txt file
	cout << "\nPlease wait while file data has completed loading...\n" << endl;
	system("pause");

	//opens and reads file
	string fileText;
	ifstream inputfile; //input file stream
	inputfile.open("TarotDiv.txt");
	if (inputfile.is_open())
	{
		// Keep reading the file
		while (getline(inputfile, fileText))
		{
			// print the line on the standard output
			cout << fileText << endl;
		}
		// File Close
		inputfile.close();
	}
	else
	{
		cout << "Unable to open the file!";
	}

	//tests that start function has been completed
	char response;
	bool run = true;
	do
	{
		cout << "\nPress [M]enu to go back to menu options or [Q]uit to exit the program..." << endl;
		cin >> response;
		switch (toupper(response))
		{
		case 'M': system("pause"); system("CLS"); TarotReading::openMenu(); break;
		case 'Q': run = false; break;
		}
	} while (run);
}

void TarotReading::openMenu()
{
	char response;
	bool run = true;
	cout << "Welcome to Tarot divination!" << endl;
	cout << "Draw cards and watch your destiny unfold...\n" << endl;
	cout << "Would you like to - ";
	cout << "[S]tart a new reading, [R]etrieve last reading, [V]iew up tarot card information or [Q]uit?" << endl;
	cin >> response;
	switch (toupper(response))
	{
	case 'S': TarotReading::start(); break;
	case 'R': TarotReading::retrieveData(); break;
	case 'V': TarotReading::viewTarot(); break;
	case 'Q': run = false; break;
	}
}
