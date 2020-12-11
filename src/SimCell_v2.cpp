#include "SimCell_v2.h"
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include <ncurses.h>

SimCell::SimCell() :
    killCounter(0),
    respawnCounter(0),
    numberOfNeighbors(0),
    killRules(1, 3),
    respawnRules(1, 3),
    showSelect(false),
    nextStepCellStatus(false),
    cellStatus(false),
    cellAddress(this),
    neighborsAddressesList(),
    label("unnamed")
{}

bool SimCell::CellState() const { return cellStatus; }

const SimCell * SimCell::GetCellAddress() const { return this; }

bool SimCell::SetNumberOfNeighbors(unsigned int neigbours)
{
    if( neigbours > 0) // if exist
    {
        if ( neigbours == neighborsAddressesList.size() )
            return false; // the same size
        else if ( neigbours > neighborsAddressesList.size() )
        {
            int counter = neigbours - neighborsAddressesList.size();
            for (int i = 0; i < counter ; i++)
                neighborsAddressesList.push_back(nullptr); // add "free host" to link with other cellls
        }
        else
        {
            auto lastAddress = (neighborsAddressesList.end() - 1);
            int counter = neighborsAddressesList.size() - neigbours;
            for (int i = 0; i < counter; i++) // downsize
            {
                if (*lastAddress == nullptr) // we can't call DeleteAddress function if address is nullptr
                {
                    lastAddress--;
                    neighborsAddressesList.pop_back();
                }
                else
                {
                    (*lastAddress)->DeleteAddress(this); // comunicate other cells to delete this cell from address list
                    lastAddress--;
                    neighborsAddressesList.pop_back();
                }
            }
        }
        numberOfNeighbors = neigbours;
        return true;
    }
    else if( neigbours == 0 )
    {
        this->ResetNumberOfNeigbors();
    }
    else
        return false;
}

void SimCell::ResetNumberOfNeigbors()
{
    if ( neighborsAddressesList.size() > 0)
        for (auto address : neighborsAddressesList) // only use to tell other cells about disconnet
            {
                if (address != nullptr) // if connection with other cell exist
                    address->DeleteAddress(this); // comunicate other cells to delete this cell from address list
                else
                    continue;
            }
        neighborsAddressesList.clear(); // clear global all vector
        numberOfNeighbors = 0;
}

// all hosts in vector has non null pointer address
bool SimCell::IsFullAddressList()
{
    if ( this->neighborsAddressesList.size() > 0)
        for( auto& address : neighborsAddressesList )
            if( address == nullptr )
                return false;
            else
                continue;

    return true; // zero-size vector is interpreted as full addressed
}

int SimCell::NumberOfFreeNeighborsHost()
{
    if ( numberOfNeighbors > 0 )
    {
        int freeNeighbor{};
        for (auto& address : neighborsAddressesList)
            if (address == nullptr)
            {
                freeNeighbor++;
            }
            else
                continue;

        return freeNeighbor;
    }
    else
        return 0;
}

bool SimCell::DeleteAddress(SimCell * addressToDelete)
{
    // looking in cell list address to delete
    for (auto addressFromList = neighborsAddressesList.begin(); addressFromList != neighborsAddressesList.end() ;addressFromList++)
        if ( *addressFromList == addressToDelete )
        {
            // looking in neighbor addresses list address to delete too - "two side disconnect"
            auto beginPos = addressToDelete->neighborsAddressesList.begin();
            auto endPos = addressToDelete->neighborsAddressesList.end();

            for(auto neighborAddressFromHisList = beginPos; neighborAddressFromHisList != endPos; neighborAddressFromHisList++ )
            {
                if( *neighborAddressFromHisList == this )
                {
                    size_t posToDelete = ( neighborAddressFromHisList - addressToDelete->neighborsAddressesList.begin() );
                    addressToDelete->neighborsAddressesList.erase( addressToDelete->neighborsAddressesList.begin() + posToDelete );
                }
            }
            size_t posToDelete = ( addressFromList - neighborsAddressesList.begin() ); // position index of adress in container
            neighborsAddressesList.erase( neighborsAddressesList.begin() + posToDelete );

            return true; // check only once, one neighbor - one address rule
        }
        else
            continue; // keep looking

    return false;
}

