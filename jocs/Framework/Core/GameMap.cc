#include "GameMap.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>

bool GameMap::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No s'ha pogut obrir el fitxer " << filename << std::endl;
        return false;
    }
    
    bool result = loadFromStream(file);
    file.close();
    return result;
}

bool GameMap::loadFromStream(std::istream& is) {
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
        if (current_section == "MAP_HEADER") {
            std::istringstream iss(line);
            if (!parseMapHeader(iss)) {
                std::cerr << "Error parsejant capçalera del mapa: " << line << std::endl;
                return false;
            }
        } else if (current_section == "MAP_GRID") {
            std::istringstream iss(line);
            if (!parseMapGrid(iss)) {
                std::cerr << "Error parsejant graella del mapa: " << line << std::endl;
                return false;
            }
        } else if (current_section == "MAP_UNITS") {
            std::istringstream iss(line);
            if (!parseMapUnits(iss)) {
                std::cerr << "Error parsejant unitats del mapa: " << line << std::endl;
                return false;
            }
        } else if (current_section == "MAP_OBJECTS") {
            std::istringstream iss(line);
            if (!parseMapObjects(iss)) {
                std::cerr << "Error parsejant objectes del mapa: " << line << std::endl;
                return false;
            }
        } else if (current_section == "MAP_CONSTANTS") {
            parseMapConstant(line);
        }
    }
    
    return validateMap();
}

bool GameMap::parseMapHeader(std::istream& is) {
    std::string key, value;
    
    // Format: key=value
    if (!(is >> key)) return false;
    
    size_t pos = key.find('=');
    if (pos == std::string::npos) return false;
    
    value = key.substr(pos + 1);
    key = key.substr(0, pos);
    
    // Processar claus especials
    if (key == "map_name") map_name = value;
    else if (key == "map_description") map_description = value;
    else if (key == "rows") rows = std::stoi(value);
    else if (key == "cols") cols = std::stoi(value);
    else if (key == "nb_players") nb_players = std::stoi(value);
    else if (key == "nb_rounds") nb_rounds = std::stoi(value);
    else if (key == "seed") seed = std::stoi(value);
    else map_constants[key] = value;
    
    return true;
}

bool GameMap::parseMapGrid(std::istream& is) {
    std::string cell_type;
    int i, j;
    
    // Format: i j cell_type [owner] [unit_id] [prop1=val1 prop2=val2 ...]
    if (!(is >> i >> j >> cell_type)) return false;
    
    if (!posOk(i, j)) {
        std::cerr << "Error: Posició invàlida al mapa: " << i << ", " << j << std::endl;
        return false;
    }
    
    MapCell& cell = grid[i][j];
    cell.cell_type = cell_type;
    
    // Llegir propietari opcional
    int owner;
    if (is >> owner) cell.owner = owner;
    
    // Llegir unit_id opcional
    int unit_id;
    if (is >> unit_id) cell.unit_id = unit_id;
    
    // Llegir propietats opcionals
    std::string prop;
    while (is >> prop) {
        size_t pos = prop.find('=');
        if (pos != std::string::npos) {
            std::string key = prop.substr(0, pos);
            std::string value = prop.substr(pos + 1);
            cell.properties[key] = value;
        }
    }
    
    return true;
}

bool GameMap::parseMapUnits(std::istream& is) {
    MapUnit unit;
    
    // Format: unit_type player_id x y health [prop1=val1 prop2=val2 ...]
    if (!(is >> unit.unit_type >> unit.player_id >> unit.x >> unit.y >> unit.health)) {
        return false;
    }
    
    if (!posOk(unit.x, unit.y)) {
        std::cerr << "Error: Posició invàlida per unitat: " << unit.x << ", " << unit.y << std::endl;
        return false;
    }
    
    // Llegir propietats opcionals
    std::string prop;
    while (is >> prop) {
        size_t pos = prop.find('=');
        if (pos != std::string::npos) {
            std::string key = prop.substr(0, pos);
            std::string value = prop.substr(pos + 1);
            unit.properties[key] = value;
        }
    }
    
    units.push_back(unit);
    return true;
}

