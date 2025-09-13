#ifndef Action_hh
#define Action_hh

#include <string>
#include <vector>
#include <set>
#include <queue>
#include <istream>
#include <ostream>

/**
 * Classe simple per gestionar accions de moviment
 * Basada en l'estructura dels jocs originals
 */
class Action {
public:
    /**
     * Tipus d'acció
     */
    enum ActionType {
        Undefined, Moving, Throwing
    };
    
    /**
     * Estructura per representar un moviment
     */
    struct Movement {
        int unit_id;        // ID de la unitat
        int direction;      // Direcció del moviment
        ActionType type;    // Tipus d'acció
        
        Movement(int id, int dir, ActionType t = Moving) : unit_id(id), direction(dir), type(t) {}
    };

private:
    std::set<int> units_acted_;     // Unitats que ja han actuat aquesta ronda
    std::queue<Movement> movements_; // Llista de moviments a realitzar

public:
    /**
     * Constructor buit
     */
    Action();
    
    /**
     * Constructor que llegeix accions des d'un stream
     */
    Action(std::istream& is);
    
    /**
     * Destructor
     */
    virtual ~Action() = default;
    
    /**
     * Afegeix un moviment a l'acció
     */
    bool command(int unit_id, int direction);
    
    /**
     * Afegeix un moviment a l'acció
     */
    bool command(const Movement& movement);
    
    /**
     * Mou el Goku en una direcció (BolaDeDrac2015)
     */
    void move(int direction);
    
    /**
     * Llança Kamehame en una direcció (BolaDeDrac2015)
     */
    void throw_kamehame(int direction);
    
    /**
     * Obté els moviments de l'acció
     */
    const std::queue<Movement>& getMovements() const { return movements_; }
    
    /**
     * Verifica si una unitat ja ha actuat aquesta ronda
     */
    bool hasUnitActed(int unit_id) const;
    
    /**
     * Neteja les accions per la següent ronda
     */
    void clear();
    
    /**
     * Imprimeix les accions a un stream
     */
    void print(std::ostream& os) const;
    
    /**
     * Llegeix accions des d'un stream
     */
    void read(std::istream& is);
    
    /**
     * Converteix direcció de char a int
     */
    static int charToDirection(char c);
    
    /**
     * Converteix direcció de int a char
     */
    static char directionToChar(int dir);
};

#endif
