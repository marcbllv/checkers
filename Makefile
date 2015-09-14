CC = g++
CXXFLAGS = -Wall -std=c++0x 

EXE = checkers2
OBJFILES = main.o player.o gamestate.o node.o heuristics.o

all: $(EXE)

$(EXE): $(OBJFILES)
	$(LINK.cpp) $(LOADLIBES) $(LDLIBS) $^ -o $@

game:
	rm -f pipe && mkfifo pipe
	./checkers2 init verbose < pipe | ./randomPlayer > pipe

game0:
	rm -f pipe && mkfifo pipe
	./checkers init verbose < pipe | ./agent0 > pipe

clean:
	rm -f $(EXE)
	rm -f $(OBJFILES)
	rm -f pipe
