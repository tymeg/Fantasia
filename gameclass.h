#ifndef GAMECLASS_H_INCLUDED
#define GAMECLASS_H_INCLUDED

#include "classes.h"

class Game
{
private:
    const int Height, Length, FieldsNumber;
    int PlayersNumber;
    vector<Player*> Players;
    vector<SpecialField*> SpecialFields;
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
    Game() : Height(12), Length(15), FieldsNumber(76) // utworzenie pol specjalnych
    {
        SpecialFields.push_back(new FieldAttributeUp(3, '@', "O! Jestes silniejszy!", 3, 's'));
        SpecialFields.push_back(new FieldMove(8, '+', "Skrot!", 4));
    }
//    ~Game() // delete na elementach wektorow Players i SpecialFields, albo smart pointery?
//    {
//        for (auto p : SpecialFields)
//            delete p;
//        SpecialFields.clear();
//    }

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
            cout << "Graczu " << i+1 << ", wprowadz swoje imie (max 10 liter): ";
            cin >> name;
            if (name.length() > 10)
                name = name.substr(0, 10);

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
                    Players.push_back(new Knight(name, i+1));
                    goto next;
                case 'l':
                    Players.push_back(new Archer(name, i+1));
                    goto next;
                case 'm':
                    Players.push_back(new Mage(name, i+1));
                    goto next;
                default:
                    break;
                }
            }
next:
            System::ClearScreen();
            cout << "Utworzono postac!";
//            System::Sleep1Sec();
        }
        System::ClearScreen();
        srand(time(0));
    }

    // WYSWIETLANIE PLANSZY
    void ShowSpecialFieldsOnBoard()
    {
        for (int i=0; i<(int)SpecialFields.size(); i++)
        {
            SpecialField* field = SpecialFields[i];
            pair<int, int> coords = FieldsToBoard[field->GetNumber()];
            Board[coords.first][coords.second] = field->GetSymbol();
        }
    }
    void ShowPlayersOnBoard()
    {
        for (int i=0; i<(int)Players.size(); i++)
        {
            Player* player = Players[i];
            pair<int, int> coords = FieldsToBoard[player->GetFieldNumber()];
            char field_symbol = Board[coords.first][coords.second];
            char player_num = (char)player->GetNumber()+48;
            if (field_symbol != player_num && (field_symbol == '0' || field_symbol == '1' || field_symbol == '2' || field_symbol == '3' || field_symbol == '4')) // na polu jest inny gracz/gracze
                Board[coords.first][coords.second] = '0';
            else // wpp przykrywamy ewentualne pole specjalne
                Board[coords.first][coords.second] = player_num;
        }
    }

    void ClearField(int field)
    {
        pair<int, int> coords = FieldsToBoard[field];
        Board[coords.first][coords.second] = ' ';
    }

    void DrawBoard()
    {
        ShowSpecialFieldsOnBoard();
        ShowPlayersOnBoard();
        for (int i=0; i<(int)Board.size(); i++)
        {
            for (int j=0; j<(int)Board[i].size(); j++)
                cout << Board[i][j];

            switch(i) // wypisywanie graczy wraz z atrybutami
            {
            case 0:
                cout << "  " << Players[0]->GetClass() << " " << Players[0]->GetName() << " (1): ";
                break;
            case 1:
                cout << "    " << Players[0]->GetStrength() << "  " << Players[0]->GetDexterity() << "  " << Players[0]->GetIntelligence();
                break;
            case 3:
                cout << "  " << Players[1]->GetClass() << " " << Players[1]->GetName() << " (2): ";
                break;
            case 4:
                cout << "    " << Players[1]->GetStrength() << "  " << Players[1]->GetDexterity() << "  " << Players[1]->GetIntelligence();
                break;
            case 6:
                if (PlayersNumber > 2)
                    cout << "  " << Players[2]->GetClass() << " " << Players[2]->GetName() << " (3): ";
                break;
            case 7:
                if (PlayersNumber > 2)
                    cout << "    " << Players[2]->GetStrength() << "  " << Players[2]->GetDexterity() << "  " << Players[2]->GetIntelligence();
                break;
            case 9:
                if (PlayersNumber > 3)
                    cout << "  " << Players[3]->GetClass() << " " << Players[3]->GetName() << " (4): ";
                break;
            case 10:
                if (PlayersNumber > 3)
                    cout << "    " << Players[3]->GetStrength() << "  " << Players[3]->GetDexterity() << "  " << Players[3]->GetIntelligence();
                break;
            }
            cout << endl;
        }
        cout << endl;
    }

    // ROZGRYWKA
    bool IsOver()
    {
        for (int i=0; i<(int)Players.size(); i++)
        {
            if (Players[i]->GetFieldNumber() != FieldsNumber-1)
                return false;
        }
        return true;
    }

    void Play()
    {
        while (!IsOver())
        {
            for (int i=0; i<PlayersNumber; i++)
            {
                System::ClearScreen();
                DrawBoard();
                Player* player = Players[i];
                cout << "Kolej na ciebie, " << player->GetName() << "!                           " << endl
                     << "[ENTER] Rzuc kostka";
                while (1)
                {
                    int key = System::GetKey();
                    if (key == System::ENTER())
                        break;
                }

                System::Sleep1Sec();
                Move(player, RollDice());
                System::ClearScreen();
                DrawBoard();
                cout << "\n[ENTER] Dalej";
                while (1)
                {
                    int key = System::GetKey();
                    if (key == System::ENTER())
                        break;
                }
            }
        }
    }

    void Move(Player* p, int n)
    {
        int field_before = p->GetFieldNumber();
        p->SetFieldNumber(field_before+n);
        ClearField(field_before);
        System::ClearScreen();
        DrawBoard();

        int field_after = p->GetFieldNumber();
        for (int i=0; i<(int)SpecialFields.size(); i++)
        {
            SpecialField* spec = SpecialFields[i];
            if (field_after == spec->GetNumber())
            {
                cout << endl << spec->GetDescription();
                cout << "\n[ENTER] Dalej";
                while (1)
                {
                    int key = System::GetKey();
                    if (key == System::ENTER())
                        break;
                }
                spec->Event(p);
            }
        }
    }
    int RollDice()
    {
        int los = rand()%6 + 1;
        cout << "\rWyrzuciles " << los;
        if (los == 1)
            cout << " oczko!";
        else if (los == 5 || los == 6)
            cout << " oczek!";
        else
            cout << " oczka!";
        System::Sleep1Sec();
        return los;
    }
};

#endif // GAMECLASS_H_INCLUDED
