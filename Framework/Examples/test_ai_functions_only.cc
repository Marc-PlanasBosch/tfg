#include "../Core/AIFunctions.hh"
#include <iostream>

/**
 * Test només de l'AIFunctionManager per a aïllar el problema
 */
int main() {
    std::cout << "=== Test Només AIFunctionManager ===" << std::endl;
    
    try {
        std::cout << "1. Creant AIFunctionManager..." << std::endl;
        AIFunctions::AIFunctionManager ai_manager;
        std::cout << "   AIFunctionManager creat correctament!" << std::endl;
        
        std::cout << "2. Intentant carregar configuració d'IA..." << std::endl;
        bool result = ai_manager.loadFromFile("dominator_game_def.cnf");
        std::cout << "   Resultat: " << (result ? "OK" : "ERROR") << std::endl;
        
        if (result) {
            std::cout << "3. Mostrant resum de funcions d'IA..." << std::endl;
            std::cout << "   Funcions disponibles: " << ai_manager.listAvailableFunctions().size() << std::endl;
            std::cout << "   Accions disponibles: " << ai_manager.listAvailableActions().size() << std::endl;
            std::cout << "   Utilitats disponibles: " << ai_manager.listAvailableUtilities().size() << std::endl;
            
            // Mostrar algunes funcions com a exemple
            auto functions = ai_manager.listAvailableFunctions();
            if (!functions.empty()) {
                std::cout << "   Exemple de funció: " << functions[0] << std::endl;
            }
        }
        
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
