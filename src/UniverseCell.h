#ifndef _UNIVERSECELL_H_
#define _UNIVERSECELL_H_

#include "SimCell_v2.h"
#include <curses.h>
#include <iostream>

// to find remember vector index in select mode
unsigned int indexSelect = 0;

// This module implements connect algorithms between cells and mutual relation
// On a square plan, width and lengh, Conway default rules
// Return one vector
std::vector<SimCell> MakeSquareUniverse(unsigned int a, unsigned int keepLifeRules = 23, unsigned int respawnRules = 3 )
{
    int area = a*a;
    std::vector<SimCell> SquareUniverse(area);

    std::string buff;                       // to name cell with index
    unsigned int i{};                       // index - int to string

    for( auto& cell : SquareUniverse )
    {
        buff = "Cell nr: " + std::to_string(i);
        cell.SetLabel(buff);
        i++;
        cell.SetNumberOfNeighbors(1);
        cell.SetRules( keepLifeRules, respawnRules );
    }

    unsigned int limit = a*a - 1;

    // row connetcion
    for ( i = 0; i < limit; i++)
    {
        // when is end of row get to next line and do nothing with last element
        if( !((i + 1) % a == 0 ) )
            SquareUniverse[i].SetNeigborAddress(&SquareUniverse[i + 1]);
        else
            continue;
    };

    // columns connections
    for ( i = 0; i <= limit - a; i++)
        SquareUniverse[i].SetNeigborAddress(&SquareUniverse[i + a]);

    // First diagonal connections '\'
    for ( i = 0; i < limit - a; i++)
    {
        if( !((i + 1) % a == 0 ) )
             SquareUniverse[i].SetNeigborAddress(&SquareUniverse[i + a + 1]);
        else
            continue;
    }

    // Second diagonal connections '/'
    for ( i = 0; i < limit - a; i++)
    {
        // if first column dont connect
        if ( ((i + 1) % a != 0 ) )
            SquareUniverse[i + 1].SetNeigborAddress(&SquareUniverse[i + a]);
        else
            continue;
    }

    return SquareUniverse;
}


template <isNcurses ncursesState>
void DisplaySquareUniverse(std::vector<SimCell> &squareUniverse, int universeSize )
{
	int nextLineCounter = 0;
	if constexpr (ncursesState == isNcurses::OFF_NCURSES)
	{
		std::cout << '\t';
	}
    for ( auto& cell : squareUniverse )
    {
        if( !(nextLineCounter % universeSize == 0) )
        {
            cell.ShowState<ncursesState>();
        	if constexpr (ncursesState == isNcurses::OFF_NCURSES)
			{
                std::cout << ' ';
			}
        	else
        	{
        		addch(' ');
        	}
            nextLineCounter++;
        }
        else
        {
        	if constexpr (ncursesState == isNcurses::OFF_NCURSES)
			{
        		std::cout << "\n\t";
			}
        	else
        	{
        		addstr("\n\t");
        	}
            cell.ShowState<ncursesState>();
        	if constexpr (ncursesState == isNcurses::OFF_NCURSES)
			{
        		std::cout << ' ';
			}
        	else
        	{
        		addch(' ');
        	}
            nextLineCounter++;
        }
    }
}


int NavigateUniverse(std::vector<SimCell>& universe, unsigned int a, unsigned int indexSelect, int key )
{
    switch( key )
    {
        case KEY_LEFT:
        {
            if( universe[0].ShowSelect() )
                break;
            else
            {
                universe[indexSelect].ShowSelect(false);
                universe[indexSelect - 1].ShowSelect(true);
            }

            indexSelect -= 1;
            break;
        }

        case KEY_RIGHT:
        {
            if( universe[universe.size() - 1].ShowSelect() )
                break;
            else
            {
                universe[indexSelect].ShowSelect(false);
                universe[indexSelect + 1].ShowSelect(true);
            }

            indexSelect += 1;
            break;
        }

        case KEY_DOWN:
        {
            if( universe[ ( universe.size() ) - a + ( indexSelect % a ) ].ShowSelect() )
                break;
            else
            {
                universe[indexSelect].ShowSelect(false);
                universe[indexSelect + a].ShowSelect(true);
            }

            indexSelect += a;
            break;
        }

        case KEY_UP:
        {
            if( universe[ indexSelect % a ].ShowSelect() )
                break;
            else
            {
                universe[indexSelect].ShowSelect(false);
                universe[indexSelect - a].ShowSelect(true);
            }

            indexSelect -= a;
            break;
        }

        case '\n':
        {
            universe[indexSelect].SetState( !universe[indexSelect].CellState() );

            break;
        }

        case 'e':
        {
            for( auto& cell : universe )
            {
                cell.SetState(false);
                cell.ShowSelect(false);
            }
            indexSelect = 0;

            break;
        }
    }

    return indexSelect;
}

#endif // _UNIVERSECELL_H_
