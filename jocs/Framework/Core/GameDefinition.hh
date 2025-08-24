#ifndef GameDefinition_hh
#define GameDefinition_hh

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <istream>
#include <ostream>

/**
 * Classe base per definir un joc de manera genèrica.
 * Permet definir tipus d'unitats, regles, i mecàniques del joc.
 */
class GameDefinition {
public:
    /**
     * Estructura per definir un tipus d'unitat
     */
    struct UnitType {
        std::string name;           // Nom del tipus d'unitat
        std::string symbol;         // Símbol per representar-lo
        int max_health;             // Vida màxima
        int initial_health;         // Vida inicial
        int regen_rate;             // Taxa de regeneració per ronda
        int movement_range;         // Rang de moviment
        int attack_range;           // Rang d'atac
        int attack_power;           // Poder d'atac
        int defense;                // Defensa
        bool can_capture;           // Pot capturar territori?
        bool can_move;              // Pot moure's?
        bool can_attack;            // Pot atacar?
        std::vector<std::string> abilities; // Habilitats especials
        
        UnitType() : max_health(100), initial_health(100), regen_rate(0),
                    movement_range(1), attack_range(1), attack_power(0),
                    defense(0), can_capture(false), can_move(true), can_attack(false) {}
    };
    
    /**
     * Estructura per definir una regla del joc
     */
    struct GameRule {
        std::string name;           // Nom de la regla
        std::string type;           // Tipus de regla
        std::map<std::string, std::string> parameters; // Paràmetres de la regla
        
        GameRule() {}
        GameRule(const std::string& n, const std::string& t) : name(n), type(t) {}
    };
    
    /**
     * Estructura per definir una mecànica del joc
     */
    struct GameMechanic {
        std::string name;           // Nom de la mecànica
        std::string description;    // Descripció
        std::vector<std::string> required_abilities; // Habilitats requerides
        std::map<std::string, std::string> parameters; // Paràmetres
        
        GameMechanic() {}
        GameMechanic(const std::string& n, const std::string& d) : name(n), description(d) {}
    };

protected:
    std::string game_name;          // Nom del joc
    std::string version;            // Versió del joc
    int max_players;                // Nombre màxim de jugadors
    int default_rounds;             // Rondes per defecte
    int default_rows;               // Files per defecte
    int default_cols;               // Columnes per defecte
    
    std::map<std::string, UnitType> unit_types;     // Tipus d'unitats disponibles
    std::vector<GameRule> game_rules;               // Regles del joc
    std::vector<GameMechanic> game_mechanics;       // Mecàniques del joc
    
    std::map<std::string, std::string> game_constants; // Constants del joc

public:
    GameDefinition() : max_players(4), default_rounds(200), default_rows(15), default_cols(15) {}
    virtual ~GameDefinition() = default;
    
    // Mètodes per llegir la definició del joc
    virtual bool loadFromFile(const std::string& filename);
    virtual bool loadFromStream(std::istream& is);
    
    // Mètodes per accedir a la configuració
    inline const std::string& getGameName() const { return game_name; }
    inline const std::string& getVersion() const { return version; }
    inline int getMaxPlayers() const { return max_players; }
    inline int getDefaultRounds() const { return default_rounds; }
    inline int getDefaultRows() const { return default_rows; }
    inline int getDefaultCols() const { return default_cols; }
    
    // Mètodes per gestionar tipus d'unitats
    inline const std::map<std::string, UnitType>& getUnitTypes() const { return unit_types; }
    inline const UnitType* getUnitType(const std::string& name) const {
        auto it = unit_types.find(name);
        return (it != unit_types.end()) ? &it->second : nullptr;
    }
    
    // Mètodes per gestionar regles i mecàniques
    inline const std::vector<GameRule>& getGameRules() const { return game_rules; }
    inline const std::vector<GameMechanic>& getGameMechanics() const { return game_mechanics; }
    
    // Mètodes per gestionar constants
    inline const std::map<std::string, std::string>& getGameConstants() const { return game_constants; }
    inline std::string getGameConstant(const std::string& key, const std::string& default_value = "") const {
        auto it = game_constants.find(key);
        return (it != game_constants.end()) ? it->second : default_value;
    }
    
    // Mètodes virtuals per personalitzar el comportament
    virtual bool validateConfiguration() const;
    virtual std::string getGameDescription() const;
    
protected:
    // Mètodes auxiliars per parsejar la configuració
    virtual bool parseUnitType(std::istream& is);
    virtual bool parseGameRule(std::istream& is);
    virtual bool parseGameMechanic(std::istream& is);
    virtual bool parseGameConstant(const std::string& key, const std::string& value);
};

#endif
