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
public:
    Player(string name, int number) : Name(name), Number(number), FieldNumber(0) {}
    virtual void SpecialAction() = 0;

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
    virtual void Event(Player* p) = 0;
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
    int HowMany; // + or - means forward/backwards
public:
    FieldMove(int num, char s, string d, int n) : SpecialField(num, s, d), HowMany(n) {}
    void Event(Player* p)
    {
        p->SetFieldNumber(p->GetFieldNumber()+HowMany);
        // tu jeszcze zmiana na planszy, ponowne narysowanie
    }
};

class FieldAttributeUp : public SpecialField
{
private:
    int HowMany;
    char Attribute;
public:
    FieldAttributeUp(int num, char s, string d, int n, char a) : SpecialField(num, s, d), HowMany(n), Attribute(a) {}
    void Event(Player* p)
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
    }
};

#endif // CLASSES_H_INCLUDED
