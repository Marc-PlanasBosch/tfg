#include "Board.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Board::Board(std::shared_ptr<GameDefinition> def) 
    : game_def(def), current_round(0), nb_players(0), rows(0), cols(0) {
    // Inicialitzar camps per al format Dominator
    land_ = std::vector<int>(0);
    total_score_ = std::vector<int>(0);
    cpu_status_ = std::vector<double>(0);
}

bool Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No s'ha pogut obrir el fitxer " << filename << std::endl;
        return false;
    }
    
    bool result = loadFromStream(file);
    file.close();
    return result;
}

bool Board::loadFromStream(std::istream& is) {
    return parseMapFile(is);
}

bool Board::parseMapFile(std::istream& is) {
    std::string line;
    
    // Llegir header del joc (format Dominator)
    if (!std::getline(is, line)) return false;
    
    std::istringstream iss(line);
    std::string game_name, version;
    iss >> game_name >> version;
    
    // Debug: imprimir el que hem llegit
    std::cerr << "Debug: llegint mapa - game_name='" << game_name << "', version='" << version << "'" << std::endl;
    
    // Llegir paràmetres del mapa
    while (std::getline(is, line) && !line.empty()) {
        std::cerr << "Debug: llegint paràmetre del mapa: '" << line << "'" << std::endl;
        iss.clear();
        iss.str(line);
        std::string key, value;
        iss >> key >> value;
        
        if (key == "nb_players") {
            nb_players = std::stoi(value);
            std::cerr << "Debug: nb_players = " << nb_players << std::endl;
        }
        else if (key == "rows") {
            rows = std::stoi(value);
            std::cerr << "Debug: rows = " << rows << std::endl;
        }
        else if (key == "cols") {
            cols = std::stoi(value);
            std::cerr << "Debug: cols = " << cols << std::endl;
        }
        else if (key == "FIXED") {
            // Hem arribat al grid, saltar línia buida
            std::cerr << "Debug: detectat grid FIXED" << std::endl;
            std::getline(is, line); // Saltar línia buida després de FIXED
            break;
        }
    }
    
    // Llegir grid del mapa
    std::cerr << "Debug: llegint grid " << rows << "x" << cols << std::endl;
    grid.clear();
    grid.resize(rows, std::vector<Cell>(cols));
    
    for (int i = 0; i < rows; ++i) {
        if (!std::getline(is, line)) {
            std::cerr << "Error: No s'ha pogut llegir la línia " << i << " del grid" << std::endl;
            return false;
        }
        
        std::cerr << "Debug: llegint línia " << i << " del grid: '" << line << "' (longitud: " << line.length() << ")" << std::endl;
        
        if (static_cast<int>(line.length()) != cols) {
            std::cerr << "Error: La línia " << i << " del grid té longitud incorrecta: " << line.length() << " (esperat: " << cols << ")" << std::endl;
            return false;
        }
        
        for (int j = 0; j < cols; ++j) {
            if (line[j] == 'X') {
                grid[i][j] = Cell(Wall, i, j);
            } else if (line[j] == '.') {
                grid[i][j] = Cell(Empty, i, j);
            } else {
                // Això pot ser una unitat, ho tractarem després
                grid[i][j] = Cell(Empty, i, j);
            }
        }
    }
    
    return true;
}

