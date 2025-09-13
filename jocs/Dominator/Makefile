# Simplified Makefile for Dominator
# Assumes g++ and make are available in PATH

# Config
OPTIMIZE = 2 # Optimization level (0 to 3)
DEBUG    = 0 # Compile for debugging (0 or 1)
PROFILE  = 0 # Compile for profile (0 or 1)
32BITS   = 0 # Produce 32 bits objects on 64 bits systems (0 or 1)

# Platform detection
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    RM = del /Q
else
    EXE_EXT = 
    RM = rm -f
endif

# Compiler
CXX = g++

# The following two lines will detect all your players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC))

# Flags

ifeq ($(strip $(PROFILE)),1)
	PROFILEFLAGS=-pg
endif
ifeq ($(strip $(DEBUG)),1)
ifeq ($(PLATFORM),WINDOWS)
	DEBUGFLAGS=$(DEBUGFLAGS_WIN)
else
	DEBUGFLAGS=$(DEBUGFLAGS_WIN)
endif
endif
ifeq ($(strip $(32BITS)),1)
	ARCHFLAGS=-m32 -L/usr/lib32
endif

CXXFLAGS = -std=c++11 -Wall -Wno-unused-variable $(ARCHFLAGS) $(PROFILEFLAGS) $(DEBUGFLAGS) -O$(strip $(OPTIMIZE))

LDFLAGS  = -std=c++11 -lm $(ARCHFLAGS) $(PROFILEFLAGS) $(DEBUGFLAGS) -O$(strip $(OPTIMIZE))

# Rules

all: Game$(EXE_EXT)

clean:
	$(RM) Game$(EXE_EXT) SecGame$(EXE_EXT) *.o Makefile.deps

# Run the game with default configuration (4 Demo players)
run: Game$(EXE_EXT)
	./Game$(EXE_EXT) --seed=123 Demo Demo Demo Demo --input=fixed1.cnf --output=game_output.out


# Order of objects is important here to deactivate standard sleep function.

Game$(EXE_EXT): Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o Game.o Main.o $(PLAYERS_OBJ) Utils.o 
	$(CXX) $^ -o $@ $(LDFLAGS)

SecGame$(EXE_EXT): Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o SecGame.o SecMain.o Utils.o 
	$(CXX) $^ -o $@ $(LDFLAGS) -lrt

%.exe: %.o Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o SecGame.o SecMain.o Utils.o
	$(CXX) $^ -o $@ $(LDFLAGS) -lrt

Makefile.deps: *.cc
	$(CXX) $(CXXFLAGS) -MM *.cc > Makefile.deps

include Makefile.deps
