#include "AIAdapter.hh"
#include <random>
#include <algorithm>
#include <sstream>

namespace AIAdapter {

AIAdapter::AIAdapter(std::shared_ptr<GameBoard> board, int player_id)
    : game_board(board), current_player_id(player_id) {
}

bool AIAdapter::loadAIConfiguration(const std::string& config_file) {
    return ai_manager.loadFromFile(config_file);
}

// Implementació de funcions de consulta
std::vector<int> AIAdapter::farmers(int player_id) {
    return getUnitsByTypeAndPlayer("Farmer", player_id);
}

std::vector<int> AIAdapter::knights(int player_id) {
    return getUnitsByTypeAndPlayer("Knight", player_id);
}

std::vector<int> AIAdapter::witches(int player_id) {
    return getUnitsByTypeAndPlayer("Witch", player_id);
}

std::string AIAdapter::unit(int unit_id) {
    return getUnitInfo(unit_id);
}

std::string AIAdapter::cell(int x, int y) {
    return getCellContent(x, y);
}

double AIAdapter::status(int player_id) {
    return getPlayerStatus(player_id);
}

int AIAdapter::total_score(int player_id) {
    return getPlayerScore(player_id);
}

int AIAdapter::land(int player_id) {
    return getPlayerLand(player_id);
}

int AIAdapter::round() {
    return getCurrentRound();
}

int AIAdapter::nb_players() {
    return getNbPlayers();
}

int AIAdapter::nb_units() {
    return getNbUnits();
}

int AIAdapter::nb_farmers(int player_id) {
    return getNbUnitsByTypeAndPlayer("Farmer", player_id);
}

int AIAdapter::nb_knights(int player_id) {
    return getNbUnitsByTypeAndPlayer("Knight", player_id);
}

int AIAdapter::nb_witches(int player_id) {
    return getNbUnitsByTypeAndPlayer("Witch", player_id);
}

// Implementació d'accions
void AIAdapter::command(int unit_id, const std::string& direction) {
    executeMove(unit_id, direction);
}

void AIAdapter::attack(int attacker_id, int target_id) {
    executeAttack(attacker_id, target_id);
}

void AIAdapter::capture(int unit_id) {
    executeCapture(unit_id);
}

// Implementació d'utilitats
int AIAdapter::random(int min, int max) {
    return generateRandom(min, max);
}

std::vector<int> AIAdapter::random_permutation(int n) {
    return generateRandomPermutation(n);
}

bool AIAdapter::pos_ok(int x, int y) {
    return isValidPosition(x, y);
}

// Funcions auxiliars de mapeig
std::vector<int> AIAdapter::getUnitsByTypeAndPlayer(const std::string& unit_type, int player_id) {
    std::vector<int> unit_ids;
    
    // Obtenir totes les unitats del jugador des del mapa de posicions
    auto unit_positions = game_board->getUnitPositions();
    auto unit_owners = game_board->getUnitOwners();
    
    for (const auto& pair : unit_positions) {
        int unit_id = pair.first;
        if (unit_owners.find(unit_id) != unit_owners.end() &&
            unit_owners.at(unit_id) == player_id) {
            // Aquí hauríem de verificar el tipus d'unitat, però no tenim aquesta informació
            // Per ara, retornem totes les unitats del jugador
            unit_ids.push_back(unit_id);
        }
    }
    
    return unit_ids;
}

int AIAdapter::getNbUnitsByTypeAndPlayer(const std::string& unit_type, int player_id) {
    return getUnitsByTypeAndPlayer(unit_type, player_id).size();
}

std::string AIAdapter::getUnitInfo(int unit_id) {
    std::ostringstream oss;
    
    std::string pos = game_board->getUnitPosition(unit_id);
    if (!pos.empty()) {
        int owner = game_board->getUnitOwner(unit_id);
        int health = game_board->getUnitHealth(unit_id);
        
        oss << "Unit{id=" << unit_id 
            << ", pos=" << pos 
            << ", player=" << owner 
            << ", health=" << health << "}";
    } else {
        oss << "Unit{id=-1, not_found}";
    }
    
    return oss.str();
}

std::string AIAdapter::getCellContent(int x, int y) {
    std::string cell_content = game_board->getCell(x, y);
    std::ostringstream oss;
    oss << "Cell{content=" << cell_content << "}";
    return oss.str();
}

double AIAdapter::getPlayerStatus(int player_id) {
    // Convertir l'estat del jugador a un valor entre 0.0 i 1.0
    auto scores = game_board->getScores();
    if (player_id >= 0 && player_id < static_cast<int>(scores.size())) {
        int score = scores[player_id];
        return score / 100.0; // Assumim que la puntuació màxima és 100
    }
    return 0.0;
}

int AIAdapter::getPlayerScore(int player_id) {
    return game_board->getScore(player_id);
}

int AIAdapter::getPlayerLand(int player_id) {
    // Comptar cel·les que pertanyen al jugador
    int land_count = 0;
    auto grid = game_board->getBoardGrid();
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            // Aquí hauríem de verificar si la cel·la pertany al jugador
            // Per ara, retornem un valor estimat
            if (cell.find("player_" + std::to_string(player_id)) != std::string::npos) {
                land_count++;
            }
        }
    }
    return land_count;
}

