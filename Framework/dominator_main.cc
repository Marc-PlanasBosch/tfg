#include "Core/Game.hh"
#include "Core/Player.hh"
#include "Core/Board.hh"
#include "Core/Action.hh"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <random>
#include <fstream>

/**
 * Jugador dummy per Dominator compatible amb Windows
 */
class DominatorDummy : public Player {
private:
    std::mt19937 rng;
    
public:
    DominatorDummy() : rng(std::random_device{}()) {}
    
    void play() override {
        auto units = board->getUnitsByPlayer(player_id);
        if (units.empty()) return;
        
        std::uniform_int_distribution<int> action_dist(0, 1);
        std::uniform_int_distribution<int> dir_dist(0, 7);
        std::uniform_int_distribution<int> unit_dist(0, units.size() - 1);
        
        int action_type = action_dist(rng);
        int direction = dir_dist(rng);
        int unit_index = unit_dist(rng);
        
        // Trobar l'ID real de la unitat
        int unit_id = -1;
        for (int i = 0; i < board->getRows(); ++i) {
            for (int j = 0; j < board->getCols(); ++j) {
                const auto& cell = board->getCell(i, j);
                if (cell.unit != -1) {
                    const auto* unit = board->getUnit(cell.unit);
                    if (unit && unit->player_id == player_id) {
                        if (unit_index == 0) {
                            unit_id = cell.unit;
                            break;
                        }
                        unit_index--;
                    }
                }
            }
            if (unit_id != -1) break;
        }
        
        if (unit_id != -1) {
            if (action_type == 0) {
                action->move(unit_id, direction);
            } else {
                action->attack(unit_id, direction);
            }
        }
    }
};

/**
 * Executable generat automàticament per dominator
 * Basat en dominator_game_def.cnf
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <map_file> [player1] [player2] [player3] [player4]" << std::endl;
        return 1;
    }

    std::string map_file = argv[1];
    std::vector<std::string> player_names;
    for (int i = 2; i < argc && i < 6; ++i) {
        player_names.push_back(argv[i]);
    }
    while (player_names.size() < 4) {
        player_names.push_back("dummy");
    }

    std::cout << "=== dominator - Framework Genèric ===" << std::endl;
    std::cout << "Mapa: " << map_file << std::endl;
    std::cout << "Jugadors: ";
    for (const auto& name : player_names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    // Crear definició del joc Dominator (hardcoded per a aquest executable específic)
    auto game_def = std::make_shared<GameDefinition>();
    if (!game_def->loadFromFile("dominator_game_def.cnf")) {
        std::cerr << "Error carregant la definició del joc" << std::endl;
        return 1;
    }

    // Crear el joc
    Game game(game_def);

    // Carregar mapa
    if (!game.loadMap(map_file)) {
        std::cerr << "Error carregant el mapa: " << map_file << std::endl;
        return 1;
    }

    // Afegir jugadors dummy
    for (int i = 0; i < 4; ++i) {
        game.addPlayer(std::make_shared<DominatorDummy>());
    }

    // Executar partida i generar result.res
    std::cout << "\n=== Iniciant Partida ===" << std::endl;
    
    // Obrir fitxer de sortida
    std::ofstream result_file("result.res");
    if (!result_file.is_open()) {
        std::cerr << "Error obrint result.res" << std::endl;
        return 1;
    }
    
    // Executar partida escrivint al fitxer
    game.run(result_file);
    result_file.close();
    
    std::cout << "\n=== Partida Completada ===" << std::endl;
    std::cout << "Resultat guardat a result.res" << std::endl;
    return 0;
}
