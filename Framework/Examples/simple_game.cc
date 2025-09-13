#include "../Core/Game.hh"
#include "../Core/Player.hh"
#include "../Core/Board.hh"
#include "../Core/Action.hh"
#include <iostream>
#include <memory>

/**
 * Jugador simple que mou les unitats aleatòriament
 */
class RandomPlayer : public Player {
public:
    void play() override {
        // Obtenir les unitats del jugador
        auto units = board->getUnitsByPlayer(player_id);
        
        // Moure cada unitat en una direcció aleatòria
        for (const auto& unit : units) {
            int unit_id = &unit - &units[0]; // ID simple
            int direction = rand() % 8; // Direcció aleatòria 0-7
            
            action->command(unit_id, direction);
        }
    }
};

int main() {
    std::cout << "=== Framework Genèric de Jocs ===" << std::endl;
    
    // 1. Carregar definició del joc
    auto game_def = std::make_shared<GameDefinition>();
    if (!game_def->loadFromFile("dominator_game_def.cnf")) {
        std::cerr << "Error carregant la definició del joc" << std::endl;
        return 1;
    }
    
    std::cout << "Joc carregat: " << game_def->getGameName() << " v" << game_def->getVersion() << std::endl;
    
    // 2. Crear el joc
    Game game(game_def);
    
    // 3. Carregar mapa original
    if (!game.loadMap("../../jocs/Dominator/fixed1.cnf")) {
        std::cerr << "Error carregant el mapa" << std::endl;
        return 1;
    }
    
    std::cout << "Mapa carregat correctament" << std::endl;
    
    // 4. Afegir jugadors
    for (int i = 0; i < game_def->getMaxPlayers(); ++i) {
        game.addPlayer(std::make_shared<RandomPlayer>());
    }
    
    std::cout << "Jugadors afegits: " << game.getNbPlayers() << std::endl;
    
    // 5. Executar partida
    std::cout << "\n=== Iniciant Partida ===" << std::endl;
    game.run(std::cout);
    
    std::cout << "\n=== Partida Completada ===" << std::endl;
    
    return 0;
}
