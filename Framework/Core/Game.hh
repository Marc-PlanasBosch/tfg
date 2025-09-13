#ifndef Game_hh
#define Game_hh

#include "Board.hh"
#include "Action.hh"
#include "Player.hh"
#include "GameDefinition.hh"
#include <vector>
#include <string>
#include <memory>
#include <istream>
#include <ostream>

/**
 * Classe principal que orquestra una partida de joc
 * Basada en l'estructura dels jocs originals però configurable
 */
class Game {
private:
    std::shared_ptr<GameDefinition> game_def;
    std::shared_ptr<Board> board;
    std::vector<std::shared_ptr<Player>> players;
    int current_round;
    
public:
    Game(std::shared_ptr<GameDefinition> def);
    virtual ~Game() = default;
    
    /**
     * Carrega un mapa i inicialitza la partida
     */
    bool loadMap(const std::string& map_file);
    
    /**
     * Afegeix un jugador a la partida
     */
    void addPlayer(std::shared_ptr<Player> player);
    
    /**
     * Executa una partida completa
     */
    void run(std::ostream& os);
    
    /**
     * Executa una ronda de la partida
     */
    void playRound(std::ostream& os);
    
    /**
     * Obté el tauler actual
     */
    std::shared_ptr<Board> getBoard() const { return board; }
    
    /**
     * Obté la definició del joc
     */
    std::shared_ptr<GameDefinition> getGameDefinition() const { return game_def; }
    
    /**
     * Obté la ronda actual
     */
    int getCurrentRound() const { return current_round; }
    
    /**
     * Obté el nombre de jugadors
     */
    int getNbPlayers() const { return players.size(); }
};

#endif
