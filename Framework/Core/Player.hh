#ifndef Player_hh
#define Player_hh

#include "Action.hh"
#include "Board.hh"
#include <memory>

/**
 * Classe abstracta per representar un jugador (IA)
 * Basada en l'estructura dels jocs originals
 */
class Player {
protected:
    int player_id;
    std::shared_ptr<Board> board;
    Action* action;
    
public:
    Player() : player_id(-1), action(nullptr) {}
    virtual ~Player() = default;
    
    /**
     * Configura el jugador per a una nova ronda
     */
    virtual void reset(int id, std::shared_ptr<Board> b, Action& a) {
        player_id = id;
        board = b;
        action = &a;
    }
    
    /**
     * Mètode principal que el jugador implementa per jugar
     */
    virtual void play() = 0;
    
    /**
     * Obté l'ID del jugador
     */
    int getPlayerId() const { return player_id; }
    
    /**
     * Obté el tauler actual
     */
    std::shared_ptr<Board> getBoard() const { return board; }
    
    /**
     * Obté l'acció actual
     */
    Action* getAction() const { return action; }
};

#endif
