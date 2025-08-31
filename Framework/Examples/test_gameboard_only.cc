#include "../Core/GameBoard.hh"
#include "../Core/GameDefinition.hh"
#include "../Core/GameMap.hh"
#include <iostream>
#include <memory>

/**
 * Test només del GameBoard per a aïllar el problema
 */
int main() {
    std::cout << "=== Test Només GameBoard ===" << std::endl;
    
    try {
        std::cout << "1. Creant GameDefinition..." << std::endl;
        auto game_def = std::make_shared<GameDefinition>();
        std::cout << "   GameDefinition creat correctament!" << std::endl;
        
        std::cout << "2. Carregant definició del joc..." << std::endl;
        if (!game_def->loadFromFile("dominator_game_def.cnf")) {
            std::cerr << "   Error carregant la definició del joc" << std::endl;
            return 1;
        }
        std::cout << "   Definició del joc carregada correctament!" << std::endl;
        
        std::cout << "3. Creant GameMap..." << std::endl;
        auto game_map = std::make_shared<GameMap>();
        std::cout << "   GameMap creat correctament!" << std::endl;
        
        std::cout << "4. Carregant mapa del joc..." << std::endl;
        if (!game_map->loadFromFile("dominator_map.cnf")) {
            std::cerr << "   Error carregant el mapa del joc" << std::endl;
            return 1;
        }
        std::cout << "   Mapa del joc carregat correctament!" << std::endl;
        
        std::cout << "5. Creant GameBoard..." << std::endl;
        auto game_board = std::make_shared<GameBoard>(game_def, game_map);
        std::cout << "   GameBoard creat correctament!" << std::endl;
        
        std::cout << "6. Inicialitzant GameBoard..." << std::endl;
        if (!game_board->initialize()) {
            std::cerr << "   Error inicialitzant el GameBoard" << std::endl;
            return 1;
        }
        std::cout << "   GameBoard inicialitzat correctament!" << std::endl;
        
        std::cout << "7. Test completat amb èxit!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Excepció capturada: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Excepció desconeguda capturada" << std::endl;
        return 1;
    }
    
    std::cout << "=== Test Completat ===" << std::endl;
    return 0;
}
