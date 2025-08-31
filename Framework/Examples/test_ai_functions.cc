#include "../Core/AIFunctions.hh"
#include <iostream>

/**
 * Programa de prova per al sistema de funcions d'IA
 * Carrega la configuració i mostra les funcions disponibles
 */
int main() {
    std::cout << "=== Test del Sistema de Funcions d'IA ===" << std::endl;
    
    // Crear el gestor de funcions d'IA
    AIFunctions::AIFunctionManager ai_manager;
    
    // Carregar la configuració del joc Dominator
    std::cout << "\nCarregant configuració del joc Dominator..." << std::endl;
    if (!ai_manager.loadFromFile("dominator_game_def.cnf")) {
        std::cerr << "Error carregant la configuració d'IA" << std::endl;
        return 1;
    }
    
    std::cout << "Configuració carregada correctament!" << std::endl;
    
    // Mostrar funcions disponibles
    std::cout << "\n=== Funcions d'IA Disponibles ===" << std::endl;
    auto functions = ai_manager.listAvailableFunctions();
    for (const auto& func_name : functions) {
        const auto* func = ai_manager.getFunction(func_name);
        if (func) {
            std::cout << "  " << func_name << " -> " << func->return_type;
            if (!func->parameters.empty()) {
                std::cout << " (";
                for (size_t i = 0; i < func->parameters.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << func->parameters[i].name << ":" << func->parameters[i].type;
                }
                std::cout << ")";
            }
            if (!func->description.empty()) {
                std::cout << " - " << func->description;
            }
            std::cout << std::endl;
        }
    }
    
    // Mostrar accions disponibles
    std::cout << "\n=== Accions d'IA Disponibles ===" << std::endl;
    auto actions = ai_manager.listAvailableActions();
    for (const auto& action_name : actions) {
        const auto* action = ai_manager.getAction(action_name);
        if (action) {
            std::cout << "  " << action_name;
            if (!action->parameters.empty()) {
                std::cout << " (";
                for (size_t i = 0; i < action->parameters.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << action->parameters[i].name << ":" << action->parameters[i].type;
                }
                std::cout << ")";
            }
            if (!action->description.empty()) {
                std::cout << " - " << action->description;
            }
            std::cout << std::endl;
        }
    }
    
    // Mostrar utilitats disponibles
    std::cout << "\n=== Utilitats d'IA Disponibles ===" << std::endl;
    auto utilities = ai_manager.listAvailableUtilities();
    for (const auto& utility_name : utilities) {
        const auto* utility = ai_manager.getUtility(utility_name);
        if (utility) {
            std::cout << "  " << utility_name << " -> " << utility->return_type;
            if (!utility->parameters.empty()) {
                std::cout << " (";
                for (size_t i = 0; i < utility->parameters.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << utility->parameters[i].name << ":" << utility->parameters[i].type;
                }
                std::cout << ")";
            }
            if (!utility->description.empty()) {
                std::cout << " - " << utility->description;
            }
            std::cout << std::endl;
        }
    }
    
    // Mostrar mapeigs de funcions
    std::cout << "\n=== Mapeigs de Funcions ===" << std::endl;
    for (const auto& func_name : functions) {
        std::string mapping = ai_manager.getFrameworkMapping(func_name);
        if (!mapping.empty()) {
            std::cout << "  " << func_name << " -> " << mapping << std::endl;
        }
    }
    
    std::cout << "\n=== Test Completat ===" << std::endl;
    return 0;
}
