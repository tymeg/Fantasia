#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <vector>
#include <map>

#include "system.h"

class Player // klasa abstrakcyjna
{
protected:
    string Name;
    string Class;
    int Number;
    int FieldNumber;
    int Strength, Dexterity, Intelligence;
    bool Turn;
public:
    Player(string name, int number) : Name(name), Number(number), FieldNumber(0), Turn(true) {}
    virtual void SpecialAction() = 0;
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

    string GetName()
    {
        return Name;
    }
    int GetNumber()
    {
        return Number;
    }
    string GetClass()
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
};

class Knight : public Player
{
public:
    Knight(string name, int number) : Player(name, number)
    {
        Class = "Rycerz";
        Strength=20;
        Dexterity=10;
        Intelligence=5;
    }
    void SpecialAction()
    {
        cout << "Knight's special action!";
    }
};

class Archer : public Player
{
public:
    Archer(string name, int number) : Player(name, number)
    {
        Class = "Lucznik";
        Strength=5;
        Dexterity=20;
        Intelligence=10;
    }
    void SpecialAction()
    {
        cout << "Archer's special action!";
    }
};

class Mage : public Player
{
public:
    Mage(string name, int number) : Player(name, number)
    {
        Class = "Mag";
        Strength=5;
        Dexterity=10;
        Intelligence=20;
    }
    void SpecialAction()
    {
        cout << "Mage's special action!";
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
    string Description;
public:
    SpecialField(int num, char s, string d) : Field(num), Symbol(s), Description(d) {}
    virtual int Event(Player* p) = 0; // zwraca 0, jesli na danym polu gracz cos traci, 1 gdy nie traci, 2 gdy nie ma takiego rozroznienia
    char GetSymbol()
    {
        return Symbol;
    }
    string GetDescription()
    {
        return Description;
    }
};

class FieldMove : public SpecialField
{
private:
    int HowMany; // + lub - oznacza przemieszczenie do przodu/do tylu
public:
    FieldMove(int num, char s, string d, int n) : SpecialField(num, s, d), HowMany(n) {}
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
    FieldAttributeUp(int num, char s, string d, int n, char a) : SpecialField(num, s, d), HowMany(n), Attribute(a) {}
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
    string WinMessage, LoseMessage;
public:
    SpecialFieldWinOrLose(int num, char s, string d, string win, string lose) : SpecialField(num, s, d), WinMessage(win), LoseMessage(lose) {}

    string GetWinMessage()
    {
        return WinMessage;
    }
    string GetLoseMessage()
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
    FieldLoseTurn(int num, char s, string d, string win, string lose, int n, char a) : SpecialFieldWinOrLose(num, s, d, win, lose), AttributeThreshold(n), Attribute(a) {}
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
        if (attribute_level >= AttributeThreshold)
            return 1;
        else
        {
            p->LoseTurn();
            return 0;
        }
    }

};

class FieldFight : public SpecialFieldWinOrLose
{
private:
    int DiceThreshold;
    char Attribute;
public:
    FieldFight(int num, char s, string d, string win, string lose, int n, char a) : SpecialFieldWinOrLose(num, s, d, win, lose), DiceThreshold(n), Attribute(a) {}
//    int CalculateDiceTreshold(int player_attribute)
//    {
//        return 1;
//    }
//    int Event(Player* p)
//    {
//        return 1;
//    }

};


#endif // CLASSES_H_INCLUDED
