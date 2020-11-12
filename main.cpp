#include <iostream>
#include "SimCell_v2.h"
#include "UniverseCell.h"
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>

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
    std::cout << "\t! ! ! WELCOME IN GAME OF LIFE ! ! !\n";
    auto connectedCells = MakeSquareUniverse(40);
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
        DisplaySquareUniverse(connectedCells, 40);

        std::this_thread::sleep_for(std::chrono::milliseconds(125));

        for( auto& cell : connectedCells )
            cell.MakeAnalize();
        for( auto& cell : connectedCells )
            cell.MakeUpdate();
            //break;
    }


    // DisplaySquareUniverse(connectedCells, 30);

    std::cout << std::endl;

    return 0;
}
