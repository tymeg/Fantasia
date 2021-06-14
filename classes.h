#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <vector>
#include <map>

#include "system.h"

// --------------------------------------
// GRACZE I POLA - DEKLARACJE + DEFINICJE

class Player // klasa abstrakcyjna
{
protected:
    std::string Name;
    std::string Class;
    int Number;
    int FieldNumber = 0;
    int Strength, Dexterity, Intelligence;
    bool UsedSpecialPower = false;
    int MovesNumberMultiplier = 1;
    bool Turn = true;

public:
    Player(std::string name, int number) : Name(name), Number(number) {}
    virtual ~Player() {}
    virtual void SpecialPower(std::vector<Player*> &players) = 0;
    bool LosesTurn()
    {
        if (Turn)
            return false;
        else
        {
            Turn = true;
            return true;
        }
    }
    void LoseTurn()
    {
        Turn = false;
    }

    // gettery i settery
    int GetFieldNumber()
    {
        return FieldNumber;
    }
    void SetFieldNumber(int n)
    {
        FieldNumber = n;
    }

    std::string GetName()
    {
        return Name;
    }
    int GetNumber()
    {
        return Number;
    }
    std::string GetClass()
    {
        return Class;
    }

    int GetStrength()
    {
        return Strength;
    }
    void SetStrength(int s)
    {
        Strength = s;
    }
    int GetDexterity()
    {
        return Dexterity;
    }
    void SetDexterity(int d)
    {
        Dexterity = d;
    }
    int GetIntelligence()
    {
        return Intelligence;
    }
    void SetIntelligence(int i)
    {
        Intelligence = i;
    }

    bool CheckUsedSpecialPower()
    {
        return UsedSpecialPower;
    }
    void ToggleUsedSpecialPower()
    {
        UsedSpecialPower = true;
    }

    int GetMovesNumberMultiplier()
    {
        return MovesNumberMultiplier;
    }
    void SetMovesNumberMultiplier(int mult)
    {
        MovesNumberMultiplier = mult;
    }
};

class Knight : public Player
{
public:
    Knight(std::string name, int number) : Player(name, number)
    {
        Class = "Rycerz";
        Strength=20;
        Dexterity=10;
        Intelligence=5;
    }
    ~Knight() {}
    void SpecialPower(std::vector<Player*> &players)
    {
        std::cout << "\rUzywasz Szalu Bitewnego!                ";
        for (int i=0; i<(int)players.size(); i++)
        {
            if (players[i]->GetNumber() == GetNumber())
            {
                players[i]->LosesTurn();
                break;
            }
        }
    }
};

class Archer : public Player
{
public:
    Archer(std::string name, int number) : Player(name, number)
    {
        Class = "Lucznik";
        Strength=5;
        Dexterity=20;
        Intelligence=10;
    }
    ~Archer() {}
    void SpecialPower(std::vector<Player*> &players)
    {
        std::cout << "\rUzywasz Sprintu - w tej kolejce wynik rzutu kostka bedzie podwojony!";
        for (int i=0; i<(int)players.size(); i++)
        {
            if (players[i]->GetNumber() == GetNumber())
            {
                players[i]->SetMovesNumberMultiplier(2);
                break;
            }
        }
    }
};

class Mage : public Player
{
public:
    Mage(std::string name, int number) : Player(name, number)
    {
        Class = "Mag";
        Strength=5;
        Dexterity=10;
        Intelligence=20;
    }
    ~Mage() {}
    void SpecialPower(std::vector<Player*> &players)
    {
        std::cout << "\rUzywasz Lodowego Deszczu - kazdy z pozostalych graczy traci kolejke!";
        for (int i=0; i<(int)players.size(); i++)
        {
            if (players[i]->GetNumber() != GetNumber())
                players[i]->LoseTurn();
        }
    }
};

class Field
{
private:
    int Number;
public:
    Field(int num) : Number(num) {}
    int GetNumber()
    {
        return Number;
    }
};

class SpecialField : public Field // klasa abstrakcyjna
{
protected:
    char Symbol;
    std::string Description;
public:
    SpecialField(int num, char s, std::string d) : Field(num), Symbol(s), Description(d) {}
    virtual ~SpecialField() {}
    virtual int Event(Player* p) = 0; // zwraca 0, jesli na danym polu gracz cos traci, 1 gdy nie traci, 2 gdy nie ma takiego rozroznienia
    char GetSymbol()
    {
        return Symbol;
    }
    std::string GetDescription()
    {
        return Description;
    }
};

class FieldMove : public SpecialField
{
private:
    int HowMany; // + lub - oznacza przemieszczenie do przodu/do tylu
public:
    FieldMove(int num, char s, std::string d, int n) : SpecialField(num, s, d), HowMany(n) {}
    ~FieldMove() {}
    int Event(Player* p)
    {
        p->SetFieldNumber(p->GetFieldNumber()+HowMany);
        return 2;
    }
};

class FieldAttributeUp : public SpecialField
{
private:
    int HowMany;
    char Attribute;
public:
    FieldAttributeUp(int num, char s, std::string d, int n, char a) : SpecialField(num, s, d), HowMany(n), Attribute(a) {}
    ~FieldAttributeUp() {}
    int Event(Player* p)
    {
        switch(Attribute)
        {
        case 's':
            p->SetStrength(p->GetStrength()+HowMany);
            break;
        case 'd':
            p->SetDexterity(p->GetDexterity()+HowMany);
            break;
        case 'i':
            p->SetIntelligence(p->GetIntelligence()+HowMany);
            break;
        }
        return 2;
    }
};