int AIAdapter::getCurrentRound() {
    return game_board->getRound();
}

int AIAdapter::getNbPlayers() {
    return game_board->getNbPlayers();
}

int AIAdapter::getNbUnits() {
    auto unit_positions = game_board->getUnitPositions();
    return unit_positions.size();
}

void AIAdapter::executeMove(int unit_id, const std::string& direction) {
    // Convertir la direcció a coordenades
    int dx = 0, dy = 0;
    if (direction == "N") { dx = -1; dy = 0; }
    else if (direction == "S") { dx = 1; dy = 0; }
    else if (direction == "E") { dx = 0; dy = 1; }
    else if (direction == "W") { dx = 0; dy = -1; }
    else if (direction == "NE") { dx = -1; dy = 1; }
    else if (direction == "NW") { dx = -1; dy = -1; }
    else if (direction == "SE") { dx = 1; dy = 1; }
    else if (direction == "SW") { dx = 1; dy = -1; }
    
    // Obtenir la posició actual de la unitat
    std::string pos = game_board->getUnitPosition(unit_id);
    if (!pos.empty()) {
        // Parsejar la posició (assumim format "x,y")
        size_t comma_pos = pos.find(',');
        if (comma_pos != std::string::npos) {
            int x = std::stoi(pos.substr(0, comma_pos));
            int y = std::stoi(pos.substr(comma_pos + 1));
            
            // Crear acció de moviment
            GameBoard::GameAction action;
            action.unit_id = unit_id;
            action.action_type = "move";
            action.parameters["target_x"] = std::to_string(x + dx);
            action.parameters["target_y"] = std::to_string(y + dy);
            
            // Executar l'acció
            std::vector<GameBoard::GameAction> actions = {action};
            std::ostringstream dummy_output;
            game_board->next(actions, dummy_output);
        }
    }
}

void AIAdapter::executeAttack(int attacker_id, int target_id) {
    // Crear acció d'atac
    GameBoard::GameAction action;
    action.unit_id = attacker_id;
    action.action_type = "attack";
    action.parameters["target_unit_id"] = std::to_string(target_id);
    
    // Executar l'acció
    std::vector<GameBoard::GameAction> actions = {action};
    std::ostringstream dummy_output;
    game_board->next(actions, dummy_output);
}

void AIAdapter::executeCapture(int unit_id) {
    // Crear acció de captura
    GameBoard::GameAction action;
    action.unit_id = unit_id;
    action.action_type = "capture";
    
    // Executar l'acció
    std::vector<GameBoard::GameAction> actions = {action};
    std::ostringstream dummy_output;
    game_board->next(actions, dummy_output);
}

int AIAdapter::generateRandom(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

std::vector<int> AIAdapter::generateRandomPermutation(int n) {
    std::vector<int> permutation(n);
    for (int i = 0; i < n; ++i) {
        permutation[i] = i;
    }
    
    // Fisher-Yates shuffle
    for (int i = n - 1; i > 0; --i) {
        int j = generateRandom(0, i);
        std::swap(permutation[i], permutation[j]);
    }
    
    return permutation;
}

bool AIAdapter::isValidPosition(int x, int y) {
    auto grid = game_board->getBoardGrid();
    return x >= 0 && x < static_cast<int>(grid.size()) && 
           y >= 0 && y < static_cast<int>(grid[0].size());
}

std::string AIAdapter::directionToString(int direction) {
    static const std::vector<std::string> directions = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    if (direction >= 0 && direction < static_cast<int>(directions.size())) {
        return directions[direction];
    }
    return "N";
}

int AIAdapter::stringToDirection(const std::string& direction) {
    static const std::map<std::string, int> direction_map = {
        {"N", 0}, {"NE", 1}, {"E", 2}, {"SE", 3},
        {"S", 4}, {"SW", 5}, {"W", 6}, {"NW", 7}
    };
    
    auto it = direction_map.find(direction);
    return (it != direction_map.end()) ? it->second : 0;
}

} // namespace AIAdapter
