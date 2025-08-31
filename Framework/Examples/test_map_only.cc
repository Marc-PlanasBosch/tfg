#include "../Core/GameMap.hh"
#include <iostream>
#include <memory>

/**
 * Test només del GameMap per a aïllar el problema
 */
int main() {
    std::cout << "=== Test Només GameMap ===" << std::endl;
    
    try {
        std::cout << "Creant GameMap..." << std::endl;
        auto game_map = std::make_shared<GameMap>();
        std::cout << "GameMap creat correctament!" << std::endl;
        
        std::cout << "Intentant carregar mapa..." << std::endl;
        bool result = game_map->loadFromFile("dominator_map.cnf");
        std::cout << "Resultat de loadFromMap: " << (result ? "OK" : "ERROR") << std::endl;
        
        if (result) {
            std::cout << "Mapa carregat correctament!" << std::endl;
        } else {
            std::cout << "Error carregant el mapa" << std::endl;
        }
        
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
