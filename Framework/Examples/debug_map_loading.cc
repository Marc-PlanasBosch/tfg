#include "../Core/GameMap.hh"
#include <iostream>
#include <fstream>

/**
 * Test de debug per a veure exactament què passa durant la càrrega del mapa
 */
int main() {
    std::cout << "=== Test de Debug de Càrrega de Mapa ===" << std::endl;
    
    try {
        std::cout << "1. Obrint fitxer del mapa..." << std::endl;
        std::ifstream file("dominator_map.cnf");
        if (!file.is_open()) {
            std::cerr << "Error: No es pot obrir el fitxer del mapa!" << std::endl;
            return 1;
        }
        std::cout << "   Fitxer del mapa obert correctament!" << std::endl;
        
        std::cout << "2. Llegint primeres línies del mapa..." << std::endl;
        std::string line;
        int line_count = 0;
        while (std::getline(file, line) && line_count < 15) {
            std::cout << "   Línia " << line_count + 1 << ": " << line << std::endl;
            line_count++;
        }
        file.close();
        
        std::cout << "3. Creant GameMap..." << std::endl;
        GameMap game_map;
        std::cout << "   GameMap creat correctament!" << std::endl;
        
        std::cout << "4. Intentant carregar mapa..." << std::endl;
        bool result = game_map.loadFromFile("dominator_map.cnf");
        std::cout << "   Resultat: " << (result ? "OK" : "ERROR") << std::endl;
        
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
