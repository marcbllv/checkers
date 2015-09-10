CC = g++
CXXFLAGS = -Wall -std=c++0x 

EXE = checkers
OBJFILES = main.o player.o gamestate.o node.o

all: $(EXE)

$(EXE): $(OBJFILES)
	$(LINK.cpp) $(LOADLIBES) $(LDLIBS) $^ -o $@

game:
	rm -f pipe && mkfifo pipe
	./checkers init verbose < pipe | ./checkers > pipe

clean:
	rm -f $(EXE)
	rm -f $(OBJFILES)
	rm -f pipe
