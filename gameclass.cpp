#include "classes.h"
using namespace std;

// ---------------
// METODY PRYWATNE
// ---------------

// WYSWIETLANIE PLANSZY
void Game::ShowSpecialFieldsOnBoard()
{
    for (int i=0; i<(int)SpecialFields.size(); i++)
    {
        SpecialField* field = SpecialFields[i];
        pair<int, int> coords = FieldsToBoard[field->GetNumber()];
        Board[coords.first][coords.second] = field->GetSymbol();
    }
}
void Game::ShowPlayersOnBoard()
{
    for (int i=0; i<(int)Players.size(); i++)
    {
        Player* player = Players[i];
        pair<int, int> coords = FieldsToBoard[player->GetFieldNumber()];
        char field_symbol = Board[coords.first][coords.second];
        char player_num = (char)player->GetNumber()+48;
        if (field_symbol != player_num && (field_symbol == '@' || (field_symbol >= '1' && field_symbol <= '4'))) // na polu jest inny gracz/gracze
            Board[coords.first][coords.second] = '@';
        else // wpp przykrywamy ewentualne pole specjalne
            Board[coords.first][coords.second] = player_num;
    }
}

void Game::ClearField(int field)
{
    pair<int, int> coords = FieldsToBoard[field];
    Board[coords.first][coords.second] = ' ';
}

void Game::DrawBoard()
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
bool Game::IsOver()
{
    for (int i=0; i<(int)Players.size(); i++)
    {
        if (Players[i]->GetFieldNumber() != FieldsNumber-1)
            return false;
    }
    return true;
}

void Game::Move(Player* p, int n)
{
    int field_before = p->GetFieldNumber();
    if (field_before+n > FieldsNumber-1) // gracz na mecie
        p->SetFieldNumber(FieldsNumber-1);
    else
        p->SetFieldNumber(field_before+n);

    ClearField(field_before);
    System::ClearScreen();
    DrawBoard();
    if (p->GetFieldNumber() == FieldsNumber-1)
    {
        cout << endl << "Gracz " << p->GetName() << " na mecie!";
        System::Sleep1Sec();
        Result.push_back(p);
        return;
    }

    int field_after = p->GetFieldNumber();
    for (int i=0; i<(int)SpecialFields.size(); i++)
    {
        SpecialField* spec = SpecialFields[i];
        if (field_after == spec->GetNumber())
        {
            cout << endl << spec->GetDescription() << endl;
            cout << "[ENTER] Dalej";
            WaitForEnter();
            int flag = spec->Event(p);
            if (flag == 2) {}
            else
            {
                SpecialFieldWinOrLose* win_or_lose = (SpecialFieldWinOrLose*)spec;
                if (flag == 1)
                    cout << "\r" << win_or_lose->GetWinMessage();
                else if (flag == 0)
                    cout << "\r" << win_or_lose->GetLoseMessage() << endl << "Tracisz kolejke!";
                else if (flag == -1) // FieldFight, przegrana, gracz klasy Rycerz - mozliwosc uzycia mocy specjalnej (jesli jeszcze nie zuzyta, sprawdza to FieldFight)
                {
                    System::ClearScreen();
                    DrawBoard();
                    cout << "Aby uniknac porazki, mozesz uzyc Mocy Specjalnej!" << endl
                         << "[ENTER] Dalej [M] Uzyj Mocy Specjalnej";
                    while (1)
                    {
                        int key = System::GetKey();
                        if (key == System::ENTER())
                        {
                            cout << "\r" << win_or_lose->GetLoseMessage() << "              " << endl << "Tracisz kolejke!";
                            break;
                        }
                        else if (key == 'm')
                        {
                            p->SpecialPower(Players);
                            p->ToggleUsedSpecialPower();
                            cout << "\r" << win_or_lose->GetWinMessage() << "               ";
                            break;
                        }
                    }
                }
                cout << endl << "[ENTER] Dalej";
                WaitForEnter();
            }
        }
    }
}

void Game::GameEnd() // wypisanie wynikow wedlug kolejnosci w wektorze Result, wyczyszczenie tego wektora
{
    System::ClearScreen();
    System::HideCursor();
    cout << "Bla bla bla - koniec fabuly.";

    cout << endl << endl << "Wyniki: " << endl;
    for (int i=0; i<(int)Result.size(); i++)
        cout << i+1 << ". " << Result[i]->GetName() << endl;

    cout << endl << "[ENTER] Powrot do Menu";
    WaitForEnter();

    Result.clear();
}

