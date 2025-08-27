#include "GameBoard.hh"
#include <iostream>
#include <sstream>
#include <algorithm>

GameBoard::GameBoard(std::shared_ptr<GameDefinition> def, std::shared_ptr<GameMap> map)
    : game_def(def), game_map(map) {
    
    if (game_map) {
        current_state.scores.resize(game_map->getNbPlayers(), 0);
        current_state.status.resize(game_map->getNbPlayers(), 0);
    }
}

bool GameBoard::initialize() {
    if (!game_def || !game_map) {
        std::cerr << "Error: GameDefinition o GameMap no inicialitzats" << std::endl;
        return false;
    }
    
    initializeBoard();
    placeInitialUnits();
    updateBoardGrid();
    
    return true;
}

void GameBoard::reset() {
    current_state.round = 0;
    std::fill(current_state.scores.begin(), current_state.scores.end(), 0);
    std::fill(current_state.status.begin(), current_state.status.end(), 0);
    
    unit_positions.clear();
    unit_health.clear();
    unit_owners.clear();
    
    if (game_map) {
        placeInitialUnits();
        updateBoardGrid();
    }
}

bool GameBoard::next(const std::vector<GameAction>& actions, std::ostream& os) {
    if (isGameOver()) return false;
    
    // Processar accions dels jugadors
    processUnitActions(actions);
    
    // Aplicar regles del joc
    applyGameRules();
    
    // Actualitzar estats de les unitats
    updateUnitStates();
    
    // Actualitzar estat del joc
    updateState();
    
    // Incrementar ronda
    current_state.round++;
    
    return true;
}

void GameBoard::updateState() {
    // Actualitzar puntuacions basades en l'estat actual
    for (int player = 0; player < getNbPlayers(); ++player) {
        // Calcular puntuació basada en unitats supervivents, territori controlat, etc.
        int score = 0;
        
        // Puntuació per unitats supervivents
        for (const auto& unit : unit_owners) {
            if (unit.second == player && unit_health.find(unit.first) != unit_health.end()) {
                score += unit_health.at(unit.first);
            }
        }
        
        // Puntuació per territori controlat
        for (int i = 0; i < game_map->getRows(); ++i) {
            for (int j = 0; j < game_map->getCols(); ++j) {
                if (game_map->getCell(i, j).owner == player) {
                    score += 10; // Puntuació per cel·la controlada
                }
            }
        }
        
        current_state.scores[player] = score;
    }
}

bool GameBoard::isGameOver() const {
    return current_state.round >= getNbRounds();
}

bool GameBoard::addUnit(int unit_id, const std::string& unit_type, int player, int x, int y, int health) {
    if (!isValidPosition(x, y)) return false;
    
    // Verificar que la posició està lliure
    if (checkCollision(x, y)) return false;
    
    // Obtenir vida per defecte si no s'especifica
    if (health == -1) {
        const auto* type_def = game_def->getUnitType(unit_type);
        health = type_def ? type_def->initial_health : 100;
    }
    
    // Afegir unitat
    unit_positions[unit_id] = std::to_string(x) + "," + std::to_string(y);
    unit_health[unit_id] = health;
    unit_owners[unit_id] = player;
    
    // Actualitzar tauler
    updateBoardGrid();
    
    return true;
}

bool GameBoard::removeUnit(int unit_id) {
    if (unit_positions.find(unit_id) == unit_positions.end()) return false;
    
    unit_positions.erase(unit_id);
    unit_health.erase(unit_id);
    unit_owners.erase(unit_id);
    
    updateBoardGrid();
    return true;
}

bool GameBoard::moveUnit(int unit_id, int new_x, int new_y) {
    if (unit_positions.find(unit_id) == unit_positions.end()) return false;
    if (!isValidPosition(new_x, new_y)) return false;
    if (checkCollision(new_x, new_y)) return false;
    
    unit_positions[unit_id] = std::to_string(new_x) + "," + std::to_string(new_y);
    updateBoardGrid();
    return true;
}

