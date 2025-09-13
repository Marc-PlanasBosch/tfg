#include "Game.hh"
#include <iostream>

Game::Game(std::shared_ptr<GameDefinition> def) 
    : game_def(def), current_round(0) {
}

bool Game::loadMap(const std::string& map_file) {
    // Crear el tauler basat en la definició del joc
    board = std::make_shared<Board>(game_def);
    
    // Carregar el mapa
    if (!board->loadFromFile(map_file)) {
        std::cerr << "Error carregant el mapa: " << map_file << std::endl;
        return false;
    }
    
    // Inicialitzar el tauler
    if (!board->initialize()) {
        std::cerr << "Error inicialitzant el tauler" << std::endl;
        return false;
    }
    
    return true;
}

void Game::addPlayer(std::shared_ptr<Player> player) {
    players.push_back(player);
}

void Game::run(std::ostream& os) {
    if (!board) {
        std::cerr << "Error: No s'ha carregat cap mapa" << std::endl;
        return;
    }
    
    if (static_cast<int>(players.size()) != game_def->getMaxPlayers()) {
        std::cerr << "Error: Nombre de jugadors incorrecte" << std::endl;
        return;
    }
    
    // Imprimir preàmbul
    board->printPreamble(os);
    board->print(os);
    
    // Executar rondes
    for (current_round = 1; current_round <= game_def->getDefaultRounds(); ++current_round) {
        std::cerr << "Iniciant ronda " << current_round << std::endl;
        playRound(os);
    }
    
    std::cerr << "Partida completada" << std::endl;
}

void Game::playRound(std::ostream& os) {
    os << "actions" << std::endl;
    
    // Recollir accions de tots els jugadors
    std::vector<Action> actions;
    for (int player_id = 0; player_id < static_cast<int>(players.size()); ++player_id) {
        std::cerr << "Processant jugador " << player_id << std::endl;
        
        // Crear acció buida
        Action action;
        
        // Configurar el jugador
        players[player_id]->reset(player_id, board, action);
        
        // Fer que el jugador jugui
        players[player_id]->play();
        
        // Afegir l'acció a la llista
        actions.push_back(action);
        
        // Imprimir acció del jugador
        os << player_id << std::endl;
        action.print(os);
    }
    
    // Aplicar totes les accions al tauler
    Action actions_done;
    std::shared_ptr<Board> new_board = board->next(actions, actions_done);
    
    // Imprimir moviments realitzats
    os << std::endl << "movements" << std::endl;
    actions_done.print(os);
    
    // Actualitzar tauler
    board = new_board;
    board->print(os);
}
