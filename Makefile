CPP=g++
CC=gcc
DBG= -DNO_CHECK  # -DNDEBUG -O0 -g -DNFREE_ISA
CPPFLAGS=-Wall $(DBG) $(FREE)  -O2 -funroll-loops
LINKFLAGS=
CFLAGS=-Wall -ansi
EXE=test
OBJ=sbvtree.o DynamicSuffixArray.o rbtree.o  intTree.o lpermutation.o gRankS.o

all: $(EXE)

$(EXE) : % : %.o $(OBJ)
	$(CPP) $(LINKFLAGS) -o $@ $^

testgRankS : % : %.o $(OBJ)
	$(CPP) $(LINKFLAGS) -o $@ $^

lib: $(OBJ)
	ar rc dsa.a $(OBJ)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $^
%.o : %.c
	$(CC) $(CFLAGS) -c $^
%.cpp : %.h
%.c : %.h

depend:
	g++ -MM *.cpp  > dep.mk

clean: 
	rm -f *.o
cleanall: clean
	rm -f *.gch $(EXE)

include dep.mk
