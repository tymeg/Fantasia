#include "game.h"

using namespace std;

int main()
{
    Game game;

    while(1)
    {
        game.Menu();
        game.Start();
        game.Play();
    }

    return 0;
}