bool GameBoard::attackUnit(int attacker_id, int target_id) {
    if (unit_positions.find(attacker_id) == unit_positions.end() ||
        unit_positions.find(target_id) == unit_positions.end()) return false;
    
    // Verificar que són de jugadors diferents
    if (unit_owners[attacker_id] == unit_owners[target_id]) return false;
    
    // Obtenir poder d'atac de l'atacant
    const auto* attacker_type = game_def->getUnitType(""); // TODO: obtenir tipus real
    int attack_power = attacker_type ? attacker_type->attack_power : 10;
    
    // Aplicar dany
    unit_health[target_id] -= attack_power;
    
    // Eliminar unitat si ha mort
    if (unit_health[target_id] <= 0) {
        removeUnit(target_id);
    }
    
    updateBoardGrid();
    return true;
}

bool GameBoard::setCell(int x, int y, const std::string& cell_type, int owner) {
    if (!isValidPosition(x, y)) return false;
    
    if (game_map) {
        auto& cell = game_map->getCell(x, y);
        cell.cell_type = cell_type;
        cell.owner = owner;
    }
    
    updateBoardGrid();
    return true;
}

std::string GameBoard::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) return "";
    
    if (game_map) {
        return game_map->getCell(x, y).cell_type;
    }
    
    return "";
}

bool GameBoard::isValidPosition(int x, int y) const {
    if (!game_map) return false;
    return game_map->posOk(x, y);
}

void GameBoard::addScore(int player, int points) {
    if (player >= 0 && player < current_state.scores.size()) {
        current_state.scores[player] += points;
    }
}

int GameBoard::getScore(int player) const {
    if (player >= 0 && player < current_state.scores.size()) {
        return current_state.scores[player];
    }
    return 0;
}

std::vector<int> GameBoard::getScores() const {
    return current_state.scores;
}

void GameBoard::printState(std::ostream& os) const {
    os << "Round " << current_state.round << std::endl;
    
    // Imprimir tauler
    for (int i = 0; i < game_map->getRows(); ++i) {
        for (int j = 0; j < game_map->getCols(); ++j) {
            os << board_grid[i][j] << " ";
        }
        os << std::endl;
    }
    
    // Imprimir puntuacions
    os << "Scores: ";
    for (int i = 0; i < current_state.scores.size(); ++i) {
        os << current_state.scores[i];
        if (i < current_state.scores.size() - 1) os << " ";
    }
    os << std::endl;
}

void GameBoard::printPreamble(std::ostream& os) const {
    if (game_def) {
        os << game_def->getGameName() << " v" << game_def->getVersion() << std::endl;
        os << "Max players: " << game_def->getMaxPlayers() << std::endl;
        os << "Default rounds: " << game_def->getDefaultRounds() << std::endl;
    }
}

void GameBoard::printNames(std::ostream& os) const {
    if (game_map) {
        const auto& names = game_map->getPlayerNames();
        os << "Players: ";
        for (size_t i = 0; i < names.size(); ++i) {
            os << names[i];
            if (i < names.size() - 1) os << " ";
        }
        os << std::endl;
    }
}

void GameBoard::printResults(std::ostream& os) const {
    os << "Game Over!" << std::endl;
    os << "Final scores:" << std::endl;
    
    std::vector<std::pair<int, int>> player_scores;
    for (size_t i = 0; i < current_state.scores.size(); ++i) {
        player_scores.push_back({current_state.scores[i], i});
    }
    
    // Ordenar per puntuació (descendent)
    std::sort(player_scores.begin(), player_scores.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });
    
    for (const auto& ps : player_scores) {
        os << "Player " << ps.second << ": " << ps.first << std::endl;
    }
}

bool GameBoard::validateAction(const GameAction& action) const {
    if (action.unit_id < 0) return false;
    if (unit_positions.find(action.unit_id) == unit_positions.end()) return false;
    
    // Validacions específiques segons el tipus d'acció
    if (action.action_type == "move") {
        // Verificar que la nova posició és vàlida
        auto it = action.parameters.find("x");
        auto jt = action.parameters.find("y");
        if (it == action.parameters.end() || jt == action.parameters.end()) return false;
        
        int new_x = std::stoi(it->second);
        int new_y = std::stoi(jt->second);
        
        if (!isValidPosition(new_x, new_y)) return false;
        if (checkCollision(new_x, new_y)) return false;
    }
    
    return true;
}

