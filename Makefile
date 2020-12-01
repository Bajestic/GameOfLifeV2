# Makefile
CXX := g++

main:	clean out
	g++ -o out src/main.cpp *.o -lncurses

out:
	g++ -c src/SimCell_v2.cpp src/*.h -lncurses

clean:
	rm -f *.o out
