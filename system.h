#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <string>
#include <conio.h>
#include <windows.h>

class System // do zmiany w przypadku portowania na inny system (funkcjonalnosci z conio.h i windows.h opakowane w metody statyczne klasy System)
{
private:
    std::string OS = "Windows";
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

#endif // SYSTEM_H_INCLUDED