bool GameBoard::executeAction(const GameAction& action) {
    if (!validateAction(action)) return false;
    
    if (action.action_type == "move") {
        auto it = action.parameters.find("x");
        auto jt = action.parameters.find("y");
        if (it != action.parameters.end() && jt != action.parameters.end()) {
            int new_x = std::stoi(it->second);
            int new_y = std::stoi(jt->second);
            return moveUnit(action.unit_id, new_x, new_y);
        }
    } else if (action.action_type == "attack") {
        auto it = action.parameters.find("target");
        if (it != action.parameters.end()) {
            int target_id = std::stoi(it->second);
            return attackUnit(action.unit_id, target_id);
        }
    }
    
    return false;
}

void GameBoard::applyGameRules() {
    // Aplicar regles comunes del joc
    processCaptures();
    processRegeneration();
    processSpecialEffects();
}

void GameBoard::updateUnitStates() {
    // Actualitzar estats de les unitats (vida, efectes, etc.)
    for (auto& unit : unit_health) {
        // Aplicar regeneració si és necessari
        // TODO: Implementar lògica de regeneració
    }
}

std::string GameBoard::version() const {
    return game_def ? game_def->getVersion() : "Unknown";
}

std::string GameBoard::name(int player) const {
    if (game_map && player >= 0 && player < game_map->getPlayerNames().size()) {
        return game_map->getPlayerNames()[player];
    }
    return "Player" + std::to_string(player);
}

int GameBoard::score(int player) const {
    return getScore(player);
}

int GameBoard::status(int player) const {
    if (player >= 0 && player < current_state.status.size()) {
        return current_state.status[player];
    }
    return 0;
}

void GameBoard::initializeBoard() {
    if (!game_map) return;
    
    board_grid.clear();
    board_grid.resize(game_map->getRows(), std::vector<std::string>(game_map->getCols(), " "));
}

void GameBoard::placeInitialUnits() {
    if (!game_map) return;
    
    // Col·locar unitats inicials segons el mapa
    for (const auto& unit : game_map->getUnits()) {
        addUnit(std::stoi(unit.unit_type), unit.unit_type, unit.player_id, unit.x, unit.y, unit.health);
    }
}

void GameBoard::updateBoardGrid() {
    if (!game_map) return;
    
    // Actualitzar tauler visual basat en l'estat actual
    for (int i = 0; i < game_map->getRows(); ++i) {
        for (int j = 0; j < game_map->getCols(); ++j) {
            board_grid[i][j] = game_map->getCell(i, j).cell_type;
        }
    }
    
    // Afegir unitats al tauler
    for (const auto& unit : unit_positions) {
        size_t pos = unit.second.find(',');
        if (pos != std::string::npos) {
            int x = std::stoi(unit.second.substr(0, pos));
            int y = std::stoi(unit.second.substr(pos + 1));
            if (isValidPosition(x, y)) {
                board_grid[x][y] = "U"; // Representació de unitat
            }
        }
    }
}

bool GameBoard::checkCollision(int x, int y) const {
    if (!isValidPosition(x, y)) return true;
    
    // Verificar si hi ha una unitat en aquesta posició
    for (const auto& unit : unit_positions) {
        size_t pos = unit.second.find(',');
        if (pos != std::string::npos) {
            int unit_x = std::stoi(unit.second.substr(0, pos));
            int unit_y = std::stoi(unit.second.substr(pos + 1));
            if (unit_x == x && unit_y == y) return true;
        }
    }
    
    // Verificar si la cel·la és un mur
    if (game_map && game_map->getCell(x, y).cell_type == "Wall") return true;
    
    return false;
}

void GameBoard::processUnitActions(const std::vector<GameAction>& actions) {
    for (const auto& action : actions) {
        executeAction(action);
    }
}

void GameBoard::processCaptures() {
    // Implementar lògica de captura de territori
    // TODO: Implementar segons les regles del joc
}

void GameBoard::processRegeneration() {
    // Implementar lògica de regeneració de unitats
    // TODO: Implementar segons les regles del joc
}

void GameBoard::processSpecialEffects() {
    // Implementar efectes especials
    // TODO: Implementar segons les regles del joc
}
