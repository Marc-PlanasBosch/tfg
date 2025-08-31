#include "../Core/GameBoard.hh"
#include "../Core/GameDefinition.hh"
#include "../Core/GameMap.hh"
#include "../Core/AIAdapter.hh"
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

/**
 * Executor de joc complet que utilitza el framework i genera sortida per al visualitzador
 */
int main() {
    std::cout << "=== Executor de Joc amb Visualitzador ===" << std::endl;
    
    try {
        // 1. Carregar configuració del joc
        std::cout << "1. Carregant configuració del joc..." << std::endl;
        auto game_def = std::make_shared<GameDefinition>();
        if (!game_def->loadFromFile("dominator_game_def.cnf")) {
            std::cerr << "Error carregant la definició del joc" << std::endl;
            return 1;
        }
        std::cout << "   Joc: " << game_def->getGameName() << " v" << game_def->getVersion() << std::endl;
        
        // 2. Carregar mapa
        std::cout << "2. Carregant mapa..." << std::endl;
        auto game_map = std::make_shared<GameMap>();
        if (!game_map->loadFromFile("dominator_map.cnf")) {
            std::cerr << "Error carregant el mapa" << std::endl;
            return 1;
        }
        std::cout << "   Mapa: " << game_map->getMapSummary() << std::endl;
        
        // 3. Inicialitzar tauler
        std::cout << "3. Inicialitzant tauler..." << std::endl;
        auto game_board = std::make_shared<GameBoard>(game_def, game_map);
        if (!game_board->initialize()) {
            std::cerr << "Error inicialitzant el tauler" << std::endl;
            return 1;
        }
        std::cout << "   Tauler inicialitzat correctament!" << std::endl;
        
        // 4. Crear adaptadors d'IA per a cada jugador
        std::cout << "4. Creant adaptadors d'IA..." << std::endl;
        std::vector<std::shared_ptr<AIAdapter::AIAdapter>> ai_adapters;
        for (int player_id = 0; player_id < game_def->getMaxPlayers(); ++player_id) {
            auto adapter = std::make_shared<AIAdapter::AIAdapter>(game_board, player_id);
            if (!adapter->loadAIConfiguration("dominator_game_def.cnf")) {
                std::cerr << "Error carregant configuració d'IA per al jugador " << player_id << std::endl;
                return 1;
            }
            ai_adapters.push_back(adapter);
            std::cout << "   Adaptador d'IA " << player_id << " creat correctament!" << std::endl;
        }
        
        // 5. Obrir fitxer de sortida per al visualitzador
        std::cout << "5. Obrint fitxer de sortida..." << std::endl;
        std::ofstream output_file("game_output.txt");
        if (!output_file.is_open()) {
            std::cerr << "Error obrint fitxer de sortida" << std::endl;
            return 1;
        }
        
        // 6. Escriure capçalera del fitxer
        output_file << "# Sortida del joc generada pel framework genèric" << std::endl;
        output_file << "# Format compatible amb el visualitzador del Dominator" << std::endl;
        output_file << "# Ronda | Jugador | Acció | Detalls" << std::endl;
        output_file << std::endl;
        
        // 7. Executar partida
        std::cout << "6. Executant partida..." << std::endl;
        int max_rounds = game_def->getDefaultRounds();
        
        for (int round = 0; round < max_rounds; ++round) {
            std::cout << "   Ronda " << round + 1 << "/" << max_rounds << std::endl;
            
            // Per a cada jugador
            for (int player_id = 0; player_id < game_def->getMaxPlayers(); ++player_id) {
                // Simular accions de l'IA
                auto& adapter = ai_adapters[player_id];
                
                // Obtenir unitats del jugador
                auto farmers = adapter->farmers(player_id);
                auto knights = adapter->knights(player_id);
                auto witches = adapter->witches(player_id);
                
                // Simular accions amb les unitats
                if (!farmers.empty()) {
                    int farmer_id = farmers[0];
                    // Moure el primer farmer
                    adapter->command(farmer_id, "North");
                    output_file << round + 1 << " | " << player_id << " | MOVE | Farmer " << farmer_id << " cap al nord" << std::endl;
                }
                
                if (!knights.empty()) {
                    int knight_id = knights[0];
                    // Atacar amb el primer knight (simulat)
                    output_file << round + 1 << " | " << player_id << " | ATTACK | Knight " << knight_id << " preparat per atacar" << std::endl;
                }
                
                if (!witches.empty()) {
                    int witch_id = witches[0];
                    // Capturar amb la primera witch
                    adapter->capture(witch_id);
                    output_file << round + 1 << " | " << player_id << " | CAPTURE | Witch " << witch_id << " capturant territori" << std::endl;
                }
            }
            
            // Actualitzar estat del joc (simulat)
            if (round % 10 == 0) {
                output_file << round + 1 << " | SYSTEM | STATUS | Ronda completada, actualitzant estat..." << std::endl;
            }
            
            // Pausa per a no saturar la sortida
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        output_file.close();
        std::cout << "7. Partida completada! Sortida guardada a 'game_output.txt'" << std::endl;
        
        // 8. Mostrar instruccions per al visualitzador
        std::cout << std::endl;
        std::cout << "=== INSTRUCCIONS PER AL VISUALITZADOR ===" << std::endl;
        std::cout << "1. Copia 'game_output.txt' a la carpeta del visualitzador del Dominator" << std::endl;
        std::cout << "2. Obre el visualitzador HTML" << std::endl;
        std::cout << "3. Carrega el fitxer 'game_output.txt'" << std::endl;
        std::cout << "4. Visualitza la partida!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Excepció capturada: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Excepció desconeguda capturada" << std::endl;
        return 1;
    }
    
    std::cout << "=== Execució Completada ===" << std::endl;
    return 0;
}
