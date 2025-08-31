#include "../Core/GameDefinition.hh"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Test Shared Ptr ===" << std::endl;
    
    try {
        std::cout << "1. Creant GameDefinition amb make_shared..." << std::endl;
        auto game_def = std::make_shared<GameDefinition>();
        std::cout << "   GameDefinition creat correctament!" << std::endl;
        
        std::cout << "2. Verificant que l'objecte és vàlid..." << std::endl;
        if (game_def) {
            std::cout << "   Punter vàlid!" << std::endl;
        } else {
            std::cout << "   Punter invàlid!" << std::endl;
            return 1;
        }
        
        std::cout << "3. Intentant carregar configuració..." << std::endl;
        bool result = game_def->loadFromFile("dominator_game_def.cnf");
        std::cout << "   Resultat: " << (result ? "OK" : "ERROR") << std::endl;
        
        std::cout << "4. Test completat amb èxit!" << std::endl;
        
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