bool Board::parseOriginalMap(std::istream& is) {
    std::string line;
    
    // Llegir header del joc
    if (!std::getline(is, line)) return false;
    
    std::istringstream iss(line);
    std::string game_name, version;
    iss >> game_name >> version;
    
    // Debug: imprimir el que hem llegit
    std::cerr << "Debug: game_name='" << game_name << "', version='" << version << "'" << std::endl;
    
    // Llegir paràmetres del joc
    while (std::getline(is, line) && !isGameSection(line) && !line.empty()) {
        std::cerr << "Debug: llegint paràmetre: '" << line << "'" << std::endl;
        iss.clear();
        iss.str(line);
        std::string key, value;
        iss >> key >> value;
        
        if (key == "nb_players") {
            nb_players = std::stoi(value);
            std::cerr << "Debug: nb_players = " << nb_players << std::endl;
        }
        else if (key == "nb_rounds") {
            // Ignorar, utilitzem el de game_def
        }
        else if (key == "rows") {
            rows = std::stoi(value);
            std::cerr << "Debug: rows = " << rows << std::endl;
        }
        else if (key == "cols") {
            cols = std::stoi(value);
            std::cerr << "Debug: cols = " << cols << std::endl;
        }
        else if (key == "names" || key == "round") {
            // Ignorar línies que no són paràmetres del joc
            std::cerr << "Debug: ignorant línia: " << key << std::endl;
        }
        else if (key.length() == cols && key.find_first_not_of("X.") == std::string::npos) {
            // Aquesta línia sembla ser el grid! Tornar enrere
            std::cerr << "Debug: detectat grid, tornant enrere" << std::endl;
            // Tornar la línia al stream
            is.seekg(-static_cast<int>(line.length() + 1), std::ios::cur);
            break;
        }
    }
    
    // Si no hem llegit les dimensions, utilitzar les del game_def
    if (rows == 0 || cols == 0) {
        rows = game_def->getDefaultRows();
        cols = game_def->getDefaultCols();
        std::cerr << "Debug: utilitzant dimensions del game_def: " << rows << "x" << cols << std::endl;
    }
    
    // Si no hem arribat al final del fitxer, saltar línies buides
    if (is.good()) {
        while (std::getline(is, line) && line.empty()) {
            std::cerr << "Debug: saltant línia buida" << std::endl;
        }
    }
    
    // Llegir grid del mapa
    std::cerr << "Debug: llegint grid " << rows << "x" << cols << std::endl;
    grid.clear();
    grid.resize(rows, std::vector<Cell>(cols));
    
    for (int i = 0; i < rows; ++i) {
        if (!std::getline(is, line)) {
            std::cerr << "Debug: error llegint línia " << i << " del grid" << std::endl;
            return false;
        }
        std::cerr << "Debug: línia " << i << ": '" << line << "'" << std::endl;
        
        for (int j = 0; j < cols && j < static_cast<int>(line.length()); ++j) {
            char c = line[j];
            if (c == 'X') {
                grid[i][j] = Cell(Wall, i, j);
            } else if (c == 'C') {
                grid[i][j] = Cell(Empty, i, j); // Capsule - tractar com Empty per ara
            } else if (c == 'B') {
                grid[i][j] = Cell(Empty, i, j); // Ball - tractar com Empty per ara
            } else {
                grid[i][j] = Cell(Empty, i, j);
            }
        }
    }
    
    // Llegir seccions d'objectes i unitats
    while (std::getline(is, line) && !line.empty()) {
        if (game_def && game_def->isObjectSection(line)) {
            // Ignorar seccions d'objectes per ara
            continue;
        }
        
        // Llegir unitats basant-nos en la configuració
        if (game_def && isUnitLine(line)) {
            parseUnitLine(line);
        }
    }
    
    // Inicialitzar puntuacions
    scores.resize(nb_players, 0);
    
    return true;
}

bool Board::initialize() {
    if (!game_def) return false;
    
    // Utilitzar paràmetres de game_def si no s'han carregat del mapa
    if (nb_players == 0) nb_players = game_def->getMaxPlayers();
    if (rows == 0) rows = game_def->getDefaultRows();
    if (cols == 0) cols = game_def->getDefaultCols();
    
    // Inicialitzar camps per al format Dominator
    land_ = std::vector<int>(nb_players, 0);
    total_score_ = std::vector<int>(nb_players, 0);
    cpu_status_ = std::vector<double>(nb_players, 0);
    
    // Col·locar unitats inicials
    placeInitialUnits();
    
    // Calcular puntuacions inicials
    computeScores();
    
    return true;
}

void Board::placeInitialUnits() {
    // Crear unitats inicials basades en la definició del joc
    int unit_id = 0;
    
    for (int player_id = 0; player_id < nb_players; ++player_id) {
        // Crear farmers
        for (int i = 0; i < 5; ++i) {
            int x = 2 + (player_id % 2) * (rows - 4);
            int y = 2 + (player_id / 2) * (cols - 4) + i;
            
            if (x < rows && y < cols && grid[x][y].type == Empty) {
                Unit unit("Farmer", player_id, x, y, 100);
                units.push_back(unit);
                position_to_unit[{x, y}] = unit_id;
                unit_to_position[unit_id] = {x, y};
                grid[x][y].unit = unit_id;
                unit_id++;
            }
        }
        
        // Crear knights
        for (int i = 0; i < 3; ++i) {
            int x = 3 + (player_id % 2) * (rows - 6);
            int y = 3 + (player_id / 2) * (cols - 6) + i;
            
            if (x < rows && y < cols && grid[x][y].type == Empty) {
                Unit unit("Knight", player_id, x, y, 200);
                units.push_back(unit);
                position_to_unit[{x, y}] = unit_id;
                unit_to_position[unit_id] = {x, y};
                grid[x][y].unit = unit_id;
                unit_id++;
            }
        }
    }
}

