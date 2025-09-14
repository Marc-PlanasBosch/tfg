#ifndef Board_hh
#define Board_hh

#include "GameDefinition.hh"
#include "Action.hh"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <istream>
#include <ostream>

/**
 * Classe que representa l'estat del tauler del joc
 * Basada en l'estructura dels jocs originals però configurable
 */
class Board {
public:
    /**
     * Tipus de cel·la
     */
    enum CellType {
        Empty, Wall, CellTypeSize
    };
    
    /**
     * Representa una cel·la del tauler
     */
    struct Cell {
        CellType type;
        int owner;      // Propietari de la cel·la (-1 si cap)
        int unit;       // ID de la unitat en aquesta cel·la (-1 si cap)
        int x, y;       // Posició
        
        Cell() : type(Empty), owner(-1), unit(-1), x(0), y(0) {}
        Cell(CellType t, int x_pos, int y_pos) : type(t), owner(-1), unit(-1), x(x_pos), y(y_pos) {}
    };
    
    /**
     * Representa una unitat al tauler
     */
    struct Unit {
        std::string type;       // Tipus d'unitat (Farmer, Knight, etc.)
        int player_id;          // ID del jugador propietari
        int x, y;               // Posició
        int health;             // Vida actual
        int max_health;         // Vida màxima
        
        Unit() : player_id(-1), x(0), y(0), health(100), max_health(100) {}
        Unit(const std::string& t, int player, int x_pos, int y_pos, int h = 100) 
            : type(t), player_id(player), x(x_pos), y(y_pos), health(h), max_health(h) {}
    };

private:
    std::shared_ptr<GameDefinition> game_def;
    std::vector<std::vector<Cell>> grid;
    std::vector<Unit> units;
    std::vector<int> scores;
    std::vector<std::string> player_names;
    int current_round;
    int nb_players;
    int rows, cols;
    
    // Camps per al format Dominator
    std::vector<int> land_;          // Terreny controlat per cada jugador
    std::vector<int> total_score_;   // Puntuació total de cada jugador
    std::vector<double> cpu_status_; // Estat de CPU de cada jugador
    
    // Mapeig de posicions a unitats
    std::map<std::pair<int, int>, int> position_to_unit;
    std::map<int, std::pair<int, int>> unit_to_position;

public:
    Board(std::shared_ptr<GameDefinition> def);
    virtual ~Board() = default;
    
    /**
     * Carrega un mapa des d'un fitxer
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * Inicialitza el tauler
     */
    bool initialize();
    
    /**
     * Aplica les accions i retorna el nou estat del tauler
     */
    std::shared_ptr<Board> next(const std::vector<Action>& actions, Action& actions_done);
    
    /**
     * Imprimeix el preàmbul del joc
     */
    void printPreamble(std::ostream& os) const;
    
    /**
     * Imprimeix l'estat actual del tauler
     */
    void print(std::ostream& os) const;
    
    // Getters
    inline int getRows() const { return rows; }
    inline int getCols() const { return cols; }
    inline int getNbPlayers() const { return nb_players; }
    inline int getCurrentRound() const { return current_round; }
    inline const std::vector<Unit>& getUnits() const { return units; }
    inline const std::vector<int>& getScores() const { return scores; }
    
    /**
     * Obté una cel·la específica
     */
    const Cell& getCell(int x, int y) const;
    
    /**
     * Obté una unitat per ID
     */
    const Unit* getUnit(int unit_id) const;
    
    /**
     * Obté les unitats d'un jugador
     */
    std::vector<Unit> getUnitsByPlayer(int player_id) const;
    
    /**
     * Obté les unitats d'un tipus específic
     */
    std::vector<Unit> getUnitsByType(const std::string& type) const;
    
    /**
     * Obté la puntuació d'un jugador
     */
    int getScore(int player_id) const;
    
    /**
     * Afegeix puntuació a un jugador
     */
    void addScore(int player_id, int points);
    
    /**
     * Imprimeix la llista d'unitats (format Dominator)
     */
    void printUnits(std::ostream& os) const;
    
    /**
     * Imprimeix l'estat de la ronda (format Dominator)
     */
    void printRoundState(std::ostream& os) const;
    
    /**
     * Calcula les puntuacions de terreny
     */
    void computeScores();

protected:
    /**
     * Carrega el mapa des d'un stream
     */
    bool loadFromStream(std::istream& is);
    
    /**
     * Parseja un mapa en format original dels jocs
     */
    bool parseOriginalMap(std::istream& is);
    
    /**
     * Parseja un fitxer de mapa (format Dominator)
     */
    bool parseMapFile(std::istream& is);
    
    /**
     * Col·loca les unitats inicials
     */
    void placeInitialUnits();
    
    /**
     * Actualitza l'estat de les unitats
     */
    void updateUnitStates();
    
    /**
     * Aplica una acció al tauler
     */
    bool applyAction(const Action& action);
    
    /**
     * Mou una unitat
     */
    bool moveUnit(int unit_id, int direction);
    
    /**
     * Ataca una unitat
     */
    bool attackUnit(int attacker_id, int target_id);
    
    /**
     * Captura una cel·la
     */
    bool captureCell(int unit_id, int x, int y);
    
    /**
     * Verifica si una línia és una secció del joc
     */
    bool isGameSection(const std::string& line) const;
    
    /**
     * Verifica si una línia conté una unitat
     */
    bool isUnitLine(const std::string& line) const;
    
    /**
     * Parseja una línia d'unitat
     */
    void parseUnitLine(const std::string& line);
};

#endif
