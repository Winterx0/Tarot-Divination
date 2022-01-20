#include "tarot.h"
/* Outline is designed from Black Jack game in book "Beginning C++ Through Game Programming" by Michael Dawson*/

////////////////////////////////////////////////////////////////////////////
//declare overloaded operator in main function
ostream& operator<<(ostream& os, const Card& aCard);
ostream& operator<<(ostream& os, const Player& aPlayer);

int main()
{
	//////////////////////////////////////////////////////////////
	TarotReading tarot;
	tarot.openMenu(); //should be able to access all program functions from menu



	//////////////////////////////////////////////////////////////
	//menu to loop
	string goTo;

	cout << "\nWould you like to restart the program? [y/n]\n";
	cin >> goTo;
	while (goTo == "y")
	{
		system("pause");
		system("CLS");
		return main();
		break;
	}
	if (goTo == "n")
	{
		cout << "PROGRAM EXITING...\n";
		system("pause");
		system("cls");
		return 0;
	}
}

//overloading operator so cards can be sent to cout
ostream& operator<<(ostream& os, const Card& aCard)
{
	const string RANKS[] = { "ACE-", "2-", "3-", "4-", "5-", "6-", "7-", "8-", "9-", "10-", "PAGE-", "KNIGHT-", "QUEEN-", "KING-" };
	const string SUITS[] = { "WANDS-", "PENTACLES-", "CUPS-", "SWORDS-" };
	const string ARCANA[] = { "FOOL", "MAGICIAN", "HIGH_PRIESTESS", "EMPRESS", "EMPEROR", "HIEROPHANT", "LOVERS", "CHARIOT",
	"STRENGTH", "HERMIT", "WHEEL_OF_FORTUNE", "JUSTICE", "HANGED_MAN", "DEATH", "TEMPERANCE", "DEVIL",
	"TOWER", "STAR", "MOON", "SUN", "JUDGEMENT", "WORLD" };

	if (aCard.m_ifu)
	{
		os << RANKS[aCard.m_rank] << SUITS[aCard.m_suit];
		os << ARCANA[aCard.m_arcana];
	}
	else
	{
		os << "XX";
	}

	return os;

}

////////////////////////////////////////////////////////////////////////////
//overloads operator so Player object can be sent to cout
ostream& operator<<(ostream& os, const Player& aPlayer)
{
	os << aPlayer.m_name << ":\t";

	vector<Card*>::const_iterator tCard;
	if (!aPlayer.m_cards.empty())
	{
		for (tCard = aPlayer.m_cards.begin(); tCard != aPlayer.m_cards.end(); ++tCard)
		{
			os << *(*tCard) << "\t";
		}
	}
	else
	{
		os << "<empty>";
	}
	return os;
}