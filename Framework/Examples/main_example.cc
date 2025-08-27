#include "../Core/GameDefinition.hh"
#include "../Core/GameMap.hh"
#include "../Core/GameBoard.hh"
#include <iostream>
#include <memory>

/**
 * Exemple d'ús del framework genèric per crear un joc
 */
int main() {
    std::cout << "=== Framework Genèric de Jocs ===" << std::endl;
    
    // 1. Carregar la definició del joc
    auto game_def = std::make_shared<GameDefinition>();
    if (!game_def->loadFromFile("dominator_game_def.cnf")) {
        std::cerr << "Error carregant la definició del joc" << std::endl;
        return 1;
    }
    
    std::cout << "Joc carregat: " << game_def->getGameDescription() << std::endl;
    
    // 2. Carregar el mapa
    auto game_map = std::make_shared<GameMap>();
    if (!game_map->loadFromFile("dominator_map.cnf")) {
        std::cerr << "Error carregant el mapa" << std::endl;
        return 1;
    }
    
    std::cout << "Mapa carregat: " << game_map->getMapSummary() << std::endl;
    
    // 3. Crear el tauler del joc
    auto game_board = std::make_shared<GameBoard>(game_def, game_map);
    if (!game_board->initialize()) {
        std::cerr << "Error inicialitzant el tauler" << std::endl;
        return 1;
    }
    
    std::cout << "Tauler inicialitzat correctament" << std::endl;
    
    // 4. Simular una partida
    std::cout << "\n=== Iniciant Partida ===" << std::endl;
    
    for (int round = 0; round < 5; ++round) {
        std::cout << "\n--- Ronda " << round << " ---" << std::endl;
        
        // Crear accions d'exemple per als jugadors
        std::vector<GameBoard::GameAction> actions;
        
        // Acció d'exemple: moure una unitat
        GameBoard::GameAction move_action(0, "move");
        move_action.parameters["x"] = "3";
        move_action.parameters["y"] = "3";
        actions.push_back(move_action);
        
        // Acció d'exemple: atacar
        GameBoard::GameAction attack_action(1, "attack");
        attack_action.parameters["target"] = "5";
        actions.push_back(attack_action);
        
        // Executar la ronda
        if (!game_board->next(actions, std::cout)) {
            std::cout << "Fi del joc!" << std::endl;
            break;
        }
        
        // Mostrar estat del tauler
        game_board->printState(std::cout);
    }
    
    // 5. Mostrar resultats finals
    std::cout << "\n=== Resultats Finals ===" << std::endl;
    game_board->printResults(std::cout);
    
    return 0;
}
