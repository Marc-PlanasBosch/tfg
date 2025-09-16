#include "GameConfiguration.hh"
#include <iostream>
#include <algorithm>

GameConfiguration::GameConfiguration()
    : game_name(""), game_version(""), loaded(false) {
    // No hi ha valors per defecte - tot es carrega del fitxer de configuració
    
    // Intentar carregar configuració per defecte
    if (!loadFromFile("game_config.cnf")) {
        std::cerr << "[DEBUG] Carregant configuració per defecte" << std::endl;
        loadFromFile("default_config.cnf");
    }
}

bool GameConfiguration::loadFromFile(const std::string& filename) {
        std::cerr << "[DEBUG] Carregant configuració des de: " << filename << std::endl;
    
    std::ifstream config(filename);
    if (!config.is_open()) {
        std::cerr << "Warning: No s'ha pogut obrir el fitxer de configuració: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    std::string current_unit_type;
    
    while (std::getline(config, line)) {
        // Ignorar comentaris i línies buides
        if (line.empty() || line[0] == '#') continue;
        
        // Eliminar espais al principi
        line.erase(0, line.find_first_not_of(" \t"));
        
        if (line.find(":") != std::string::npos && line.find(" ") == std::string::npos) {
            // És una secció d'unitat (ex: "farmers:")
            current_unit_type = line.substr(0, line.find(":"));
            unit_types.push_back(current_unit_type);
            std::cerr << "[DEBUG] Secció d'unitat: " << current_unit_type << std::endl;
        } else if (line.find("  ") == 0) {
            // És un paràmetre d'unitat (ex: "  health 60")
            if (!current_unit_type.empty()) {
                std::istringstream iss(line);
                std::string param, value;
                iss >> param >> value;
                
                if (param == "health") {
                    unit_health[current_unit_type] = std::stoi(value);
                    unit_params[current_unit_type].push_back("health");
                    std::cerr << "[DEBUG]   " << current_unit_type << ".health = " << value << std::endl;
                } else if (param == "regen") {
                    unit_regen[current_unit_type] = std::stoi(value);
                    unit_params[current_unit_type].push_back("regen");
                    std::cerr << "[DEBUG]   " << current_unit_type << ".regen = " << value << std::endl;
                } else {
                    // Paràmetres especials (ex: magic_cost, curse_damage, damage_min, damage_max)
                    unit_special_params[current_unit_type + "_" + param] = std::stoi(value);
                    unit_params[current_unit_type].push_back(param);
                    std::cerr << "[DEBUG]   " << current_unit_type << "." << param << " = " << value << std::endl;
                }
            }
        } else {
            // És un paràmetre global
            std::istringstream iss(line);
            std::string key, value;
            iss >> key >> value;
            
            if (key == "game_name") {
                game_name = value;
                std::cerr << "[DEBUG] game_name = " << value << std::endl;
            } else if (key == "version") {
                game_version = value;
                std::cerr << "[DEBUG] version = " << value << std::endl;
            } else if (key == "damage_min") {
                unit_special_params["global_damage_min"] = std::stoi(value);
                std::cerr << "[DEBUG] damage_min = " << value << std::endl;
            } else if (key == "damage_max") {
                unit_special_params["global_damage_max"] = std::stoi(value);
                std::cerr << "[DEBUG] damage_max = " << value << std::endl;
            }
        }
    }
    
    config.close();
    loaded = true;
    
    std::cerr << "[DEBUG] Configuració carregada correctament:" << std::endl;
    std::cerr << "  - Joc: " << game_name << " " << game_version << std::endl;
    std::cerr << "  - Tipus d'unitats: ";
    for (const auto& type : unit_types) {
        std::cerr << type << " ";
    }
    std::cerr << std::endl;
    std::cerr << "  - Característiques per tipus:" << std::endl;
    for (const auto& type : unit_types) {
        std::cerr << "    * " << type << ": salut=" << unit_health[type] 
                  << ", regen=" << unit_regen[type] << std::endl;
    }
    
    return true;
}

bool GameConfiguration::hasUnitType(const std::string& type) const {
    return std::find(unit_types.begin(), unit_types.end(), type) != unit_types.end();
}

int GameConfiguration::getUnitHealth(const std::string& type) const {
    auto it = unit_health.find(type);
    return (it != unit_health.end()) ? it->second : 100; // Valor per defecte
}

int GameConfiguration::getUnitRegen(const std::string& type) const {
    auto it = unit_regen.find(type);
    return (it != unit_regen.end()) ? it->second : 0; // Valor per defecte
}

int GameConfiguration::getUnitSpecialParam(const std::string& type, const std::string& param) const {
    std::string key = type + "_" + param;
    auto it = unit_special_params.find(key);
    return (it != unit_special_params.end()) ? it->second : 0; // Valor per defecte
}

const std::vector<std::string>& GameConfiguration::getUnitParams(const std::string& unit_type) const {
    auto it = unit_params.find(unit_type);
    if (it != unit_params.end()) {
        return it->second;
    }
    static const std::vector<std::string> empty;
    return empty;
}

void GameConfiguration::addUnitType(const std::string& type, int health, int regen) {
    if (!hasUnitType(type)) {
        unit_types.push_back(type);
    }
    unit_health[type] = health;
    unit_regen[type] = regen;
}

void GameConfiguration::setUnitSpecialParam(const std::string& type, const std::string& param, int value) {
    std::string key = type + "_" + param;
    unit_special_params[key] = value;
}

void GameConfiguration::addUnitParamOrder(const std::string& param_name, const std::string& unit_type, const std::string& param_type) {
    unit_param_order.push_back(std::make_tuple(param_name, unit_type, param_type));
}