std::shared_ptr<Board> Board::next(const std::vector<Action>& actions, Action& actions_done) {
    // Crear una còpia del tauler actual
    auto new_board = std::make_shared<Board>(game_def);
    new_board->grid = grid;
    new_board->units = units;
    new_board->scores = scores;
    new_board->player_names = player_names;
    new_board->current_round = current_round + 1;
    new_board->nb_players = nb_players;
    new_board->rows = rows;
    new_board->cols = cols;
    new_board->position_to_unit = position_to_unit;
    new_board->unit_to_position = unit_to_position;
    
    // Copiar camps del format Dominator
    new_board->land_ = land_;
    new_board->total_score_ = total_score_;
    new_board->cpu_status_ = cpu_status_;
    
    // Aplicar totes les accions
    for (const auto& action : actions) {
        new_board->applyAction(action);
    }
    
    // Actualitzar estat de les unitats
    new_board->updateUnitStates();
    
    // Recalcular puntuacions després dels moviments
    new_board->computeScores();
    
    return new_board;
}

void Board::printPreamble(std::ostream& os) const {
    os << game_def->getGameName() << " " << game_def->getVersion() << std::endl;
    os << "nb_players " << nb_players << std::endl;
    os << "nb_rounds " << game_def->getDefaultRounds() << std::endl;
    os << "rows " << rows << std::endl;
    os << "cols " << cols << std::endl;
    os << std::endl;
}

void Board::print(std::ostream& os) const {
    // Imprimir grid
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j].type == Wall) {
                os << 'X';
            } else if (grid[i][j].unit != -1) {
                // Imprimir número del jugador propietari
                const Unit* unit = getUnit(grid[i][j].unit);
                if (unit) {
                    os << unit->player_id;
                } else {
                    os << '.';
                }
            } else {
                os << '.';
            }
        }
        os << std::endl;
    }
    os << std::endl;
}

const Board::Cell& Board::getCell(int x, int y) const {
    static Cell empty_cell;
    if (x >= 0 && x < rows && y >= 0 && y < cols) {
        return grid[x][y];
    }
    return empty_cell;
}

const Board::Unit* Board::getUnit(int unit_id) const {
    if (unit_id >= 0 && unit_id < static_cast<int>(units.size())) {
        return &units[unit_id];
    }
    return nullptr;
}

std::vector<Board::Unit> Board::getUnitsByPlayer(int player_id) const {
    std::vector<Unit> result;
    for (const auto& unit : units) {
        if (unit.player_id == player_id) {
            result.push_back(unit);
        }
    }
    return result;
}

std::vector<Board::Unit> Board::getUnitsByType(const std::string& type) const {
    std::vector<Unit> result;
    for (const auto& unit : units) {
        if (unit.type == type) {
            result.push_back(unit);
        }
    }
    return result;
}

int Board::getScore(int player_id) const {
    if (player_id >= 0 && player_id < static_cast<int>(scores.size())) {
        return scores[player_id];
    }
    return 0;
}

void Board::addScore(int player_id, int points) {
    if (player_id >= 0 && player_id < static_cast<int>(scores.size())) {
        scores[player_id] += points;
    }
}

bool Board::applyAction(const Action& action) {
    // Aplicar moviments de l'acció
    auto movements = action.getMovements();
    while (!movements.empty()) {
        auto movement = movements.front();
        movements.pop();
        moveUnit(movement.unit_id, movement.direction);
    }
    
    return true;
}

bool Board::moveUnit(int unit_id, int direction) {
    const Unit* unit = getUnit(unit_id);
    if (!unit) return false;
    
    // Calcular nova posició basada en la direcció
    int new_x = unit->x;
    int new_y = unit->y;
    
    // Mapeig de direccions (0=N, 1=NE, 2=E, 3=SE, 4=S, 5=SW, 6=W, 7=NW)
    switch (direction) {
        case 0: new_x--; break; // N
        case 1: new_x--; new_y++; break; // NE
        case 2: new_y++; break; // E
        case 3: new_x++; new_y++; break; // SE
        case 4: new_x++; break; // S
        case 5: new_x++; new_y--; break; // SW
        case 6: new_y--; break; // W
        case 7: new_x--; new_y--; break; // NW
    }
    
    // Verificar si la nova posició és vàlida
    if (new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols) return false;
    if (grid[new_x][new_y].type == Wall) return false;
    if (grid[new_x][new_y].unit != -1) return false; // Ja hi ha una unitat
    
    // Moure la unitat
    grid[unit->x][unit->y].unit = -1;
    grid[new_x][new_y].unit = unit_id;
    
    // Actualitzar posicions
    position_to_unit.erase({unit->x, unit->y});
    position_to_unit[{new_x, new_y}] = unit_id;
    unit_to_position[unit_id] = {new_x, new_y};
    
    // Actualitzar unitat
    units[unit_id].x = new_x;
    units[unit_id].y = new_y;
    
    return true;
}

bool Board::attackUnit(int attacker_id, int target_id) {
    // Implementació d'atac
    return true;
}

bool Board::captureCell(int unit_id, int x, int y) {
    // Implementació de captura
    return true;
}

