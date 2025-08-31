#include "../Core/GameDefinition.hh"
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * Test de debug per a veure exactament què passa durant el parsing
 */
int main() {
    std::cout << "=== Test de Debug de Parsing ===" << std::endl;
    
    try {
        std::cout << "1. Obrint fitxer..." << std::endl;
        std::ifstream file("dominator_game_def.cnf");
        if (!file.is_open()) {
            std::cerr << "Error: No es pot obrir el fitxer!" << std::endl;
            return 1;
        }
        std::cout << "   Fitxer obert correctament!" << std::endl;
        
        std::cout << "2. Simulant el parsing..." << std::endl;
        std::string line;
        std::string current_section = "";
        int line_number = 0;
        
        while (std::getline(file, line)) {
            line_number++;
            std::cout << "   Línia " << line_number << ": " << line << std::endl;
            
            // Ignorar línies buides i comentaris
            if (line.empty() || line[0] == '#') {
                std::cout << "     -> Ignorada (buit o comentari)" << std::endl;
                continue;
            }
            
            // Eliminar espais en blanc al principi i final
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            
            if (line.empty()) {
                std::cout << "     -> Ignorada (només espais)" << std::endl;
                continue;
            }
            
            // Detectar seccions
            if (line[0] == '[' && line[line.length()-1] == ']') {
                current_section = line.substr(1, line.length()-2);
                std::cout << "     -> Nova secció: [" << current_section << "]" << std::endl;
                continue;
            }
            
            // Parsejar segons la secció actual
            if (current_section == "GAME") {
                std::cout << "     -> Processant secció GAME" << std::endl;
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    key.erase(0, key.find_first_not_of(" \t"));
                    key.erase(key.find_last_not_of(" \t") + 1);
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
                    std::cout << "       Key: '" << key << "', Value: '" << value << "'" << std::endl;
                    
                    // Simular parseGameConstant
                    if (key == "max_players") {
                        try {
                            int max_players = std::stoi(value);
                            std::cout << "       max_players = " << max_players << std::endl;
                        } catch (const std::exception& e) {
                            std::cout << "       ERROR en std::stoi: " << e.what() << std::endl;
                        }
                    }
                }
            }
        }
        
        file.close();
        std::cout << "3. Parsing completat!" << std::endl;
        
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
