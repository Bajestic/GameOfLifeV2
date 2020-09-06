#ifndef SIMCELL_V2_H
#define SIMCELL_V2_H
#include <vector>
#include <string>

class SimCell
{
public:
    SimCell();
    bool CellState() const;
    const SimCell * GetCellAddress() const;

    // Set & get information functions about neighbors
    bool SetNumberOfNeighbors(unsigned int);
    void ResetNumberOfNeigbors(); // to zero
    bool IsAllNeighborsHasAddresses();
    int NumberOfFreeNeighborsHost();
    bool DeleteAddress(SimCell *);
    void ShowNeighbors() const;
    void ShowLabel() const;
    void SetLabel(const char *);
    void SetLabel(std::string);

    int GetNumberOfNeighbors();
    bool SetNeigborAddress(SimCell *);
    void SetState(bool);
    std::vector<SimCell *> ShowNeighborsAddresses() const;
protected:
private:
    unsigned int killCounter;
    unsigned int respawnCounter;
    unsigned int numberOfNeighbors;
    bool cellStatus;
    bool nextStepCellStatus;
    SimCell * cellAddress;
    std::vector<SimCell *> neighborsAddressesList;
    std::string label;

    // private functions
    void AddOneFreeAddressMore();
};

#endif // SIMCELL_V2_H
