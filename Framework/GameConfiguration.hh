#ifndef GameConfiguration_hh
#define GameConfiguration_hh

#include <string>
#include <fstream>
#include <sstream>

class GameConfiguration {
private:
    std::string game_name;
    std::string game_version;
    bool loaded;
    
public:
    GameConfiguration();
    ~GameConfiguration() = default;
    
    // Carregar configuració des d'un fitxer
    bool loadFromFile(const std::string& filename);
    
    // Getters
    const std::string& getGameName() const { return game_name; }
    const std::string& getGameVersion() const { return game_version; }
    std::string getFullVersion() const { return game_name + " " + game_version; }
    bool isLoaded() const { return loaded; }
    
    // Setters
    void setGameName(const std::string& name) { game_name = name; }
    void setGameVersion(const std::string& version) { game_version = version; }
};

#endif
