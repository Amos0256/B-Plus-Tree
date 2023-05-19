all: bptree

bptree: BPTree.o utilFunction.o insertion.o delete.o display.o
	g++ ./BPTree.o ./utilFunction.o ./insertion.o ./delete.o ./display.o -o BPTree

BPTree.o: BPTree.h BPTree.cpp
	g++ -c BPTree.h BPTree.cpp

utilFunction.o: BPTree.h utilFunction.cpp
	g++ -c BPTree.h utilFunction.cpp

insertion.o: BPTree.h insertion.cpp
	g++ -c BPTree.h insertion.cpp

delete.o: BPTree.h delete.cpp
	g++ -c BPTree.h delete.cpp

display.o: BPTree.h display.cpp
	g++ -c BPTree.h display.cpp

clean:
	rm -f *.o bptree
