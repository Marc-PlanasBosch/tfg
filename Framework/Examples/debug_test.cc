#include "../Core/GameDefinition.hh"
#include <iostream>
#include <memory>
#include <fstream>

/**
 * Test de debug per a veure exactament què passa al carregar la configuració
 */
int main() {
    std::cout << "=== Test de Debug ===" << std::endl;
    
    try {
        std::cout << "1. Creant GameDefinition..." << std::endl;
        auto game_def = std::make_shared<GameDefinition>();
        std::cout << "   GameDefinition creat correctament!" << std::endl;
        
        std::cout << "2. Verificant fitxer..." << std::endl;
        std::ifstream test_file("dominator_game_def.cnf");
        if (!test_file.is_open()) {
            std::cerr << "   Error: No es pot obrir el fitxer!" << std::endl;
            return 1;
        }
        std::cout << "   Fitxer obert correctament!" << std::endl;
        
        std::cout << "3. Llegint primeres línies..." << std::endl;
        std::string line;
        int line_count = 0;
        while (std::getline(test_file, line) && line_count < 10) {
            std::cout << "   Línia " << line_count + 1 << ": " << line << std::endl;
            line_count++;
        }
        test_file.close();
        
        std::cout << "4. Intentant carregar configuració..." << std::endl;
        bool result = game_def->loadFromFile("dominator_game_def.cnf");
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
