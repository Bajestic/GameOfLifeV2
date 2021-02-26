# GameOfLifeV2
The Game of Life is a cellular automaton devised by the British mathematician John Horton Conway in 1970

# Required
To use keypad for navigate is required lncurses library

# Install lncurses library for enable keypad to navigate
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

# Usage
For build and run use Make command
```bash
./make
```
And next
```bash
./out
```

# Navigate
Arrow Keys - Move around the universe\
Enter Key - Change cell to Life \ Death status\
Space Key - Start \ Stop simulation\
'u' Key - Change size of universe from 10 to 150 (Will Clear universe)\
'r' Key - Change rules of Game Of Life (Conway's rules are default)\
'e' Key - Clear universe\
Ctrl + 'c' - Exit
