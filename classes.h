#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <vector>
#include <map>
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
    Player(string name, int number) : Name(name), Number(number), FieldNumber(0) {}
    virtual void SpecialAction() = 0;

    // getters and setters
    int getFieldNumber()
    {
        return FieldNumber;
    }
    void setFieldNumber(int n)
    {
        FieldNumber = n;
    }

    int getStrength()
    {
        return Strength;
    }
    void setStrength(int s)
    {
        Strength = s;
    }
    int getDexterity()
    {
        return Dexterity;
    }
    void setDexterity(int d)
    {
        Dexterity = d;
    }
    int getIntelligence()
    {
        return Intelligence;
    }
    void setIntelligence(int i)
    {
        Intelligence = i;
    }
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
    int GetX()
    {
        return XCoord;
    }
    int GetY()
    {
        return YCoord;
    }
};

class SpecialField : public Field // abstract class
{
protected:
    char Symbol;
    string Description;
public:
    SpecialField(int X, int Y, char s, string d) : Field(X, Y), Symbol(s), Description(d) {}
    virtual void Event(Player* p) = 0;
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
    FieldMove(int X, int Y, char s, string d, int n) : SpecialField(X, Y, s, d), HowMany(n) {}
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
    FieldAttributeUp(int X, int Y, char s, string d, int n, char a) : SpecialField(X, Y, s, d), HowMany(n), Attribute(a) {}
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
    const int Height, Length, FieldsNumber;
    vector<Field> Track;
    vector<vector<char>> Board;
    map<int, pair<int, int>> TrackToBoard
    {
        {0, {10, 1}}, {1, {9, 1}}, {2, {8, 1}}, {3, {7, 1}}, {4, {6, 1}}, {5, {5, 1}}, {6, {4, 1}}, {7, {3, 1}}, {8, {2, 1}}, {9, {1, 1}}, {10, {1, 2}},
        {11, {1, 3}}, {12, {2, 3}}, {13, {3, 3}}, {14, {4, 3}}, {15, {5, 3}}, {16, {6, 3}}, {17, {7, 3}}, {18, {8, 3}}, {19, {9, 3}}, {20, {10, 3}}, {21, {10, 4}},
        {22, {10, 5}}, {23, {9, 5}}, {24, {8, 5}}, {25, {7, 5}}, {26, {6, 5}}, {27, {5, 5}}, {28, {4, 5}}, {29, {3, 5}}, {30, {2, 5}}, {31, {1, 5}}, {32, {1, 6}},
        {33, {1, 7}}, {34, {2, 7}}, {35, {3, 7}}, {36, {4, 7}}, {37, {5, 7}}, {38, {6, 7}}, {39, {7, 7}}, {40, {8, 7}}, {41, {9, 7}}, {42, {10, 7}}, {43, {10, 8}},
        {44, {10, 9}}, {45, {9, 9}}, {46, {8, 9}}, {47, {7, 9}}, {48, {6, 9}}, {49, {5, 9}}, {50, {4, 9}}, {51, {3, 9}}, {52, {2, 9}}, {53, {1, 9}}, {54, {1, 10}},
        {55, {1, 11}}, {56, {2, 11}}, {57, {3, 11}}, {58, {4, 11}}, {59, {5, 11}}, {60, {6, 11}}, {61, {7, 11}}, {62, {8, 11}}, {63, {9, 11}}, {64, {10, 11}}, {65, {10, 12}},
        {66, {10, 13}}, {67, {9, 13}}, {68, {8, 13}}, {69, {7, 13}}, {70, {6, 13}}, {71, {5, 13}}, {72, {4, 13}}, {73, {3, 13}}, {74, {2, 13}}, {75, {1, 13}}
    };

public:
    Game() : Height(12), Length(15), FieldsNumber(76)
    {
        vector<char> first_row, second_row, before_last_row, last_row, middle_row;
        for (int i=0; i<Length; i++)
        {
            first_row.push_back('#');
            last_row.push_back('#');

            i%2==0 ? middle_row.push_back('#') : middle_row.push_back(' ');
            i%4==0 ? second_row.push_back('#') : second_row.push_back(' ');
            i%4-2==0 ? before_last_row.push_back('#') : before_last_row.push_back(' ');
        }
        last_row.at(1) = 'S';
        first_row.at(Length-2) = 'M';
        second_row.at(Length-1) = '#';
        before_last_row.at(0) = '#';

        Board.push_back(first_row);
        Board.push_back(second_row);
        for (int i=2; i<Height-2; i++)
            Board.push_back(middle_row);
        Board.push_back(before_last_row);
        Board.push_back(last_row);
    }
    void DrawBoard()
    {
        for (int i=0; i<(int)Board.size(); i++)
        {
            for (int j=0; j<(int)Board[i].size(); j++)
                cout << Board[i][j];
            cout << endl;
        }
    }

    void Move() {}
    void RollDice() {}
};

#endif // CLASSES_H_INCLUDED
