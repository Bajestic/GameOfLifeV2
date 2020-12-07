#include <iostream>
#include "SimCell_v2.h"
#include "UniverseCell.h"
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <curses.h>
#include <algorithm>

extern unsigned int indexSelect;
// TODO: Even more tests
// TODO: Connect algorithm ( for now is only on square plan )
// TODO: Ctor's ( for now only default )
// TODO: Exceptions...diagnostic communictes etc.

int main(int argc, char *argv[])
{
	unsigned int keepLifeRules = 23;
	unsigned int respawnRules = 3;

	if (argc == 1)
	{
		unsigned int sizeUniverse = 150;
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
				DisplaySquareUniverse<isNcurses::ON_NCURSES>(connectedCells, sizeUniverse);
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
					DisplaySquareUniverse<isNcurses::ON_NCURSES>(connectedCells, sizeUniverse);
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
				unsigned int temp{};
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

				break;
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

				break;
			}
			default:
			{
				connectedCells[indexSelect].ShowSelect(true);
				clear();
				DisplaySquareUniverse<isNcurses::ON_NCURSES>(connectedCells, sizeUniverse);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				break;
			}
			}
		}

		refresh();
		endwin();
	}
	else if (argc == 2 && std::string(argv[1]) == "-auto")
	{
		std::cout << "\t! ! ! WELCOME IN GAME OF LIFE ! ! !\n";
		auto connectedCells = MakeSquareUniverse(40, keepLifeRules, respawnRules);
		std::cout << std::endl;

		connectedCells[672].SetState(true);
		connectedCells[673].SetState(true);
		connectedCells[674].SetState(true);
		connectedCells[702].SetState(true);
		connectedCells[733].SetState(true);

		int nr = 1;

		connectedCells[672 + nr].SetState(true);
		connectedCells[673 + nr].SetState(true);
		connectedCells[674 + nr].SetState(true);
		connectedCells[702 + nr].SetState(true);
		connectedCells[733 + nr].SetState(true);

		nr = 4;

		connectedCells[672 + nr].SetState(true);
		connectedCells[673 + nr].SetState(true);
		connectedCells[674 + nr].SetState(true);
		connectedCells[702 + nr].SetState(true);
		connectedCells[733 + nr].SetState(true);

		int row = 0;

		//    connectedCells[150 + row].SetState(true);
		//    connectedCells[151 + row].SetState(true);
		//    connectedCells[152 + row].SetState(true);
		//    connectedCells[122 + row].SetState(true);
		//    connectedCells[91 + row].SetState(true);

		//    connectedCells[0].SeeRules();

		while( true )
		{
			system("clear");
			DisplaySquareUniverse<isNcurses::OFF_NCURSES>(connectedCells, 40);

			std::this_thread::sleep_for(std::chrono::milliseconds(125));

			for( auto& cell : connectedCells )
				cell.MakeAnalize();
			for( auto& cell : connectedCells )
				cell.MakeUpdate();
			//break;
		}
	}
	else
	{
		std::cout << "Command line argument not recognized!\n";
	}

	return 0;
}
