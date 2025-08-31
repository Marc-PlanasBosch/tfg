#include "../Core/GameDefinition.hh"
#include <iostream>
#include <memory>

/**
 * Test simple per a aïllar el problema de segmentation fault
 */
int main() {
    std::cout << "=== Test Simple ===" << std::endl;
    
    try {
        std::cout << "Creant GameDefinition..." << std::endl;
        auto game_def = std::make_shared<GameDefinition>();
        std::cout << "GameDefinition creat correctament!" << std::endl;
        
        std::cout << "Intentant carregar fitxer..." << std::endl;
        bool result = game_def->loadFromFile("dominator_game_def.cnf");
        std::cout << "Resultat de loadFromFile: " << (result ? "OK" : "ERROR") << std::endl;
        
        if (result) {
            std::cout << "Fitxer carregat correctament!" << std::endl;
        } else {
            std::cout << "Error carregant el fitxer" << std::endl;
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
