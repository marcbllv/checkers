CC = g++
CXXFLAGS = -Wall -std=c++0x 

EXE = checkers
OBJFILES = main.o player.o gamestate.o node.o heuristics.o

all: $(EXE)

$(EXE): $(OBJFILES)
	$(LINK.cpp) $(LOADLIBES) $(LDLIBS) $^ -o $@

game:
	rm -f pipe && mkfifo pipe
	./$(EXE) init verbose < pipe | ./agent1 > pipe

game0:
	rm -f pipe && mkfifo pipe
	./$(EXE) init verbose < pipe | ./agent0 > pipe

game7:
	rm -f pipe && mkfifo pipe
	./$(EXE) init verbose < pipe | ./v7 > pipe

game7bis:
	rm -f pipe && mkfifo pipe
	./$(EXE) init verbose < pipe | ./v7bis > pipe

game8:
	rm -f pipe && mkfifo pipe
	./$(EXE) init verbose < pipe | ./v8 > pipe

gamerand:
	rm -f pipe && mkfifo pipe
	./$(EXE) init verbose < pipe | ./randomPlayer > pipe

clean:
	rm -f $(EXE)
	rm -f $(OBJFILES)
	rm -f pipe
