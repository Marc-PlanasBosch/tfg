#ifndef GameBoard_hh
#define GameBoard_hh

#include "GameDefinition.hh"
#include "GameMap.hh"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <istream>
#include <ostream>

/**
 * Classe base per gestionar el tauler del joc de manera genèrica.
 * Implementa la lògica comuna per tots els jocs.
 */
class GameBoard {
public:
    /**
     * Estructura per representar una acció genèrica
     */
    struct GameAction {
        int unit_id;                // ID de la unitat que realitza l'acció
        std::string action_type;    // Tipus d'acció (move, attack, capture, etc.)
        std::map<std::string, std::string> parameters; // Paràmetres de l'acció
        
        GameAction() : unit_id(-1) {}
        GameAction(int id, const std::string& type) : unit_id(id), action_type(type) {}
    };
    
    /**
     * Estructura per representar l'estat del joc
     */
    struct GameState {
        int round;                  // Ronda actual
        std::vector<int> scores;    // Puntuacions dels jugadors
        std::vector<int> status;    // Estat dels jugadors
        std::map<std::string, std::string> properties; // Propietats addicionals
        
        GameState() : round(0) {}
    };

protected:
    std::shared_ptr<GameDefinition> game_def;  // Definició del joc
    std::shared_ptr<GameMap> game_map;         // Mapa actual
    GameState current_state;                   // Estat actual del joc
    
    std::vector<std::vector<std::string>> board_grid;  // Tauler actual
    std::map<int, std::string> unit_positions;         // Posicions de les unitats
    std::map<int, int> unit_health;                    // Vida de les unitats
    std::map<int, int> unit_owners;                    // Propietaris de les unitats

public:
    GameBoard(std::shared_ptr<GameDefinition> def, std::shared_ptr<GameMap> map);
    virtual ~GameBoard() = default;
    
    // Mètodes per inicialitzar el tauler
    virtual bool initialize();
    virtual void reset();
    
    // Mètodes per gestionar l'estat del joc
    virtual bool next(const std::vector<GameAction>& actions, std::ostream& os);
    virtual void updateState();
    virtual bool isGameOver() const;
    
    // Mètodes per accedir a la informació del tauler
    inline const std::vector<std::vector<std::string>>& getBoardGrid() const { return board_grid; }
    inline const GameState& getCurrentState() const { return current_state; }
    inline int getRound() const { return current_state.round; }
    inline int getNbPlayers() const { return game_map ? game_map->getNbPlayers() : 0; }
    inline int getNbRounds() const { return game_map ? game_map->getNbRounds() : 0; }
    
    // Mètodes per gestionar unitats
    virtual bool addUnit(int unit_id, const std::string& unit_type, int player, int x, int y, int health = -1);
    virtual bool removeUnit(int unit_id);
    virtual bool moveUnit(int unit_id, int new_x, int new_y);
    virtual bool attackUnit(int attacker_id, int target_id);
    
    // Mètodes per gestionar el tauler
    virtual bool setCell(int x, int y, const std::string& cell_type, int owner = -1);
    virtual std::string getCell(int x, int y) const;
    virtual bool isValidPosition(int x, int y) const;
    
    // Mètodes per gestionar puntuacions
    virtual void addScore(int player, int points);
    virtual int getScore(int player) const;
    virtual std::vector<int> getScores() const;
    
    // Mètodes per accedir a la informació de les unitats (per a l'adaptador d'IA)
    virtual std::map<int, std::string> getUnitPositions() const { return unit_positions; }
    virtual std::map<int, int> getUnitOwners() const { return unit_owners; }
    virtual std::map<int, int> getUnitHealth() const { return unit_health; }
    virtual int getUnitOwner(int unit_id) const;
    virtual int getUnitHealth(int unit_id) const;
    virtual std::string getUnitPosition(int unit_id) const;
    
    // Mètodes per imprimir informació
    virtual void printState(std::ostream& os) const;
    virtual void printPreamble(std::ostream& os) const;
    virtual void printNames(std::ostream& os) const;
    virtual void printResults(std::ostream& os) const;
    
    // Mètodes virtuals per personalitzar el comportament
    virtual bool validateAction(const GameAction& action) const;
    virtual bool executeAction(const GameAction& action);
    virtual void applyGameRules();
    virtual void updateUnitStates();
    
    // Mètodes auxiliars
    virtual std::string version() const;
    virtual std::string name(int player) const;
    virtual int score(int player) const;
    virtual int status(int player) const;

protected:
    // Mètodes auxiliars per la lògica del joc
    virtual void initializeBoard();
    virtual void placeInitialUnits();
    virtual void updateBoardGrid();
    virtual bool checkCollision(int x, int y) const;
    virtual void processUnitActions(const std::vector<GameAction>& actions);
    
    // Mètodes per gestionar mecàniques específiques
    virtual void processCaptures();
    virtual void processRegeneration();
    virtual void processSpecialEffects();
};

#endif
