#ifndef PLAYERS_FIELDS_H_INCLUDED
#define PLAYERS_FIELDS_H_INCLUDED

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
// --------------------------------------

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
    std::string Description;
    char Symbol;
public:
    SpecialField(int num, std::string d, char s) : Field(num), Description(d), Symbol(s) {}
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
    FieldMove(int num, std::string d, int n) : SpecialField(num, d, '+'), HowMany(n)
    {
        if (num<0)
            Symbol = '-';
    }
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
    FieldAttributeUp(int num, std::string d, int n, char a) : SpecialField(num, d, '$'), HowMany(n), Attribute(a) {}
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
    SpecialFieldWinOrLose(int num, std::string d, char s, std::string win, std::string lose) : SpecialField(num, d, s), WinMessage(win), LoseMessage(lose) {}
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
    FieldLoseTurn(int num, std::string d, std::string win, std::string lose, int n, char a) : SpecialFieldWinOrLose(num, d, 'X', win, lose), AttributeThreshold(n), Attribute(a) {}
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
    FieldFight(int num, std::string d, std::string win, std::string lose, char a) : SpecialFieldWinOrLose(num, d, 'W', win, lose), Attribute(a) {}
    ~FieldFight() {}
    int CalculateDiceTreshold(int player_attribute)
    {
        return (55-player_attribute)/4;
    }
    int Event(Player* p);

};


#endif // PLAYERS_FIELDS_H_INCLUDED
