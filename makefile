all: bptree

bptree: BPTree.h BPTree.cpp utilFunction.cpp insertion.cpp display.cpp
	g++ ./BPTree.h ./BPTree.cpp ./utilFunction.cpp ./insertion.cpp ./display.cpp -o ./BPTree

clean:
	rm mylogin
