#ifndef _UNIVERSECELL_H_
#define _UNIVERSECELL_H_

#include "SimCell_v2.h"

// This module implements connect algorithms between cells and mutual relation

// On a square plan, width and lengh, Conway default rules
// Return one vector
std::vector<SimCell> MakeSquareUniverse(unsigned int a, unsigned int keepLifeRules = 23, unsigned int respawnRules = 3 )
{
    int area = a*a;
    std::vector<SimCell> SquareUniverse(area);

    std::string buff;                       // to name cell with index
    int i{};                                // index - int to string

    for( auto& cell : SquareUniverse )
    {
        buff = "Cell nr: " + std::to_string(i);
        cell.SetLabel(buff);
        i++;
        //std::cout << buff << "\tCell address: " << cell.GetCellAddress() << '\n';
        //std::cout << "State: ";
        cell.SetNumberOfNeighbors(1);
        cell.SetRules( keepLifeRules,respawnRules );
    }

    std::cout << std::endl;
    int limit = a*a - 1;

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

    // Display connections state
//    for (auto& cell : SquareUniverse)
//    {
//        cell.ShowLabel();
//        std::cout << std::endl;
//        cell.ShowNeighbors();
//    };

    return SquareUniverse;
}

void DisplaySquareUniverse(std::vector<SimCell> squareUniverse, int universeSize )
{
    int nextLineCounter = 0;
    std::cout << '\t';
    for ( auto& cell : squareUniverse )
    {
        if( !(nextLineCounter % universeSize == 0) )
        {
            cell.ShowState();
            std::cout << ' ';
            nextLineCounter++;
        }
        else
        {
            std::cout << "\n\t";
            cell.ShowState();
            std::cout << ' ';
            nextLineCounter++;
        }
    };
}

#endif // _UNIVERSECELL_H_
