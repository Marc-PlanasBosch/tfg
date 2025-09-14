# Makefile simple per al Framework Genèric de Jocs

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
INCLUDES = -I./Core
LIBS = 

# Extensió d'executable (per Windows)
EXE_EXT = .exe

# Directoris
CORE_DIR = Core
EXAMPLES_DIR = Examples
BUILD_DIR = build

# Fitxers font
CORE_SOURCES = $(wildcard $(CORE_DIR)/*.cc)
EXAMPLES_SOURCES = $(wildcard $(EXAMPLES_DIR)/*.cc)

# Fitxers objecte
CORE_OBJECTS = $(CORE_SOURCES:$(CORE_DIR)/%.cc=$(BUILD_DIR)/%.o)
EXAMPLES_OBJECTS = $(EXAMPLES_SOURCES:$(EXAMPLES_DIR)/%.cc=$(BUILD_DIR)/%.o)

# Executables
EXAMPLES = $(EXAMPLES_SOURCES:$(EXAMPLES_DIR)/%.cc=$(BUILD_DIR)/%$(EXE_EXT))

# Executables principals
DOMINATOR_EXE = $(BUILD_DIR)/dominator_game$(EXE_EXT)

# Objectiu principal
all: $(BUILD_DIR) $(EXAMPLES) $(DOMINATOR_EXE)

# Crear directori de build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilar classes base
$(BUILD_DIR)/%.o: $(CORE_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compilar exemples
$(BUILD_DIR)/%$(EXE_EXT): $(EXAMPLES_DIR)/%.cc $(CORE_OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(CORE_OBJECTS) -o $@ $(LIBS)

# Compilar dominator
$(DOMINATOR_EXE): dominator_main.cc $(CORE_OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(CORE_OBJECTS) -o $@ $(LIBS)

# Objectiu per executar l'exemple
run: $(BUILD_DIR)/simple_game$(EXE_EXT)
	cd $(BUILD_DIR) && ./simple_game

# Netejar
clean:
	rm -rf $(BUILD_DIR)

# Mostrar ajuda
help:
	@echo "Makefile del Framework Genèric de Jocs"
	@echo ""
	@echo "Objectius disponibles:"
	@echo "  all        - Compila tot el framework i exemples"
	@echo "  run        - Compila i executa l'exemple simple"
	@echo "  clean      - Neteja els fitxers compilats"
	@echo "  help       - Mostra aquesta ajuda"

# Dependències
.PHONY: all clean help run