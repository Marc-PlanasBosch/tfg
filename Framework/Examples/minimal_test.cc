#include "../Core/GameDefinition.hh"
#include <iostream>

int main() {
    std::cout << "=== Test Mínim ===" << std::endl;
    
    std::cout << "1. Creant GameDefinition..." << std::endl;
    GameDefinition game_def;
    std::cout << "   GameDefinition creat correctament!" << std::endl;
    
    std::cout << "2. Intentant carregar configuració..." << std::endl;
    bool result = game_def.loadFromFile("dominator_game_def.cnf");
    std::cout << "   Resultat: " << (result ? "OK" : "ERROR") << std::endl;
    
    std::cout << "=== Test Completat ===" << std::endl;
    return 0;
}
