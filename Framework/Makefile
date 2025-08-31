# Makefile per al Framework Genèric de Jocs
# Compila les classes base i l'exemple d'ús

# Configuració
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
INCLUDES = -I./Core
LIBS = 

# Platform detection
ifeq ($(OS),Windows_NT)
    # Windows
    RM = del /Q
    RM_RF = rmdir /S /Q
    EXE_EXT = .exe
    PLATFORM = WINDOWS
    MKDIR = mkdir
else
    # Unix/Linux/MacOS
    RM = rm -f
    RM_RF = rm -rf
    EXE_EXT = 
    PLATFORM = UNIX
    MKDIR = mkdir -p
endif

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

# Objectiu principal
all: $(BUILD_DIR) $(EXAMPLES)

# Crear directori de build
$(BUILD_DIR):
ifeq ($(PLATFORM),WINDOWS)
	$(MKDIR) $(BUILD_DIR)
	$(MKDIR) $(BUILD_DIR)\Core
	$(MKDIR) $(BUILD_DIR)\Examples
else
	$(MKDIR) $(BUILD_DIR)
	$(MKDIR) $(BUILD_DIR)/Core
	$(MKDIR) $(BUILD_DIR)/Examples
endif

# Compilar classes base
$(BUILD_DIR)/%.o: $(CORE_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compilar exemples
$(BUILD_DIR)/%$(EXE_EXT): $(EXAMPLES_DIR)/%.cc $(CORE_OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(CORE_OBJECTS) -o $@ $(LIBS)

# Objectiu per executar l'exemple
run: $(BUILD_DIR)/main_example$(EXE_EXT)
ifeq ($(PLATFORM),WINDOWS)
	cd $(BUILD_DIR) && .\main_example$(EXE_EXT)
else
	cd $(BUILD_DIR) && ./main_example
endif

# Netejar
clean:
ifeq ($(PLATFORM),WINDOWS)
	$(RM_RF) $(BUILD_DIR)
else
	$(RM_RF) $(BUILD_DIR)
endif

# Instal·lar (opcional)
install:
	@echo "Instal·lant headers del framework..."
	@mkdir -p /usr/local/include/GameFramework
	@cp $(CORE_DIR)/*.hh /usr/local/include/GameFramework/
	@echo "Framework instal·lat a /usr/local/include/GameFramework/"

# Desinstal·lar
uninstall:
	@echo "Desinstal·lant framework..."
	@rm -rf /usr/local/include/GameFramework
	@echo "Framework desinstal·lat"

# Mostrar ajuda
help:
	@echo "Makefile del Framework Genèric de Jocs"
	@echo ""
	@echo "Objectius disponibles:"
	@echo "  all        - Compila tot el framework i exemples"
	@echo "  run        - Compila i executa l'exemple principal"
	@echo "  clean      - Neteja els fitxers compilats"
	@echo "  install    - Instal·la els headers del framework"
	@echo "  uninstall  - Desinstal·la el framework"
	@echo "  help       - Mostra aquesta ajuda"
	@echo ""
	@echo "Exemple d'ús:"
	@echo "  make run   # Compila i executa"

# Dependències
$(BUILD_DIR)/main_example: $(CORE_OBJECTS)

# Regles especials
.PHONY: all clean install uninstall help run
