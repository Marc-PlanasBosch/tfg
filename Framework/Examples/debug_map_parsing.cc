#include "../Core/GameMap.hh"
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * Test de debug per a veure exactament què passa durant la càrrega del mapa
 */
int main() {
    std::cout << "=== Test de Debug de Càrrega de Mapa ===" << std::endl;
    
    try {
        std::cout << "1. Creant GameMap..." << std::endl;
        GameMap game_map;
        std::cout << "   GameMap creat correctament!" << std::endl;
        
        std::cout << "2. Obrint fitxer..." << std::endl;
        std::ifstream file("dominator_map.cnf");
        if (!file.is_open()) {
            std::cerr << "Error: No es pot obrir el fitxer!" << std::endl;
            return 1;
        }
        std::cout << "   Fitxer obert correctament!" << std::endl;
        
        std::cout << "3. Llegint fitxer línia per línia..." << std::endl;
        std::string line;
        std::string current_section = "";
        int line_number = 0;
        
        while (std::getline(file, line)) {
            line_number++;
            
            // Ignorar línies buides i comentaris
            if (line.empty() || line[0] == '#') {
                std::cout << "   Línia " << line_number << " (comentari/buit): " << line << std::endl;
                continue;
            }
            
            // Eliminar espais en blanc al principi i final
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            
            if (line.empty()) {
                std::cout << "   Línia " << line_number << " (només espais): " << line << std::endl;
                continue;
            }
            
            // Detectar seccions
            if (line[0] == '[' && line[line.length()-1] == ']') {
                current_section = line.substr(1, line.length()-2);
                std::cout << "   Línia " << line_number << " (nova secció): [" << current_section << "]" << std::endl;
                continue;
            }
            
            // Mostrar línies de dades
            if (current_section == "MAP_HEADER") {
                std::cout << "   Línia " << line_number << " (MAP_HEADER): " << line << std::endl;
            } else if (current_section == "MAP_GRID") {
                std::cout << "   Línia " << line_number << " (MAP_GRID): " << line << std::endl;
            } else {
                std::cout << "   Línia " << line_number << " (secció desconeguda): " << line << std::endl;
            }
        }
        
        file.close();
        std::cout << "4. Fitxer llegit completament!" << std::endl;
        
        std::cout << "5. Intentant carregar mapa amb GameMap..." << std::endl;
        bool result = game_map.loadFromFile("dominator_map.cnf");
        std::cout << "   Resultat: " << (result ? "OK" : "ERROR") << std::endl;
        
        if (result) {
            std::cout << "6. Mostrant resum del mapa..." << std::endl;
            std::cout << game_map.getMapSummary() << std::endl;
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
