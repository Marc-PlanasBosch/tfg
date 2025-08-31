# To be able to play with the Dummy:
#
# 1) Copy one of:
#
# AIDummy.o.Linux64 (Linux   64 bits)
# AIDummy.o.Linux32 (Linux   32 bits)
# AIDummy.o.Win32   (Windows 32 bits)
# AIDummy.o.Win64   (Windows 64 bits)
# AIDummy.o.MacOS   (Mac)
#
# to AIDummy.o
#
# 2) Uncomment the following line.
#
#DUMMY_OBJ = AIDummy.o

# Add here any extra .o player files you want to link to the executable
EXTRA_OBJS = 

# Config
OPTIMIZE = 2 # Optimization level (0 to 3)
DEBUG    = 0 # Compile for debugging (0 or 1)
PROFILE  = 0 # Compile for profile (0 or 1)
32BITS   = 0 # Produce 32 bits objects on 64 bits systems (0 or 1)

# Platform detection
ifeq ($(OS),Windows_NT)
    # Windows
    RM = del /Q
    RM_RF = if exist
    RM_RF_END = (del /Q /S *.*) & (rmdir /S /Q)
    EXE_EXT = .exe
    PLATFORM = WINDOWS
    DEBUGFLAGS_WIN = -DDEBUG -g
else
    # Unix/Linux/MacOS
    RM = rm -f
    RM_RF = rm -rf
    RM_RF_END = 
    EXE_EXT = 
    PLATFORM = UNIX
    DEBUGFLAGS_WIN = -DDEBUG -g -rdynamic
endif

# Do not edit past this line
############################

# The following two lines will detect all your players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) $(EXTRA_OBJS) $(DUMMY_OBJ)

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
ifeq ($(PLATFORM),WINDOWS)
	$(RM) Game$(EXE_EXT) SecGame$(EXE_EXT) *.o Makefile.deps
else
	$(RM_RF) Game SecGame *.o *.exe Makefile.deps
endif

# Run the game with default configuration
run: Game$(EXE_EXT)
ifeq ($(PLATFORM),WINDOWS)
	.\Game$(EXE_EXT) --seed=123 Demo Demo Demo Demo --input=fixed1.cnf --output=game_output.out
else
	./Game --seed=123 Demo Demo Demo Demo --input=fixed1.cnf --output=game_output.out
endif

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
