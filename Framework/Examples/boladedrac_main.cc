#include "../Core/Game.hh"
#include "../Core/Player.hh"
#include "../Core/Board.hh"
#include "../Core/Action.hh"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

/**
 * Main executable compatible amb BolaDeDrac2015
 * Generat pel Framework Genèric basat en boladedrac_game_def.cnf
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <map_file> [player1] [player2] [player3] [player4]" << std::endl;
        return 1;
    }
    
    std::string map_file = argv[1];
    
    // Noms dels jugadors (per defecte dummy si no s'especifiquen)
    std::vector<std::string> player_names;
    for (int i = 2; i < argc && i < 6; ++i) {
        player_names.push_back(argv[i]);
    }
    
    // Afegir dummys si no n'hi ha prou
    while (player_names.size() < 4) {
        player_names.push_back("dummy");
    }
    
    std::cout << "=== Framework Genèric - BolaDeDrac2015 ===" << std::endl;
    std::cout << "Mapa: " << map_file << std::endl;
    std::cout << "Jugadors: ";
    for (const auto& name : player_names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    // 1. Carregar definició del joc
    auto game_def = std::make_shared<GameDefinition>();
    if (!game_def->loadFromFile("boladedrac_game_def.cnf")) {
        std::cerr << "Error carregant la definició del joc" << std::endl;
        return 1;
    }
    
    // 2. Crear el joc
    Game game(game_def);
    
    // 3. Carregar mapa
    if (!game.loadMap(map_file)) {
        std::cerr << "Error carregant el mapa: " << map_file << std::endl;
        return 1;
    }
    
    // 4. Afegir jugadors dummy (per ara utilitzem els nostres)
    // En un futur, això hauria de carregar els dummys originals
    for (int i = 0; i < 4; ++i) {
        // TODO: Carregar dummy original basat en player_names[i]
        // Per ara utilitzem un dummy simple
        game.addPlayer(std::make_shared<BolaDeDracDummy>());
    }
    
    // 5. Executar partida
    std::cout << "\n=== Iniciant Partida ===" << std::endl;
    game.run(std::cout);
    
    std::cout << "\n=== Partida Completada ===" << std::endl;
    
    return 0;
}

// Incloure la implementació del dummy per ara
class BolaDeDracDummy : public Player {
private:
    std::mt19937 rng;
    
public:
    BolaDeDracDummy() : rng(std::random_device{}()) {}
    
    void play() override {
        // Obtenir el Goku del jugador (unit_id = 0 per simplicitat)
        auto units = board->getUnitsByPlayer(player_id);
        if (units.empty()) return;
        
        // Generar acció aleatòria
        std::uniform_int_distribution<int> action_dist(0, 1);
        std::uniform_int_distribution<int> dir_dist(0, 7);
        
        int action_type = action_dist(rng);
        int direction = dir_dist(rng);
        
        if (action_type == 0) {
            // Moviment
            action->move(direction);
        } else {
            // Llançament de Kamehame
            action->throw_kamehame(direction);
        }
    }
};
