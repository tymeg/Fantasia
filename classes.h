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
#include <memory>
using namespace std;

class System // do zmiany w przypadku portowania na inny system
{
private:
    string OS = "Windows";
public:
    static int ESC()
    {
        return 27;
    }
    static int ENTER()
    {
        return 13;
    }

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

    static int GetKey()
    {
        return getch();
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
    int GetFieldNumber()
    {
        return FieldNumber;
    }
    void SetFieldNumber(int n)
    {
        FieldNumber = n;
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
    int Number;
public:
    Field(int num) : Number(num) {}
    int GetNumber()
    {
        return Number;
    }
};

class SpecialField : public Field // abstract class
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

class Game
{
private:
    const int Height, Length, FieldsNumber;
    int PlayersNumber;
    vector<Player*> Players;
    vector<Field*> SpecialFields;
    vector<vector<char>> Board
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
    map<int, pair<int, int>> FieldsToBoard
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
    Game() : Height(12), Length(15), FieldsNumber(76) // utworzenie pol specjalnych, wypelnienie nimi juz zainicjalizowanej Board
    {
        SpecialFields.push_back(new FieldAttributeUp(3, '@', "O! Jestes silniejszy!", 3, 's'));
        SpecialFields.push_back(new FieldMove(8, '+', "Skrot!", 4));

        for (int i=0; i<(int)SpecialFields.size(); i++)
        {
            SpecialField* field = (SpecialField*)SpecialFields[i];
            pair<int, int> coords = FieldsToBoard[field->GetNumber()];
            Board[coords.first][coords.second] = field->GetSymbol();
        }
    }
//    ~Game() // delete na elementach wektorow Players i SpecialFields, albo smart pointery?
//    {
//        for (auto p : SpecialFields)
//            delete p;
//        SpecialFields.clear();
//    }
    void DrawBoard()
    {
        for (int i=0; i<(int)Board.size(); i++)
        {
            for (int j=0; j<(int)Board[i].size(); j++)
                cout << Board[i][j];
            cout << endl;
        }
    }

    void Start()
    {
        System::HideCursor();
        cout << "Bla bla bla, wprowadzenie do historii\nWcisnij ENTER by kontynuowac...";
        while (1)
        {
            int key = System::GetKey();
            if (key == System::ENTER())
                break;
        }
        System::ClearScreen();
        cout << "Wybierz liczbe graczy:\n[2] [3] [4]";
        while (1)
        {
            int key = System::GetKey();
            if (key >= '2' && key <= '4')
            {
                PlayersNumber = key-48;
                break;
            }
        }
        for (int i=0; i<PlayersNumber; i++)
        {
            System::ClearScreen();
            string name;
            cout << "Graczu " << i+1 << ", wprowadz swoje imie: ";
            cin >> name;
            System::ClearScreen();
            cout << name << ", wybierz klase postaci:" << endl
                 << "[R] Rycerz  - SILA 20, ZRECZNOSC 10, INTELIGENCJA 5.  Moc specjalna: " << endl
                 << "[L] Lucznik - SILA 5,  ZRECZNOSC 20, INTELIGENCJA 10. Moc specjalna: " << endl
                 << "[M] Mag     - SILA 5,  ZRECZNOSC 10, INTELIGENCJA 20. Moc specjalna: " << endl;

            while(1)
            {
                int key = System::GetKey();
                switch(key)
                {
                case 'r':
                    Players.push_back(new Knight(name, i));
                    goto next;
                case 'l':
                    Players.push_back(new Archer(name, i));
                    goto next;
                case 'm':
                    Players.push_back(new Mage(name, i));
                    goto next;
                default:
                    break;
                }
            }
            next:
            System::ClearScreen();
            cout << "Udalo sie!";
            System::Sleep1Sec();
        }
    }

    void Move() {}
    int RollDice();
};

#endif // CLASSES_H_INCLUDED
