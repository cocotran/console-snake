#include <iostream>
#include <Windows.h>
#include <list>
#include <thread>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 80;

struct SSnakeSegment {
    int x;
    int y;
};

int main()
{
    // Create Screen Buffer
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;


    list<SSnakeSegment> snake = { {60,15}, {61,15}, {62,15}, {63,15}, {64,15}, {65,15}, {66,15}, {67,15}, {68,15}, {69,15} };
    int nFoodX = 30;
    int nFoodY = 15;
    int nScore = 0;
    int nSnakeDirection = 3;    // Compass points
    bool bDead = false;
    bool bKeyLeft = false, bKeyRight = false;
    bool bKeyLeftOld = false, bKeyRightOld = false; // previous states


    while (true)
    {
        // Timing & Input
        this_thread::sleep_for(200ms);


        // Get input
        bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
        bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;
        
        if (bKeyRight && !bKeyRightOld)
        {
            nSnakeDirection++;
            if (nSnakeDirection == 4) nSnakeDirection = 0;
        }

        if (bKeyLeft && !bKeyLeftOld)
        {
            nSnakeDirection--;
            if (nSnakeDirection == -1) nSnakeDirection = 3;
        }

        bKeyRightOld = bKeyRight;
        bKeyLeftOld = bKeyLeft;

        // Game logic
        switch (nSnakeDirection)
        {
        case 0: // UP
            snake.push_front({ snake.front().x, snake.front().y - 1 });
            break;
        case 1: // RIGHT
            snake.push_front({ snake.front().x + 1, snake.front().y });
            break;
        case 2: // DOWN
            snake.push_front({ snake.front().x, snake.front().y + 1 });
            break;
        case 3: // LEFT
            snake.push_front({ snake.front().x - 1, snake.front().y });
            break;
        default:
            break;
        }


        // Chop off Snakes tail
        snake.pop_back();


        // Display to player
        // Clear Screen
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

        // Draw Stats & Boorder
        for (int i = 0; i < nScreenWidth; i++)
        {
            screen[i] = L'=';
            screen[2 * nScreenWidth + i] = L'=';
        }
        wsprintf(&screen[nScreenWidth + 5], L"S N A K E ! !                            SCORE: %d", nScore);

        // Draw Snake Body
        for (auto s : snake)
            screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'O';

        // Draw Snake Head
        screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'@';

        // Draw Food
        screen[nFoodY * nScreenWidth + nFoodX] = L'%';

        // Display Frame
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    return 0;
}
