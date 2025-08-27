#ifndef GameMap_hh
#define GameMap_hh

#include "GameDefinition.hh"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <istream>
#include <ostream>
#include <algorithm>

/**
 * Classe base per gestionar mapes i partides de manera genèrica.
 * Permet definir la configuració específica d'una partida dins d'un joc.
 */
class GameMap {
public:
    /**
     * Estructura per definir una unitat en el mapa
     */
    struct MapUnit {
        std::string unit_type;      // Tipus d'unitat (referència a GameDefinition)
        int player_id;              // ID del jugador propietari
        int x, y;                   // Posició al mapa
        int health;                 // Vida actual
        std::map<std::string, std::string> properties; // Propietats específiques
        
        MapUnit() : player_id(-1), x(0), y(0), health(100) {}
        MapUnit(const std::string& type, int player, int pos_x, int pos_y, int h = 100)
            : unit_type(type), player_id(player), x(pos_x), y(pos_y), health(h) {}
    };
    
    /**
     * Estructura per definir un objecte al mapa
     */
    struct MapObject {
        std::string object_type;    // Tipus d'objecte
        int x, y;                   // Posició al mapa
        std::map<std::string, std::string> properties; // Propietats específiques
        
        MapObject() : x(0), y(0) {}
        MapObject(const std::string& type, int pos_x, int pos_y)
            : object_type(type), x(pos_x), y(pos_y) {}
    };
    
    /**
     * Estructura per definir una cel·la del mapa
     */
    struct MapCell {
        std::string cell_type;      // Tipus de cel·la (Empty, Wall, etc.)
        int owner;                  // Propietari de la cel·la (-1 si cap)
        int unit_id;                // ID de la unitat en aquesta cel·la (-1 si cap)
        std::map<std::string, std::string> properties; // Propietats específiques
        
        MapCell() : cell_type("Empty"), owner(-1), unit_id(-1) {}
        MapCell(const std::string& type, int own = -1, int unit = -1)
            : cell_type(type), owner(own), unit_id(unit) {}
    };

protected:
    std::string map_name;           // Nom del mapa
    std::string map_description;    // Descripció del mapa
    int rows, cols;                 // Dimensions del mapa
    int nb_players;                 // Nombre de jugadors en aquesta partida
    int nb_rounds;                  // Nombre de rondes per aquesta partida
    int seed;                       // Seed per randomització
    
    std::vector<std::vector<MapCell>> grid;     // Tauler del joc
    std::vector<MapUnit> units;                 // Unitats al mapa
    std::vector<MapObject> objects;             // Objectes al mapa
    std::vector<std::string> player_names;      // Noms dels jugadors
    
    std::map<std::string, std::string> map_constants; // Constants específiques del mapa

public:
    GameMap() : rows(15), cols(15), nb_players(4), nb_rounds(200), seed(0) {}
    virtual ~GameMap() = default;
    
    // Mètodes per llegir el mapa
    virtual bool loadFromFile(const std::string& filename);
    virtual bool loadFromStream(std::istream& is);
    
    // Mètodes per accedir a la configuració del mapa
    inline const std::string& getMapName() const { return map_name; }
    inline const std::string& getMapDescription() const { return map_description; }
    inline int getRows() const { return rows; }
    inline int getCols() const { return cols; }
    inline int getNbPlayers() const { return nb_players; }
    inline int getNbRounds() const { return nb_rounds; }
    inline int getSeed() const { return seed; }
    
    // Mètodes per gestionar el tauler
    inline const std::vector<std::vector<MapCell>>& getGrid() const { return grid; }
    inline MapCell& getCell(int i, int j) { return grid[i][j]; }
    inline const MapCell& getCell(int i, int j) const { return grid[i][j]; }
    
    // Mètodes per gestionar unitats
    inline const std::vector<MapUnit>& getUnits() const { return units; }
    inline void addUnit(const MapUnit& unit) { units.push_back(unit); }
    inline void removeUnit(int unit_id) {
        units.erase(std::remove_if(units.begin(), units.end(),
                                  [unit_id](const MapUnit& u) { return u.unit_type == std::to_string(unit_id); }),
                   units.end());
    }
    
    // Mètodes per gestionar objectes
    inline const std::vector<MapObject>& getObjects() const { return objects; }
    inline void addObject(const MapObject& obj) { objects.push_back(obj); }
    
    // Mètodes per gestionar jugadors
    inline const std::vector<std::string>& getPlayerNames() const { return player_names; }
    inline void setPlayerNames(const std::vector<std::string>& names) { player_names = names; }
    
    // Mètodes per gestionar constants del mapa
    inline const std::map<std::string, std::string>& getMapConstants() const { return map_constants; }
    inline std::string getMapConstant(const std::string& key, const std::string& default_value = "") const {
        auto it = map_constants.find(key);
        return (it != map_constants.end()) ? it->second : default_value;
    }
    
    // Mètodes virtuals per personalitzar el comportament
    virtual bool validateMap() const;
    virtual std::string getMapSummary() const;
    virtual void generateMap(int seed = -1);
    
    // Mètodes auxiliars
    inline bool posOk(int i, int j) const { return i >= 0 && i < rows && j >= 0 && j < cols; }
    inline bool posOk(const std::pair<int, int>& pos) const { return posOk(pos.first, pos.second); }
    
protected:
    // Mètodes auxiliars per parsejar la configuració
    virtual bool parseMapHeader(std::istream& is);
    virtual bool parseMapGrid(std::istream& is);
    virtual bool parseMapUnits(std::istream& is);
    virtual bool parseMapObjects(std::istream& is);
    virtual bool parseMapConstant(const std::string& line);
    
    // Mètodes per generar mapes
    virtual void generateEmptyMap();
    virtual void generateRandomMap(int seed);
};

#endif