bool SimCell::SetNeigborAddress(SimCell * addressToSet)
{
    // cant set own address to self
    if( addressToSet != this )
    {
        // start checking conditions
        // first condition check is exist no empty address list in cell and neighbor
        if( neighborsAddressesList.size() == 0 && addressToSet->neighborsAddressesList.size() == 0 )
        {
            this->neighborsAddressesList.push_back(addressToSet);
            this->numberOfNeighbors++;
            addressToSet->neighborsAddressesList.push_back(this);
            addressToSet->numberOfNeighbors++;
            return true;
        }

        else if( neighborsAddressesList.size() == 0 && addressToSet->neighborsAddressesList.size() > 0 )
        {
            // check is neighbor has free host - nullptr in vector
            if( !(addressToSet->IsFullAddressList()) )
            {
                auto beginPos = addressToSet->neighborsAddressesList.begin();
                auto endPos = addressToSet->neighborsAddressesList.end();
                // check is forwarded address exist in neighbors list
                for( auto addressFromList = beginPos; addressFromList != endPos; addressFromList++ )
                {
                    if( *addressFromList == this )
                    {
                        std::cout << "Adressing error: giving address already exist in neighbor address list.\n";
                        return false; // check only once, one neighbor - one address rule
                    }
                    else
                        continue; // keep looking
                }

                // and looking for free host
                for( auto addressFromList = beginPos; addressFromList != endPos; addressFromList++ )
                {
                    if( *addressFromList == nullptr ) // free host
                    {
                        *addressFromList = this;
                        (*addressFromList)->numberOfNeighbors++;
                        break;
                    }
                    else
                        continue; // keep looking
                }
                // must find because function IsFullAddressList return false is find nullptr position in vector
                this->neighborsAddressesList.push_back(addressToSet);
                this->numberOfNeighbors++;
                return true;
            }
            else
            {
                // if not found free host in neighbor cell check is givin address exist in neighbor list, if not add and return true
                auto beginPos = addressToSet->neighborsAddressesList.begin();
                auto endPos = addressToSet->neighborsAddressesList.end();
                // check is forwarded address exist in neighbors list
                for( auto addressFromList = beginPos; addressFromList != endPos; addressFromList++ )
                {
                    if( *addressFromList == this )
                    {
                        std::cout << "Adressing error: giving address already exist in neighbor address list.\n";
                        return false; // check only once, one neighbor - one address rule
                    }
                    else
                        continue; // keep looking
                }
                // if not found add new in this empty cell and full addressed neighbor list
                addressToSet->neighborsAddressesList.push_back(this);
                addressToSet->numberOfNeighbors++;
                this->neighborsAddressesList.push_back(addressToSet);
                this->numberOfNeighbors++;

                return true;
            }
        }

        else if( neighborsAddressesList.size() > 0 && addressToSet->neighborsAddressesList.size() == 0 )
        {
            // check is forwarded address exist in cell (this) list
            for (auto addressFromList = neighborsAddressesList.begin(); addressFromList != neighborsAddressesList.end(); addressFromList++)
            {
                if ( *addressFromList == nullptr ) // free host
                {
                    *addressFromList = addressToSet;
                    break;
                }
                else
                    continue; // keep looking

                // if not found nullptr position in vector add one with givin address
                this->neighborsAddressesList.push_back(addressToSet);
                this->numberOfNeighbors++;
            }
            addressToSet->neighborsAddressesList.push_back(this);
            addressToSet->numberOfNeighbors++;
            return true;
        }

        else if( neighborsAddressesList.size() > 0 && addressToSet->neighborsAddressesList.size() > 0 )
        {
            // check is exist free hosts in cell and neighbor lists
            // have both free addresses
            if ( !(this->IsFullAddressList()) && !(addressToSet->IsFullAddressList()) )
            {
                // check is forwarded address exist in cell (this) list and neighbor list
                for (auto addressFromList = neighborsAddressesList.begin(); addressFromList != neighborsAddressesList.end() ;addressFromList++)
                {
                    if ( *addressFromList == addressToSet )
                    {
                        std::cout << "Adressing error: giving address already exist in both address list.\n";
                        return false; // check only once, one neighbor - one address rule
                    }
                    else if( *addressFromList == nullptr ) // free host
                    {
                        // looking for free host in neighbor address list
                        auto beginPos = addressToSet->neighborsAddressesList.begin();
                        auto endPos = addressToSet->neighborsAddressesList.end();
                        for( auto addressFromList = beginPos; addressFromList != endPos ;addressFromList++ )
                        {
                            // addressFromList is local here, overshadows the variable
                            if( *addressFromList == nullptr ) // free host
                            {
                                *addressFromList = this;
                                (*addressFromList)->numberOfNeighbors++;
                                break;
                            }
                            else
                                continue; // keep looking, must find free host because IsAllNeighborsHasAddress return false (mean cell has free host, not full)
                        }
                        *addressFromList = addressToSet;
                        (*addressFromList)->numberOfNeighbors++;
                        return true;
                    }
                    else
                        continue; // keep looking
                }
            }

            // first has free (this)
            else if ( !(this->IsFullAddressList()) )
            {
                // check is forwarded address exist in cell (this) list
                for (auto addressFromList = neighborsAddressesList.begin(); addressFromList != neighborsAddressesList.end() ;addressFromList++)
                {
                    if ( *addressFromList == addressToSet )
                    {
                        std::cout << "Adressing error: giving address already exist in both address list.\n";
                        return false; // check only once, one neighbor - one address rule
                    }
                    else if ( *addressFromList == nullptr ) // free host
                    {
                        *addressFromList = addressToSet;
                        (*addressFromList)->numberOfNeighbors++;
                        break;
                    }
                    else
                        continue; // keep looking
                }
                addressToSet->neighborsAddressesList.push_back(this);
                addressToSet->numberOfNeighbors++;
                return true;
            }

            // second has free
            else if( !(addressToSet->IsFullAddressList()) )
            {
                auto beginPos = addressToSet->neighborsAddressesList.begin();
                auto endPos = addressToSet->neighborsAddressesList.end();
                // check is forwarded address exist in neighbors list
                for( auto addressFromList = beginPos; addressFromList != endPos ;addressFromList++ )
                {
                    if( *addressFromList == this )
                    {
                        std::cout << "Adressing error: giving address already exist in neighbor address list.\n";
                        return false; // check only once, one neighbor - one address rule
                    }
                    else if( *addressFromList == nullptr ) // free host
                    {
                        *addressFromList = this;
                        (*addressFromList)->numberOfNeighbors++;
                        break;
                    }
                    else
                        continue; // keep looking
                }
                this->neighborsAddressesList.push_back(addressToSet);
                this->numberOfNeighbors++;
                return true;
            }

            // no one has free
            else
            {   // check is argument address exist in vector
                for (auto addressFromList = neighborsAddressesList.begin(); addressFromList != neighborsAddressesList.end() ;addressFromList++)
                {
                    // check is giving address exist in neighbors list
                    // attention - dont check this in neighbor vector list, only self vector
                    if ( *addressFromList == addressToSet )
                    {
                        std::cout << "Adressing error: giving address already exist in both address list.\n";
                        return false; // check only once, one neighbor - one address rule
                    }
                    else
                        continue; // keep looking
                }
                this->neighborsAddressesList.push_back(addressToSet);
                this->numberOfNeighbors++;
                addressToSet->neighborsAddressesList.push_back(this);
                addressToSet->numberOfNeighbors++;
                return true;
            }
        }
    }
    else
    {
        std::cout << "Addressing error: Cannot set own address to self\n";
        return false;
    }
}

