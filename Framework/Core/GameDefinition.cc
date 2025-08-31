#include "GameDefinition.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

bool GameDefinition::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No s'ha pogut obrir el fitxer " << filename << std::endl;
        return false;
    }
    
    bool result = loadFromStream(file);
    file.close();
    return result;
}

bool GameDefinition::loadFromStream(std::istream& is) {
    std::string line;
    std::string current_section = "";
    
    while (std::getline(is, line)) {
        // Ignorar línies buides i comentaris
        if (line.empty() || line[0] == '#') continue;
        
        // Eliminar espais en blanc al principi i final
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty()) continue;
        
        // Detectar seccions
        if (line[0] == '[' && line[line.length()-1] == ']') {
            current_section = line.substr(1, line.length()-2);
            continue;
        }
        
        // Parsejar segons la secció actual
        if (current_section == "GAME") {
            // Parsejar línia de constant: key=value
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                // Eliminar espais en blanc
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                parseGameConstant(key, value);
            }
        } else if (current_section == "UNIT_TYPES") {
            // Guardar la línia per parsejar-la després
            std::istringstream iss(line);
            if (!parseUnitType(iss)) {
                std::cerr << "Error parsejant tipus d'unitat: " << line << std::endl;
                return false;
            }
        } else if (current_section == "RULES") {
            std::istringstream iss(line);
            if (!parseGameRule(iss)) {
                std::cerr << "Error parsejant regla: " << line << std::endl;
                return false;
            }
        } else if (current_section == "MECHANICS") {
            std::istringstream iss(line);
            if (!parseGameMechanic(iss)) {
                std::cerr << "Error parsejant mecànica: " << line << std::endl;
                return false;
            }
        } else if (current_section == "CONSTANTS") {
            // Parsejar línia de constant: key=value
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                // Eliminar espais en blanc
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                parseGameConstant(key, value);
            }
        }
        // Ignorar seccions desconegudes (AI_FUNCTIONS, AI_ACTIONS, AI_UTILITIES, FUNCTION_MAPPINGS)
        // Aquestes seccions es gestionen per separat per l'AIAdapter
    }
    
    return validateConfiguration();
}

bool GameDefinition::parseUnitType(std::istream& is) {
    UnitType unit_type;
    std::string token;
    
    // Format: name symbol max_health initial_health regen_rate movement_range attack_range attack_power defense can_capture can_move can_attack [abilities...]
    if (!(is >> unit_type.name >> unit_type.symbol >> unit_type.max_health >> 
          unit_type.initial_health >> unit_type.regen_rate >> unit_type.movement_range >>
          unit_type.attack_range >> unit_type.attack_power >> unit_type.defense)) {
        return false;
    }
    
    // Llegir booleans
    int temp;
    if (is >> temp) unit_type.can_capture = (temp != 0);
    if (is >> temp) unit_type.can_move = (temp != 0);
    if (is >> temp) unit_type.can_attack = (temp != 0);
    
    // Llegir habilitats
    std::string ability;
    while (is >> ability) {
        unit_type.abilities.push_back(ability);
    }
    
    unit_types[unit_type.name] = unit_type;
    return true;
}

bool GameDefinition::parseGameRule(std::istream& is) {
    GameRule rule;
    std::string token;
    
    // Format: name type [param1=value1 param2=value2 ...]
    if (!(is >> rule.name >> rule.type)) {
        return false;
    }
    
    // Llegir paràmetres opcionals
    std::string param;
    while (is >> param) {
        size_t pos = param.find('=');
        if (pos != std::string::npos) {
            std::string key = param.substr(0, pos);
            std::string value = param.substr(pos + 1);
            rule.parameters[key] = value;
        }
    }
    
    game_rules.push_back(rule);
    return true;
}

bool GameDefinition::parseGameMechanic(std::istream& is) {
    GameMechanic mechanic;
    std::string token;
    
    // Format: name description [ability1 ability2 ...] [param1=value1 param2=value2 ...]
    if (!(is >> mechanic.name)) {
        return false;
    }
    
    // Llegir descripció (pot contenir espais)
    std::getline(is, mechanic.description);
    
    // Llegir habilitats requerides i paràmetres
    std::string token2;
    while (is >> token2) {
        if (token2.find('=') != std::string::npos) {
            // És un paràmetre
            size_t pos = token2.find('=');
            std::string key = token2.substr(0, pos);
            std::string value = token2.substr(pos + 1);
            mechanic.parameters[key] = value;
        } else {
            // És una habilitat
            mechanic.required_abilities.push_back(token2);
        }
    }
    
    game_mechanics.push_back(mechanic);
    return true;
}

bool GameDefinition::parseGameConstant(const std::string& key, const std::string& value) {
    // Processar constants especials
    if (key == "game_name") game_name = value;
    else if (key == "version") version = value;
    else if (key == "max_players") max_players = std::stoi(value);
    else if (key == "default_rounds") default_rounds = std::stoi(value);
    else if (key == "default_rows") default_rows = std::stoi(value);
    else if (key == "default_cols") default_cols = std::stoi(value);
    else game_constants[key] = value;
    
    return true;
}

bool GameDefinition::validateConfiguration() const {
    if (game_name.empty()) {
        std::cerr << "Error: Nom del joc no definit" << std::endl;
        return false;
    }
    
    if (max_players < 1) {
        std::cerr << "Error: Nombre de jugadors invàlid: " << max_players << std::endl;
        return false;
    }
    
    if (default_rounds < 1) {
        std::cerr << "Error: Nombre de rondes invàlid: " << default_rounds << std::endl;
        return false;
    }
    
    if (default_rows < 1 || default_cols < 1) {
        std::cerr << "Error: Dimensions del tauler invàlides: " << default_rows << "x" << default_cols << std::endl;
        return false;
    }
    
    if (unit_types.empty()) {
        std::cerr << "Error: No hi ha tipus d'unitats definits" << std::endl;
        return false;
    }
    
    return true;
}

std::string GameDefinition::getGameDescription() const {
    std::ostringstream oss;
    oss << "Joc: " << game_name << " v" << version << "\n";
    oss << "Jugadors: " << max_players << "\n";
    oss << "Rondes: " << default_rounds << "\n";
    oss << "Tauler: " << default_rows << "x" << default_cols << "\n";
    oss << "Tipus d'unitats: " << unit_types.size() << "\n";
    oss << "Regles: " << game_rules.size() << "\n";
    oss << "Mecàniques: " << game_mechanics.size() << "\n";
    return oss.str();
}
