#include <iostream>
#include "SimCell_v2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

// TODO: Even more tests
// TODO: Makefile !!!
// TODO: Connect algorithm ( at the beginning make connections on a square plane - in dependency sense, not graphical )
// TODO: Rules implementation
// TODO: Command line support
// TODO: Ctor's ( for now only default )
// TODO: Graphical representation of connections between cells
// TODO: Exceptions...diagnostic communictes etc.

int main()
{
    srand(time(0));
    std::string buff;                       // to name cell with index
    int i{};                                // index - int to string
    std::vector<SimCell> cellGroup(5);      // array of 5 cells

    std::cout << "\n\t\tDIAGNOSTIC CODE BLOCK\n\n";

    // Set label with index in cells to easly recognize
    for( auto& cell : cellGroup )
    {
        buff = "Cell nr: " + std::to_string(i);
        cell.SetLabel(buff);
        i++;
        std::cout << buff << "\tCell address: " << cell.GetCellAddress() << '\n';
    }

    std::cout << std::endl;
    std::cout << "------------- SET RANDOM NUMBER OF NEIGHBORS  ----------------\n";
    std::cout << "----------- SET RANDOM ADDRESS (CELL) TO OTHER  --------------\n";
    std::cout << "------------- HISTORY OF CHANGES STEP BY STEP  ---------------\n";

    int counter = 1;
    int randomNeighbors{};

    // random addressing and setting number of neighbors
    for( auto& cell : cellGroup )
    {
        std::cout << '\n' << counter << ". STEP\n";
        cell.ShowLabel();
        randomNeighbors = rand() % 4;
        std::cout << "\nChange number of neighbors to: " << randomNeighbors;
        cell.SetNumberOfNeighbors(randomNeighbors);
        int index = rand() % 5;
        std::cout << std::endl;
        cell.ShowLabel();
        std::cout << " connect with ";
        cellGroup[index].ShowLabel();
        std::cout << '\n';
        cell.SetNeigborAddress(&cellGroup[index]);

        // after changes show diffrence
        for( auto& cell : cellGroup )
        {
            cell.ShowLabel();
            std::cout << std::endl;
            cell.ShowNeighbors();
        }
        counter++;
    }

    return 0;
}