bool SimCell::IsAllNeighborsAreDead()
{
    if( neighborsAddressesList.empty() )
        std::cout << "Check error: neighbors addresses list is empty\n";
    else
        for ( auto& neighbor : neighborsAddressesList )
            // if neighbor cell life - return false
            if( neighbor->cellStatus )
                return false;
            else
                continue;

        return true;
}

std::vector<SimCell *> SimCell::ShowNeighborsAddresses() const { return neighborsAddressesList; }

void SimCell::SetState(bool state)
{
    cellStatus = state;
}

void SimCell::ShowNeighbors() const
{
    if (neighborsAddressesList.size() == 0)
        std::cout << "No neighbors, nothing to show.\n";
    else
    {
        int i = 1;
        for (auto& address : neighborsAddressesList)
        {
            std::cout << i++ << ". Address:\t" << address;
            if( address == nullptr )
                std::cout << "\t\t ---> empty\n";
            else
                std::cout << "\t ---> " << address->label << '\n';
        }
    }
}

void SimCell::AddOneFreeAddressMore()
{
    this->neighborsAddressesList.push_back(nullptr);
    numberOfNeighbors++;
}

int SimCell::GetNumberOfNeighbors()
{
    return numberOfNeighbors;
}

void SimCell::ShowLabel() const
{
    std::cout << label;
}