void Game::ResetGame() // czyszczenie wektora Players
{
    for (auto p : Players)
        delete p;
    Players.clear();
}

// ----------------
// METODY PUBLICZNE
// ----------------

// KOSNTRUKTOR
Game::Game() : Height(12), Length(15), FieldsNumber(76) // utworzenie pol specjalnych
{
    SpecialFields.push_back(new FieldAttributeUp(3, '$', "O! Jestes silniejszy!", 3, 's'));
    SpecialFields.push_back(new FieldMove(8, '+', "Skrot!", 4));
    SpecialFields.push_back(new FieldLoseTurn(5, 'X', "Spotykasz Golluma, ktory nie chce cie przepuscic, jesli nie rozwiazesz jego zagadki...", "Tak! Udzielasz poprawnej odpowiedzi i Gollum cie puszcza.", "Niestety, nie mozesz wymyslec odpowiedzi. Wracasz sie i wybierasz inna droge.", 10, 'i'));
    SpecialFields.push_back(new FieldLoseTurn(11, 'X', "Droge zagradza wielki, ciezki kamien...", "Udaje ci sie go troche przepchnac i przejsc dalej.", "Probujesz nieco go przepchnac, ale nic z tego, nie masz tyle sily. Wracasz sie i szukasz okreznej drogi.", 15, 's'));
    SpecialFields.push_back(new FieldFight(13, 'W', "Atakuje cie bandyta!", "Spuszczasz mu porzadne lanie! Po otrzymaniu kilku niezlych ciosow rzezimieszek rzuca sie w ucieczke :)", "Dostajesz kilka mocnych ciosow, tracisz przytomnosc. Dopiero po godzinie budzisz sie, oczywiscie bez pieniedzy :(", 's'));
    SpecialFields.push_back(new FieldFight(4, 'W', "Przed toba materializuje sie potezny czarnoksieznik!", "Przechytrzasz go xD Brawo!", "Mag pokonuje cie swoimi czarami. Potrzebujesz regeneracji xD", 'i'));
}

// DESTRUKTOR
Game::~Game() // delete na elementach wektorow Players i SpecialFields, albo smart pointery?
{
    for (auto f : SpecialFields)
        delete f;
    SpecialFields.clear();

    for (auto p : Players)
        delete p;
    Players.clear();
}

int Game::RollDice()
{
    int los = rand()%6 + 1;
    cout << "\rWyrzuciles " << los;
    if (los == 1)
        cout << " oczko!                                    ";
    else if (los == 5 || los == 6)
        cout << " oczek!                                    ";
    else
        cout << " oczka!                                    ";
    System::Sleep1Sec();
    return los;
}

void Game::WaitForEnter()
{
    while (1)
    {
        int key = System::GetKey();
        if (key == System::ENTER())
            break;
    }
}

void Game::Menu()
{
    int key;
    while(1)    // wyswietlanie menu
    {
        System::ClearScreen();
        System::HideCursor();
        cout << endl;
        cout << " ### ### ##  ### ### ### # ###" << endl
             << " #   # # ##   #  # # #   # # #" << endl
             << " ### ### # #  #  ### ### # ###" << endl
             << " #   # # # #  #  # #   # # # #" << endl
             << " #   # # # #  #  # # ### # # #" << endl
             << "                       by     " << endl
             << "                         Tymeg" << endl
             << "         [1] START            " << endl
             << "         [2] ZASADY           " << endl
             << "         [3] WYJSCIE          " << endl;

        while(1)    // oczekiwanie na wejscie z klawiatury uzytkownika
        {
            System::HideCursor();
            key = System::GetKey();
            if (key == '1')    // START
                return;
            else if (key == '2') // ZASADY GRY
            {
                System::ClearScreen();
                cout << "[ESC] Menu" << endl << endl;
                cout << " Zasady sa tak proste, ze nie ma czego tlumaczyc";

                while(1)    // powrot do menu
                {
                    key = System::GetKey();
                    if (key == System::ESC())
                        goto menu;
                }
            }
            else if (key == '3') // WYJSCIE
                exit(0);
        }
        menu:;
    }
}