bool GameMap::parseMapObjects(std::istream& is) {
    MapObject obj;
    
    // Format: object_type x y [prop1=val1 prop2=val2 ...]
    if (!(is >> obj.object_type >> obj.x >> obj.y)) {
        return false;
    }
    
    if (!posOk(obj.x, obj.y)) {
        std::cerr << "Error: Posició invàlida per objecte: " << obj.x << ", " << obj.y << std::endl;
        return false;
    }
    
    // Llegir propietats opcionals
    std::string prop;
    while (is >> prop) {
        size_t pos = prop.find('=');
        if (pos != std::string::npos) {
            std::string key = prop.substr(0, pos);
            std::string value = prop.substr(pos + 1);
            obj.properties[key] = value;
        }
    }
    
    objects.push_back(obj);
    return true;
}

bool GameMap::parseMapConstant(const std::string& line) {
    size_t pos = line.find('=');
    if (pos == std::string::npos) return false;
    
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    
    // Eliminar espais en blanc
    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);
    
    map_constants[key] = value;
    return true;
}

bool GameMap::validateMap() const {
    if (map_name.empty()) {
        std::cerr << "Error: Nom del mapa no definit" << std::endl;
        return false;
    }
    
    if (rows < 1 || cols < 1) {
        std::cerr << "Error: Dimensions del mapa invàlides: " << rows << "x" << cols << std::endl;
        return false;
    }
    
    if (nb_players < 1) {
        std::cerr << "Error: Nombre de jugadors invàlid: " << nb_players << std::endl;
        return false;
    }
    
    if (nb_rounds < 1) {
        std::cerr << "Error: Nombre de rondes invàlid: " << nb_rounds << std::endl;
        return false;
    }
    
    if (grid.empty() || grid.size() != rows || grid[0].size() != cols) {
        std::cerr << "Error: Graella del mapa no inicialitzada correctament" << std::endl;
        return false;
    }
    
    return true;
}

std::string GameMap::getMapDescription() const {
    std::ostringstream oss;
    oss << "Mapa: " << map_name << "\n";
    oss << "Descripció: " << map_description << "\n";
    oss << "Dimensions: " << rows << "x" << cols << "\n";
    oss << "Jugadors: " << nb_players << "\n";
    oss << "Rondes: " << nb_rounds << "\n";
    oss << "Seed: " << seed << "\n";
    oss << "Unitats: " << units.size() << "\n";
    oss << "Objectes: " << objects.size() << "\n";
    return oss.str();
}

void GameMap::generateMap(int seed) {
    if (seed >= 0) this->seed = seed;
    
    if (this->seed == 0) {
        // Generar seed automàticament
        std::random_device rd;
        this->seed = rd();
    }
    
    generateEmptyMap();
    generateRandomMap(this->seed);
}

void GameMap::generateEmptyMap() {
    // Inicialitzar graella buida
    grid.clear();
    grid.resize(rows, std::vector<MapCell>(cols));
    
    // Omplir amb cel·les buides
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = MapCell("Empty");
        }
    }
    
    // Afegir murs als bordes
    for (int i = 0; i < rows; ++i) {
        grid[i][0].cell_type = "Wall";
        grid[i][cols-1].cell_type = "Wall";
    }
    for (int j = 0; j < cols; ++j) {
        grid[0][j].cell_type = "Wall";
        grid[rows-1][j].cell_type = "Wall";
    }
}

void GameMap::generateRandomMap(int seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 100);
    
    // Generar alguns murs aleatoris al centre
    for (int i = 2; i < rows-2; ++i) {
        for (int j = 2; j < cols-2; ++j) {
            if (dis(gen) < 15) { // 15% de probabilitat de mur
                grid[i][j].cell_type = "Wall";
            }
        }
    }
    
    // Generar posicions inicials per als jugadors
    std::vector<std::pair<int, int>> player_positions;
    for (int p = 0; p < nb_players; ++p) {
        int attempts = 0;
        std::pair<int, int> pos;
        
        do {
            pos.first = 1 + (gen() % (rows-2));
            pos.second = 1 + (gen() % (cols-2));
            attempts++;
        } while (grid[pos.first][pos.second].cell_type != "Empty" && attempts < 100);
        
        if (attempts < 100) {
            player_positions.push_back(pos);
            grid[pos.first][pos.second].owner = p;
        }
    }
}
