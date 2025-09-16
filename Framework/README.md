# Framework Genèric de Jocs

Aquest framework permet crear jocs de manera declarativa utilitzant fitxers de configuració, separant la definició del joc de la definició del mapa/partida.

## Estructura del Framework

```
Framework/
├── Core/                    # Classes base del framework
│   ├── GameDefinition.hh   # Defineix les regles i tipus d'unitats del joc
│   ├── GameDefinition.cc   # Implementació
│   ├── GameMap.hh          # Gestiona mapes i partides
│   ├── GameMap.cc          # Implementació
│   ├── GameBoard.hh        # Tauler del joc amb lògica genèrica
│   └── GameBoard.cc        # Implementació
├── Examples/                # Exemples d'ús
│   ├── dominator_game_def.cnf  # Definició del joc Dominator
│   ├── dominator_map.cnf       # Mapa específic per Dominator
│   └── main_example.cc         # Exemple d'ús del framework
└── Makefile                 # Compilació del framework
```

## Conceptes Clau

### 1. Definició del Joc (`game_def.cnf`)
Defineix les regles, tipus d'unitats i mecàniques del joc:
- **Tipus d'unitats**: Farmers, Knights, Witches, etc.
- **Regles del joc**: Captura de territori, combat, efectes especials
- **Mecàniques**: Sistemes de regeneració, habilitats especials
- **Constants**: Valors numèrics del joc

### 2. Mapa/Partida (`map.cnf`)
Defineix una partida específica dins d'un joc:
- **Capçalera**: Nom, dimensions, nombre de jugadors, rondes
- **Graella**: Murs, cel·les especials
- **Unitats**: Posicions inicials de les unitats
- **Objectes**: Bonus, power-ups, etc.

## Format dels Fitxers de Configuració

### Definició del Joc

```ini
[GAME]
game_name=Dominator
version=4.0
max_players=4
default_rounds=200

[UNIT_TYPES]
# Format: name symbol max_health initial_health regen_rate movement_range attack_range attack_power defense can_capture can_move can_attack [abilities...]
Farmer f 100 100 30 1 1 0 0 1 1 0
Knight k 200 200 30 1 1 75 0 0 1 1

[RULES]
capture_territory territory_control capture_range=1 capture_points=10
unit_combat combat damage_min=60 damage_max=90

[MECHANICS]
territory_control "Els farmers poden capturar territori" Farmer can_capture=1
combat_system "Els knights poden atacar" Knight can_attack=1
```

### Mapa/Partida

```ini
[MAP_HEADER]
map_name=Classic_Map
rows=37
cols=37
nb_players=4
nb_rounds=200

[MAP_GRID]
# Format: i j cell_type [owner] [unit_id] [prop1=val1 prop2=val2 ...]
0 0 Wall
1 1 Empty

[MAP_UNITS]
# Format: unit_type player_id x y health [prop1=val1 prop2=val2 ...]
Farmer 0 2 2 100
Knight 0 5 5 200

[MAP_OBJECTS]
# Format: object_type x y [prop1=val1 prop2=val2 ...]
Bonus 10 10 points=50
```

## Ús del Framework

### 1. Compilar

```bash
cd Framework
make all
```

### 2. Executar l'Exemple

```bash
make run
```

### 3. Crear un Nou Joc

```cpp
#include "Core/GameDefinition.hh"
#include "Core/GameMap.hh"
#include "Core/GameBoard.hh"

int main() {
    // Carregar definició del joc
    auto game_def = std::make_shared<GameDefinition>();
    game_def->loadFromFile("my_game.cnf");
    
    // Carregar mapa
    auto game_map = std::make_shared<GameMap>();
    game_map->loadFromFile("my_map.cnf");
    
    // Crear tauler
    auto board = std::make_shared<GameBoard>(game_def, game_map);
    board->initialize();
    
    // Jugar!
    // ...
}
```

## Avantatges del Framework

### ✅ **Reutilització de Codi (80%)**
- Sistema de joc genèric
- Gestió d'unitats i accions
- Sistema de rondes i puntuacions
- Validació d'accions

### ✅ **Configuració Declarativa (20%)**
- Fitxers de configuració llegibles
- Fàcil de modificar i estendre
- Separació de regles i contingut
- Suport per múltiples mapes

### ✅ **Extensibilitat**
- Classes base amb mètodes virtuals
- Fàcil d'afegir noves mecàniques
- Suport per habilitats especials
- Sistema de plugins

## Crear un Nou Joc

### 1. Definir les Regles
Crea un fitxer `my_game.cnf` amb:
- Tipus d'unitats
- Regles del joc
- Mecàniques especials

### 2. Crear el Mapa
Crea un fitxer `my_map.cnf` amb:
- Dimensions del tauler
- Posicions inicials
- Objectes especials

### 3. Implementar Lògica Específica (Opcional)
Si necessites lògica específica, hereta de `GameBoard`:

```cpp
class MyGameBoard : public GameBoard {
public:
    MyGameBoard(std::shared_ptr<GameDefinition> def, std::shared_ptr<GameMap> map)
        : GameBoard(def, map) {}
    
protected:
    void processSpecialEffects() override {
        // Implementar efectes especials del teu joc
    }
};
```

## Exemples Inclosos

- **Dominator**: Joc de conquesta de territori amb farmers, knights i witches
- **Pokémon**: Joc de batalles amb criatures
- **Star Wars**: Joc espacial amb naus i míssils
- **Bola de Drac**: Joc d'acció amb personatges

## Compilació i Dependències

- **C++17** o superior
- **GCC** o **Clang**
- **Make** per la compilació

```bash
# Verificar versió de C++
g++ --version

# Compilar
make all

# Executar exemple
make run

# Netejar
make clean
```

## Estructura Recomanada per a un Nou Joc

```
MyGame/
├── my_game.cnf          # Definició del joc
├── maps/                # Mapes disponibles
│   ├── classic.cnf
│   ├── advanced.cnf
│   └── custom.cnf
├── ai/                  # IA dels jugadors
│   ├── AIPlayer1.cc
│   └── AIPlayer2.cc
└── main.cc              # Punt d'entrada
```

## Suport i Extensions

El framework està dissenyat per ser extensible. Pots:

- Afegir nous tipus d'unitats
- Implementar mecàniques especials
- Crear sistemes de puntuació personalitzats
- Afegir efectes visuals i de so
- Integrar amb motors de renderitzat

## Llicència

Aquest framework està dissenyat per a ús educatiu i de desenvolupament de jocs.
