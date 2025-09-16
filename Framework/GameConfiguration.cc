#include "GameConfiguration.hh"
#include <iostream>

GameConfiguration::GameConfiguration() 
    : game_name("battleroyale"), game_version("v1.1"), loaded(false) {
}

bool GameConfiguration::loadFromFile(const std::string& filename) {
    std::ifstream config(filename);
    if (!config.is_open()) {
        std::cerr << "Warning: No s'ha pogut obrir el fitxer de configuració: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(config, line)) {
        std::istringstream iss(line);
        std::string key, value;
        iss >> key >> value;
        
        if (key == "game_name") {
            game_name = value;
        } else if (key == "version") {
            game_version = value;
        }
    }
    
    config.close();
    loaded = true;
    return true;
}
