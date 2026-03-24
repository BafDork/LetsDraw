#include "KatamariDamacy/KatamariDamacyGame.h"

int main()
{
    KatamariDamacyGame game(800, 800);

    if (!game.Initialize())
        return -1;

    game.Run();

    return 0;
}