class SpecialFieldWinOrLose : public SpecialField // klasa abstrakcyjna (nie implementuje Event ze SpecialField)
{
private:
    std::string WinMessage, LoseMessage;
public:
    SpecialFieldWinOrLose(int num, char s, std::string d, std::string win, std::string lose) : SpecialField(num, s, d), WinMessage(win), LoseMessage(lose) {}
    virtual ~SpecialFieldWinOrLose() {}
    std::string GetWinMessage()
    {
        return WinMessage;
    }
    std::string GetLoseMessage()
    {
        return LoseMessage;
    }
};

class FieldLoseTurn : public SpecialFieldWinOrLose
{
private:
    int AttributeThreshold;
    char Attribute;
public:
    FieldLoseTurn(int num, char s, std::string d, std::string win, std::string lose, int n, char a) : SpecialFieldWinOrLose(num, s, d, win, lose), AttributeThreshold(n), Attribute(a) {}
    ~FieldLoseTurn() {}
    int Event(Player* p)
    {
        int attribute_level;
        switch(Attribute)
        {
        case 's':
            attribute_level = p->GetStrength();
            break;
        case 'd':
            attribute_level = p->GetDexterity();
            break;
        case 'i':
            attribute_level = p->GetIntelligence();
            break;
        }
        if (attribute_level >= AttributeThreshold) // win
            return 1;
        else // lose
        {
            p->LoseTurn();
            return 0;
        }
    }

};

class FieldFight : public SpecialFieldWinOrLose
{
private:
    char Attribute;
public:
    FieldFight(int num, char s, std::string d, std::string win, std::string lose, char a) : SpecialFieldWinOrLose(num, s, d, win, lose), Attribute(a) {}
    ~FieldFight() {}
    int CalculateDiceTreshold(int player_attribute)
    {
        return (55-player_attribute)/4;
    }
    int Event(Player* p);

};


// ---------------------------------------------------
// DEKLARACJA GAME
// ---------------------------------------------------
class Game
{
private:
    const int Height, Length, FieldsNumber;
    int PlayersNumber;
    std::vector<Player*> Players;
    std::vector<SpecialField*> SpecialFields;
    std::vector<Player*> Result;
    std::vector<std::vector<char>> Board
    {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', 'M', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'},
        {'#', 'S', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    };
    std::map<int, std::pair<int, int>> FieldsToBoard
    {
        {0, {10, 1}}, {1, {9, 1}}, {2, {8, 1}}, {3, {7, 1}}, {4, {6, 1}}, {5, {5, 1}}, {6, {4, 1}}, {7, {3, 1}}, {8, {2, 1}}, {9, {1, 1}}, {10, {1, 2}},
        {11, {1, 3}}, {12, {2, 3}}, {13, {3, 3}}, {14, {4, 3}}, {15, {5, 3}}, {16, {6, 3}}, {17, {7, 3}}, {18, {8, 3}}, {19, {9, 3}}, {20, {10, 3}}, {21, {10, 4}},
        {22, {10, 5}}, {23, {9, 5}}, {24, {8, 5}}, {25, {7, 5}}, {26, {6, 5}}, {27, {5, 5}}, {28, {4, 5}}, {29, {3, 5}}, {30, {2, 5}}, {31, {1, 5}}, {32, {1, 6}},
        {33, {1, 7}}, {34, {2, 7}}, {35, {3, 7}}, {36, {4, 7}}, {37, {5, 7}}, {38, {6, 7}}, {39, {7, 7}}, {40, {8, 7}}, {41, {9, 7}}, {42, {10, 7}}, {43, {10, 8}},
        {44, {10, 9}}, {45, {9, 9}}, {46, {8, 9}}, {47, {7, 9}}, {48, {6, 9}}, {49, {5, 9}}, {50, {4, 9}}, {51, {3, 9}}, {52, {2, 9}}, {53, {1, 9}}, {54, {1, 10}},
        {55, {1, 11}}, {56, {2, 11}}, {57, {3, 11}}, {58, {4, 11}}, {59, {5, 11}}, {60, {6, 11}}, {61, {7, 11}}, {62, {8, 11}}, {63, {9, 11}}, {64, {10, 11}}, {65, {10, 12}},
        {66, {10, 13}}, {67, {9, 13}}, {68, {8, 13}}, {69, {7, 13}}, {70, {6, 13}}, {71, {5, 13}}, {72, {4, 13}}, {73, {3, 13}}, {74, {2, 13}}, {75, {1, 13}}
    };

    // WYSWIETLANIE PLANSZY
    void ShowSpecialFieldsOnBoard();
    void ShowPlayersOnBoard();
    void ClearField(int field);
    void DrawBoard();

    // ROZGRYWKA
    bool IsOver();
    void Move(Player* p, int n);
    void ResetGame();
    void GameEnd();

public:
    Game();
    ~Game();

    static int RollDice();
    static void WaitForEnter();

    void Menu();
    void Start();
    void Play();
};


#endif // CLASSES_H_INCLUDED