template <>
void SimCell::ShowState<isNcurses::ON_NCURSES>() const
{

    if( showSelect )
    {	
	// in select mode 
        attron(A_REVERSE);
        attron(A_BLINK);
        if( this->cellStatus )
            addch('O'); // life
        else
            addch('_'); // death
        attroff(A_REVERSE);
        attroff(A_BLINK);
    }
    else
    {
        if( this->cellStatus )
            addch('O'); // life
        else
            addch('_'); // death
    }
}


template <>
void SimCell::ShowState<isNcurses::OFF_NCURSES>() const
{
    if( this->cellStatus )
        std::cout << 'O'; // life
    else
        std::cout << '_'; // death
}

void SimCell::SetLabel(const char * cellLabel)
{
    label = cellLabel;
}

void SimCell::SetLabel(std::string cellLabel)
{
    label = cellLabel;
}

void SimCell::SetRules(unsigned int kill, unsigned int resp)
{

    killRules.clear();
    respawnRules.clear();

    // number example 3456 to element 3 4 5 6 in vector, zeros are delete
    do
    {
        if( kill != 0 )
            killRules.push_back( kill % 10 );
        kill /= 10;

        if( resp != 0 )
            respawnRules.push_back( resp % 10 );
        resp /= 10;
    }   while ( kill > 0 || resp > 0 );

    // add at the end of vector "check numer" - if MakeAnalize() no find conditions vector returns this number
    killRules.push_back(10);
    respawnRules.push_back(10);

    // sorts and delete duplicates
    std::sort( killRules.begin(), killRules.end() );
    killRules.erase( unique( killRules.begin(), killRules.end() ), killRules.end() );

    std::sort( respawnRules.begin(), respawnRules.end() );
    respawnRules.erase( unique( respawnRules.begin(), respawnRules.end() ), respawnRules.end() );
}

void SimCell::SeeRules()
{
    std::cout << "Kill rules:\n";
    for ( auto& killr : killRules )
    {
        std::cout << killr << ' ';
    };

    std::cout << "\n\nRespawn rules:\n";
    for ( auto& respr : respawnRules )
    {
        std::cout << respr << ' ';
    };
}

void SimCell::MakeAnalize()
{
    if( neighborsAddressesList.size() == 0 )
        std::cout << "Analize error: neighbor address list is empty\n";
    else
        if( cellStatus ) // if this cell life
        {
            // first count life neighbors
            for( auto& cellToAnalize : neighborsAddressesList )
                if( cellToAnalize->cellStatus )
                    killCounter++;
                else
                    continue;

            // second check is counter satisfied conditions
            if( killRules.end() != std::find( killRules.begin(), killRules.end(), killCounter ))
                this->nextStepCellStatus = true;
            else
                this->nextStepCellStatus = false;

            killCounter = 0;
        }
        else // if this cell is death
        {
            for( auto& cellToAnalize : neighborsAddressesList )
                if( cellToAnalize->cellStatus )
                    respawnCounter++;
                else
                    continue;

            if( respawnRules.end() != std::find( respawnRules.begin(), respawnRules.end(), respawnCounter ) )
                this->nextStepCellStatus = true;
            else
                this->nextStepCellStatus = false;

            respawnCounter = 0;
        }
}

void SimCell::MakeUpdate()
{
    cellStatus = nextStepCellStatus;
}
