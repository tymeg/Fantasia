#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
using namespace std;

class Display // do zmiany w przypadku portowania na inny system
{
private:
    string System = "Windows";
public:
    static void ClearScreen()
    {
        system("cls");
    }

    static void ReturnCursor()
    {
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    }

    static void HideCursor()
    {
        HANDLE hOut;
        CONSOLE_CURSOR_INFO ConCurInf;

        hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        ConCurInf.dwSize = 10;
        ConCurInf.bVisible = FALSE;

        SetConsoleCursorInfo(hOut, &ConCurInf);
    }

    static void Sleep1Sec()
    {
        Sleep(1000);
    }
};

class Player // abstract class
{
protected:
    string Name;
    int Number;
    int FieldNumber;
    int Strength, Dexterity, Intelligence;
public:
    Player(string name, int number) : Name(name), Number(number) {}
    virtual void SpecialAction() = 0;

    // getters and setters
    int getFieldNumber() { return FieldNumber; }
    void setFieldNumber(int n) { FieldNumber = n; }

    int getStrength() { return Strength; }
    void setStrength(int s) { Strength = s; }
    int getDexterity() { return Dexterity; }
    void setDexterity(int d) { Dexterity = d; }
    int getIntelligence() { return Intelligence; }
    void setIntelligence(int i) { Intelligence = i; }
};

class Knight : public Player
{
public:
    Knight(string name, int number) : Player(name, number)
    {
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
    int XCoord, YCoord;
public:
    Field(int X, int Y) : XCoord(X), YCoord(Y) {}
    int GetX() { return XCoord; }
    int GetY() { return YCoord; }
};

class SpecialField : public Field // abstract class
{
protected:
    string Description;
public:
    SpecialField(int X, int Y, string d) : Field(X, Y), Description(d) {}
    virtual void Event(Player* p) = 0;
    string GetDescription() { return Description; }
};

class FieldMove : public SpecialField
{
private:
    int HowMany; // + or - means forward/backwards
public:
    FieldMove(int X, int Y, string d, int n) : SpecialField(X, Y, d), HowMany(n) {}
    void Event(Player* p)
    {
        p->setFieldNumber(p->getFieldNumber()+HowMany);
        // tu jeszcze zmiana na planszy, ponowne narysowanie
    }
};

class FieldAttributeUp : public SpecialField
{
private:
    int HowMany;
    char Attribute;
public:
    FieldAttributeUp(int X, int Y, string d, int n, char a) : SpecialField(X, Y, d), HowMany(n), Attribute(a) {}
    void Event(Player* p)
    {
        switch(Attribute)
        {
        case 's':
            p->setStrength(p->getStrength()+HowMany);
        case 'd':
            p->setDexterity(p->getDexterity()+HowMany);
        case 'i':
            p->setIntelligence(p->getIntelligence()+HowMany);
        }
    }
};



class Game
{
private:
    const int Length=15, Height=9;

public:
    void Move() {}
    void RollDice() {}
    void DrawBoard() {}
};

#endif // CLASSES_H_INCLUDED
