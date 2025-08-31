#include "../Core/AIAdapter.hh"
#include "../Core/GameBoard.hh"
#include "../Core/GameDefinition.hh"
#include "../Core/GameMap.hh"
#include <iostream>
#include <memory>

/**
 * Programa de prova per a l'adaptador d'IA
 * Demostra com usar l'adaptador per a executar un joc amb IAs
 */
int main() {
    std::cout << "=== Test de l'Adaptador d'IA ===" << std::endl;
    
    try {
        // Carregar definició del joc
        std::cout << "\nCarregant definició del joc..." << std::endl;
        auto game_def = std::make_shared<GameDefinition>();
        if (!game_def->loadFromFile("dominator_game_def.cnf")) {
            std::cerr << "Error carregant la definició del joc" << std::endl;
            return 1;
        }
        std::cout << "Definició del joc carregada correctament!" << std::endl;
        
        // Carregar mapa del joc
        std::cout << "\nCarregant mapa del joc..." << std::endl;
        auto game_map = std::make_shared<GameMap>();
        if (!game_map->loadFromFile("dominator_map.cnf")) {
            std::cerr << "Error carregant el mapa del joc" << std::endl;
            return 1;
        }
        std::cout << "Mapa del joc carregat correctament!" << std::endl;
        
        // Crear el tauler del joc
        std::cout << "\nInicialitzant tauler del joc..." << std::endl;
        auto game_board = std::make_shared<GameBoard>(game_def, game_map);
        if (!game_board->initialize()) {
            std::cerr << "Error inicialitzant el tauler del joc" << std::endl;
            return 1;
        }
        std::cout << "Tauler del joc inicialitzat correctament!" << std::endl;
        
        // Crear adaptador d'IA per al jugador 0
        std::cout << "\nCreant adaptador d'IA per al jugador 0..." << std::endl;
        AIAdapter::AIAdapter ai_adapter(game_board, 0);
        
        // Carregar configuració d'IA
        if (!ai_adapter.loadAIConfiguration("dominator_game_def.cnf")) {
            std::cerr << "Error carregant la configuració d'IA" << std::endl;
            return 1;
        }
        std::cout << "Configuració d'IA carregada correctament!" << std::endl;
        
        // Provar funcions de consulta
        std::cout << "\n=== Provant Funcions de Consulta ===" << std::endl;
        
        std::cout << "Ronda actual: " << ai_adapter.round() << std::endl;
        std::cout << "Nombre de jugadors: " << ai_adapter.nb_players() << std::endl;
        std::cout << "Nombre d'unitats: " << ai_adapter.nb_units() << std::endl;
        
        // Provar funcions específiques del jugador
        std::cout << "\n--- Informació del Jugador 0 ---" << std::endl;
        std::cout << "Puntuació: " << ai_adapter.total_score(0) << std::endl;
        std::cout << "Estat: " << ai_adapter.status(0) << std::endl;
        std::cout << "Terra: " << ai_adapter.land(0) << std::endl;
        
        // Provar funcions d'unitats
        std::cout << "\n--- Unitats del Jugador 0 ---" << std::endl;
        auto farmers = ai_adapter.farmers(0);
        auto knights = ai_adapter.knights(0);
        auto witches = ai_adapter.witches(0);
        
        std::cout << "Farmers: " << ai_adapter.nb_farmers(0) << " (IDs: ";
        for (size_t i = 0; i < farmers.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << farmers[i];
        }
        std::cout << ")" << std::endl;
        
        std::cout << "Knights: " << ai_adapter.nb_knights(0) << " (IDs: ";
        for (size_t i = 0; i < knights.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << knights[i];
        }
        std::cout << ")" << std::endl;
        
        std::cout << "Witches: " << ai_adapter.nb_witches(0) << " (IDs: ";
        for (size_t i = 0; i < witches.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << witches[i];
        }
        std::cout << ")" << std::endl;
        
        // Provar funcions d'utilitat
        std::cout << "\n=== Provant Funcions d'Utilitat ===" << std::endl;
        std::cout << "Número aleatori (1-10): " << ai_adapter.random(1, 10) << std::endl;
        std::cout << "Permutació aleatòria (5): ";
        auto perm = ai_adapter.random_permutation(5);
        for (size_t i = 0; i < perm.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << perm[i];
        }
        std::cout << std::endl;
        
        // Provar validació de posicions
        std::cout << "\n=== Provant Validació de Posicions ===" << std::endl;
        std::cout << "Posició (0,0) vàlida: " << (ai_adapter.pos_ok(0, 0) ? "Sí" : "No") << std::endl;
        std::cout << "Posició (100,100) vàlida: " << (ai_adapter.pos_ok(100, 100) ? "Sí" : "No") << std::endl;
        
        // Provar accions (només si hi ha unitats)
        if (!farmers.empty()) {
            std::cout << "\n=== Provant Accions ===" << std::endl;
            int unit_id = farmers[0];
            std::cout << "Informació de la unitat " << unit_id << ": " << ai_adapter.unit(unit_id) << std::endl;
            
            // Provar moviment (només si és segur)
            std::cout << "Provant moviment de la unitat " << unit_id << " cap al nord..." << std::endl;
            ai_adapter.command(unit_id, "N");
            std::cout << "Moviment executat!" << std::endl;
        }
        
        std::cout << "\n=== Test Completat amb Èxit ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error durant l'execució: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
