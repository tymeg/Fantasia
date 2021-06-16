#include "game.h"

using namespace std;

int main()
{
    Game test;

    while(1)
    {
        test.Menu();
        test.Start();
        test.Play();
    }

    return 0;
}
