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
    bool SetNumberOfNeighbors(unsigned int);
    void ResetNumberOfNeigbors(); // to zero - zero-vector too
    bool IsFullAddressList();
    int NumberOfFreeNeighborsHost();
    bool DeleteAddress(SimCell *);
    void ShowNeighbors() const;
    void ShowLabel() const;
    void SetLabel(const char *);
    void SetLabel(std::string);
    int GetNumberOfNeighbors();
    bool SetNeigborAddress(SimCell *);
    bool IsAllNeighborsAreDead();
    void SetState(bool);
    void ShowState() const;
    void ShowStateNoNcurses() const;
    void ShowStateSelect() const;
    void ShowSelect(bool show) { showSelect = show; }
    bool ShowSelect() { return showSelect; }
    std::vector<SimCell *> ShowNeighborsAddresses() const;
    void SetRules(unsigned int numberOfNeighborsToKeepLife, unsigned int numberToRespawnDeadCell );
    void SeeRules();
    void MakeAnalize();
    void MakeUpdate();
protected:
private:
    unsigned int killCounter;
    unsigned int respawnCounter;
    std::vector<unsigned int> killRules;
    std::vector<unsigned int> respawnRules;
    unsigned int numberOfNeighbors;
    bool cellStatus;
    bool nextStepCellStatus;
    bool showSelect;
    SimCell * cellAddress;
    std::vector<SimCell *> neighborsAddressesList;
    std::string label;
    void AddOneFreeAddressMore();
};

enum class isNcurses
{
	ON_NCURSES,
	OFF_NCURSES
};

#endif // SIMCELL_V2_H
