# Makefile
CXX := g++
CXXFLAGS := -std=c++17

main:	clean out cleangch
	$(CXX) $(CXXFLAGS) -o out src/main.cpp *.o -lncurses

out:
	$(CXX) $(CXXFLAGS) -c src/SimCell_v2.cpp src/*.h -lncurses

clean:
	rm -f *.o src/*.gch out

cleangch:
	rm -f src/*.gch
