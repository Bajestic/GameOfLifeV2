#include <iostream>
#include "SimCell_v2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));
    std::string buff;   // to name cell with index
    int i{};            // index - int to string
    std::vector<SimCell> cellGroup(10);

    std::cout << "DIAGNOSTIC CODE\n\n";
    for( auto& cell : cellGroup )
    {
        // Set label with index in cells to easly recognize
        buff = "Cell nr: " + std::to_string(i);
        cell.SetLabel(buff);
        i++;
        std::cout << buff << "\tCell address: " << cell.GetCellAddress() << '\n';
    }

    std::cout << std::endl;
    std::cout << "------------- SET RANDOM NUMBER OF NEIGHBORS  ----------------\n\n";

    for ( auto& cell : cellGroup )
    {
        // random addressing and setting number of neighbors
        cell.SetNumberOfNeighbors(rand() % 4);
        int index = rand() % 10;
        cell.SetNeigborAddress(&cellGroup[index]);
        cell.ShowLabel();
        std::cout << std::endl;
        cell.ShowNeighbors();
    }

    return 0;
}
