CC = g++
CXXFLAGS = -Wall

EXE = checkers
OBJFILES = main.o player.o gamestate.o

all: $(EXE)

$(EXE): $(OBJFILES)
	$(LINK.cpp) $(LOADLIBES) $(LDLIBS) $^ -o $@

clean:
	rm -f $(EXE)
	rm -f $(OBJFILES)
