#include <iostream>
#include "SimCell_v2.h"
#include "UniverseCell.h"
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <curses.h>

extern unsigned int indexSelect;
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
    unsigned int sizeUniverse = 150;
    unsigned int keepLifeRules = 23;
    unsigned int respawnRules = 3;
    auto connectedCells = MakeSquareUniverse(sizeUniverse, keepLifeRules, respawnRules);

    WINDOW * win = initscr();   // initialize screen
    nodelay(win,true);          // no waiting for enter button
    noecho();                   // dont show input
    keypad(win,true);           // enable kaypad with macros
    refresh();

    //unsigned int indexSelect = 0;
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
                indexSelect = NavigateUniverse(connectedCells,sizeUniverse, indexSelect, keyPress);
                DisplaySquareUniverse(connectedCells, sizeUniverse);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));

                break;
            }
            case ' ':
            {
                connectedCells[indexSelect].ShowSelect(false);

                // space button switch between on/off editor mode
                while( keyPress = getch() != ' ' )
                {
                    clear();
                    DisplaySquareUniverse(connectedCells, sizeUniverse);
                    std::this_thread::sleep_for(std::chrono::milliseconds(125));

                    for( auto& cell : connectedCells )
                        cell.MakeAnalize();
                    for( auto& cell : connectedCells )
                        cell.MakeUpdate();
                }
                break;
            }
            case 'r':
            {
                clear();
                nodelay(win, false);
                echo();
                printw("CHANGE RULES:\n");
                printw("Keep life rule: ");
                scanw("%u",&keepLifeRules);
                printw("Respawn rule: ");
                scanw("%u",&respawnRules);
                for( auto& cell : connectedCells )
                    cell.SetRules(keepLifeRules,respawnRules);
                noecho();
                nodelay(win, true);
            }
            case 'u':
            {
                clear();
                unsigned int temp{};
                nodelay(win, false);
                echo();
                printw("CHANGE UNIVERSE SIZE:\n");
                printw("dimension: ");
                scanw("%u",&temp);
                while( !(temp >= 10 && temp <= 150) )
                {
                    printw("give dimension from 10 to 150: ");
                    scanw("%u",&temp);
                }
                sizeUniverse = temp;
                connectedCells = MakeSquareUniverse(sizeUniverse);
                clear();
                indexSelect = 0;
                noecho();
                nodelay(win, true);
            }
            default:
            {
                connectedCells[indexSelect].ShowSelect(true);
                clear();
                DisplaySquareUniverse(connectedCells, sizeUniverse);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                break;
            }
        }
    }

    refresh();
    endwin();

    return 0;
}
