CPP=g++
CC=gcc
FREE=
DBG= -DNDEBUG -DNO_CHECK  #  -O0 -g -DNFREE_ISA
CPPFLAGS=-Wall $(DBG) $(FREE)  -O2 -funroll-loops
LINKFLAGS=
CFLAGS=-Wall -ansi
EXE=test slidingWindow locate
OBJ=sbvtree.o  rbtree.o  intTree.o lpermutation.o  DSASampling.o dynSA.o gRankS.o  lcp.o utils.o

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
