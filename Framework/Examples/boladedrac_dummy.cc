#include "../Core/Game.hh"
#include "../Core/Player.hh"
#include "../Core/Board.hh"
#include "../Core/Action.hh"
#include <iostream>
#include <memory>
#include <random>
#include <fstream>

/**
 * Jugador dummy per BolaDeDrac2015
 * Basat en l'estructura dels dummys originals
 */
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

int main() {
    std::cout << "=== Framework Genèric - BolaDeDrac2015 ===" << std::endl;
    
    // 1. Carregar definició del joc
    auto game_def = std::make_shared<GameDefinition>();
    if (!game_def->loadFromFile("boladedrac_game_def.cnf")) {
        std::cerr << "Error carregant la definició del joc" << std::endl;
        return 1;
    }
    
    std::cout << "Joc carregat: " << game_def->getGameName() << " v" << game_def->getVersion() << std::endl;
    
    // 2. Crear el joc
    Game game(game_def);
    
    // 3. Carregar mapa original de BolaDeDrac2015
    if (!game.loadMap("../jocs/BolaDeDrac2015/maze1.cnf")) {
        std::cerr << "Error carregant el mapa" << std::endl;
        return 1;
    }
    
    std::cout << "Mapa carregat correctament" << std::endl;
    
    // 4. Afegir jugadors dummy
    for (int i = 0; i < game_def->getMaxPlayers(); ++i) {
        game.addPlayer(std::make_shared<BolaDeDracDummy>());
    }
    
    std::cout << "Jugadors afegits: " << game.getNbPlayers() << std::endl;
    
    // 5. Executar partida i guardar resultat
    std::ofstream result_file("result.res");
    if (!result_file.is_open()) {
        std::cerr << "Error obrint result.res" << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Iniciant Partida ===" << std::endl;
    game.run(result_file);
    
    result_file.close();
    
    std::cout << "\n=== Partida Completada ===" << std::endl;
    std::cout << "Resultat guardat a result.res" << std::endl;
    
    return 0;
}
