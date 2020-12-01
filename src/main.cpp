#include <iostream>
#include "SimCell_v2.h"
#include "UniverseCell.h"
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <curses.h>

// TODO: Even more tests
// TODO: Makefile !!!
// TODO: Connect algorithm ( for now is only on square plan )
// TODO: Rules implementation
// TODO: Command line support
// TODO: Ctor's ( for now only default )
// TODO: Graphical representation of connections between cells
// TODO: Exceptions...diagnostic communictes etc.

int main()
{
    auto connectedCells = MakeSquareUniverse(35);

    WINDOW * win = initscr();   // initialize screen
    nodelay(win,true);          // no waiting for enter button
    noecho();                   // dont show input
    keypad(win,true);           // enable kaypad with macros
    refresh();

    unsigned int indexSelect = 0;
    int keyPress{};

    while( keyPress != 'q' )
    {
        keyPress = getch();
        switch( keyPress )
        {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
            case KEY_ENTER:
            case '\n':
            case 'e':
            {
                clear();
                indexSelect = NavigateUniverse(connectedCells,35,keyPress);
                DisplaySquareUniverse(connectedCells, 35);
                std::this_thread::sleep_for(std::chrono::milliseconds(30));

                break;
            }
            case ' ':
            {
                connectedCells[indexSelect].ShowSelect(false);

                // space button switch between on/off editor mode
                while( keyPress = getch() != ' ' )
                {
                    clear();
                    DisplaySquareUniverse(connectedCells, 35);
                    std::this_thread::sleep_for(std::chrono::milliseconds(125));

                    for( auto& cell : connectedCells )
                        cell.MakeAnalize();
                    for( auto& cell : connectedCells )
                        cell.MakeUpdate();
                }
            }
            default:
            {
                connectedCells[indexSelect].ShowSelect(true);
                clear();
                DisplaySquareUniverse(connectedCells, 35);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

    refresh();
    endwin();

    return 0;
}