void Board::updateUnitStates() {
    // Actualitzar vida de les unitats, etc.
    for (auto& unit : units) {
        // Regeneració de vida
        if (unit.health < unit.max_health) {
            unit.health = std::min(unit.max_health, unit.health + 10);
        }
    }
}

bool Board::isGameSection(const std::string& line) const {
    if (!game_def) return false;
    
    // Verificar si és una secció d'objectes
    if (game_def->isObjectSection(line)) return true;
    
    // Verificar si és una secció d'unitats
    if (game_def->isUnitSection(line)) return true;
    
    return false;
}

bool Board::isUnitLine(const std::string& line) const {
    if (!game_def) return false;
    
    std::istringstream iss(line);
    std::string first_word;
    iss >> first_word;
    
    // Verificar si la primera paraula és un tipus d'unitat conegut
    const auto& unit_parsing = game_def->getUnitParsing();
    return unit_parsing.find(first_word) != unit_parsing.end();
}

void Board::parseUnitLine(const std::string& line) {
    if (!game_def) return;
    
    std::istringstream iss(line);
    std::string unit_type;
    iss >> unit_type;
    
    const auto& unit_parsing = game_def->getUnitParsing();
    auto it = unit_parsing.find(unit_type);
    if (it == unit_parsing.end()) return;
    
    const auto& params = it->second;
    std::vector<std::string> values;
    std::string value;
    
    // Llegir tots els valors de la línia
    while (iss >> value) {
        values.push_back(value);
    }
    
    // Crear unitat basant-nos en el format configurat
    if (values.size() >= 4) { // Mínim: x, y, player, direction
        int x = std::stoi(values[0]);
        int y = std::stoi(values[1]);
        int player = std::stoi(values[2]);
        int strength = (values.size() > 4) ? std::stoi(values[4]) : 100;
        
        // Crear unitat
        Unit unit(unit_type, player, x, y, strength);
        units.push_back(unit);
        
        // Actualitzar cel·la
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            grid[x][y].unit = units.size() - 1;
            grid[x][y].owner = player;
        }
    }
}

void Board::printUnits(std::ostream& os) const {
    // Imprimir totes les unitats en format Dominator
    for (const auto& unit : units) {
        // Format: tipus player_id x y health
        char type_char = 'f'; // Default farmer
        if (unit.type == "knight") type_char = 'k';
        else if (unit.type == "witch") type_char = 'w';
        else if (unit.type == "farmer") type_char = 'f';
        
        os << type_char << " " << unit.player_id << " " << unit.x << " " << unit.y << " " << unit.health << std::endl;
    }
}

void Board::printRoundState(std::ostream& os) const {
    os << std::endl << std::endl;
    
    // Imprimir grid
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j].type == Wall) {
                os << 'X';
            } else if (grid[i][j].owner == -1) {
                os << '.';
            } else if (grid[i][j].owner >= 0 && grid[i][j].owner < nb_players) {
                os << grid[i][j].owner;
            } else {
                os << '.';
            }
        }
        os << std::endl;
    }
    
    os << std::endl;
    
    // Només imprimir informació de la ronda si no és la ronda 0
    if (current_round > 0) {
        os << "round " << current_round << std::endl;
        
        os << "land";
        for (int i = 0; i < nb_players; ++i) {
            os << " " << land_[i];
        }
        os << std::endl;
        
        os << "total_score";
        for (int i = 0; i < nb_players; ++i) {
            os << " " << total_score_[i];
        }
        os << std::endl;
        
        os << "status";
        for (int i = 0; i < nb_players; ++i) {
            os << " " << cpu_status_[i];
        }
        os << std::endl;
    }
    
    // Imprimir unitats
    for (const auto& unit : units) {
        char type_char = 'f';
        if (unit.type == "knight") type_char = 'k';
        else if (unit.type == "witch") type_char = 'w';
        else if (unit.type == "farmer") type_char = 'f';
        
        os << type_char << " " << unit.player_id << " " << unit.x << " " << unit.y << " " << unit.health << std::endl;
    }
    os << std::endl;
}

void Board::computeScores() {
    // Inicialitzar land_ amb zeros
    land_ = std::vector<int>(nb_players, 0);
    
    // Comptar terreny controlat per cada jugador
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int owner = grid[i][j].owner;
            if (owner >= 0 && owner < nb_players) {
                ++land_[owner];
            }
        }
    }
    
    // Actualitzar total_score_ (acumulatiu només si no és la primera vegada)
    if (current_round > 0) {
        for (int pl = 0; pl < nb_players; ++pl) {
            total_score_[pl] += land_[pl];
        }
    } else {
        // Primera vegada, copiar land_ a total_score_
        total_score_ = land_;
    }
}