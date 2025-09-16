#ifndef GameConfiguration_hh
#define GameConfiguration_hh

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

class GameConfiguration {
private:
    std::string game_name;
    std::string game_version;
    std::vector<std::string> unit_types;
    std::map<std::string, int> unit_health;
    std::map<std::string, int> unit_regen;
    std::map<std::string, int> unit_special_params;
    std::map<std::string, std::vector<std::string>> unit_params; // Paràmetres que té cada unitat
    std::vector<std::tuple<std::string, std::string, std::string>> unit_param_order; // param_name, unit_type, param_type
    bool loaded;
    
public:
    GameConfiguration();
    ~GameConfiguration() = default;
    
    // Carregar configuració des d'un fitxer
    bool loadFromFile(const std::string& filename);
    
    // Getters bàsics
    const std::string& getGameName() const { return game_name; }
    const std::string& getGameVersion() const { return game_version; }
    std::string getFullVersion() const { return game_name + " " + game_version; }
    bool isLoaded() const { return loaded; }
    
    // Getters per unitats dinàmiques
    const std::vector<std::string>& getUnitTypes() const { return unit_types; }
    bool hasUnitType(const std::string& type) const;
    int getUnitHealth(const std::string& type) const;
    int getUnitRegen(const std::string& type) const;
    int getUnitSpecialParam(const std::string& type, const std::string& param) const;
    
    // Getters per ordre de paràmetres
    const std::vector<std::string>& getUnitParams(const std::string& unit_type) const;
    const std::vector<std::tuple<std::string, std::string, std::string>>& getUnitParamOrder() const { return unit_param_order; }
    
    // Setters
    void setGameName(const std::string& name) { game_name = name; }
    void setGameVersion(const std::string& version) { game_version = version; }
    void addUnitType(const std::string& type, int health, int regen);
    void setUnitSpecialParam(const std::string& type, const std::string& param, int value);
    void addUnitParamOrder(const std::string& param_name, const std::string& unit_type, const std::string& param_type);
};

#endif