void Game::Start()
{
    System::ClearScreen();
    System::HideCursor();
    cout << "Bla bla bla, wprowadzenie do historii\nWcisnij ENTER by kontynuowac...";
    WaitForEnter();
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
        cout << name << ", wybierz klase postaci:" << endl << endl
              << " [R] Rycerz  - SILA 20, ZRECZNOSC 10, INTELIGENCJA 5\n     Moc Specjalna: Szal Bitewny \n     W przypadku przegranej walki mozesz odwrocic losy i uniknac utraty kolejki" << endl << endl
             << " [L] Lucznik - SILA 5,  ZRECZNOSC 20, INTELIGENCJA 10\n     Moc specjalna: Sprint \n     W najblizszym ruchu przemiescisz sie o dwukrotnosc liczby wyrzuconych oczek" << endl << endl
             << " [M] Mag     - SILA 5,  ZRECZNOSC 10, INTELIGENCJA 20\n     Moc specjalna: Lodowy Deszcz \n     Pozostali gracze traca jedna kolejke" << endl;

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

void Game::Play()
{
    while (!IsOver())
    {
        for (int i=0; i<PlayersNumber; i++)
        {
            Player* player = Players[i];
            if (player->LosesTurn() || player->GetFieldNumber()==FieldsNumber-1) {} // jesli gracz traci kolejke, albo jest juz na mecie, to nie wykonuje ruchu
            else
            {
                System::ClearScreen();
                System::HideCursor();
                DrawBoard();
                cout << "Kolej na ciebie, " << player->GetName() << "!" << endl
                     << "[ENTER] Rzuc kostka";
                if (player->GetClass() == "Rycerz" || player->CheckUsedSpecialPower())
                {
                    cout << endl << endl << "[ESC] Wyjdz do Menu";
                    while (1)
                    {
                        int key = System::GetKey();
                        if (key == System::ENTER())
                            break;
                        else if (key == System::ESC()) // reset gry, wyjscie do menu
                        {
                            ClearField(player->GetFieldNumber());
                            ResetGame();
                            Menu();
                            return; // wychodzi z Play, w glownej petli w main wykona sie ponownie Start i Play
                        }
                    }
                }
                else
                {
                    cout << " [M] Uzyj Mocy Specjalnej";
                    cout << endl << endl << "[ESC] Wyjdz do Menu";
                    while (1)
                    {
                        int key = System::GetKey();
                        if (key == System::ENTER())
                            break;
                        else if (key == 'm')
                        {
                            player->SpecialPower(Players);
                            player->ToggleUsedSpecialPower();
                            cout << endl << "[ENTER] Rzuc kostka";
                            WaitForEnter();
                            break;
                        }
                        else if (key == System::ESC()) // reset gry, wyjscie do menu
                        {
                            ClearField(player->GetFieldNumber());
                            ResetGame();
                            Menu();
                            return; // wychodzi z Play, w glownej petli w main wykona sie ponownie Start i Play
                        }
                    }
                }
                System::Sleep1Sec();
                Move(player, player->GetMovesNumberMultiplier() * Game::RollDice());
                if (player->GetMovesNumberMultiplier() > 1)
                    player->SetMovesNumberMultiplier(1);
            }
        }
    }
    ClearField(FieldsNumber-1);
    GameEnd();
    ResetGame();
}


// -----------------------------------------------------------------
// DEFINICJA EVENT W FIELDFIGHT (korzysta z metod statycznych z Game)
// -----------------------------------------------------------------
int FieldFight::Event(Player* p)
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
    int threshold = CalculateDiceTreshold(attribute_level);
    cout << "\rMasz 3 rzuty kostka. Aby zwyciezyc, musisz wyrzucic lacznie nastepujaca liczbe oczek: " << threshold << endl;
    int i=0, sum=0;
    while(i<3)
    {
        cout << "\r[ENTER] Rzuc kostka   ";
        Game::WaitForEnter();
        sum += Game::RollDice();
        System::Sleep1Sec();
        i++;
    }
    cout << "\rLacznie wyrzucono: " << sum << "       ";
    System::Sleep1Sec();
    if (sum >= threshold) // win
        return 1;
    else // lose
    {
        p->LoseTurn();
        if (p->GetClass() == "Rycerz" && !(p->CheckUsedSpecialPower()))
            return -1;
        else
            return 0;
    }
}
