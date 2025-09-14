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
    
    // Imprimir header del joc (format Dominator)
    os << "Game" << std::endl;
    os << std::endl;
    os << "Seed 123" << std::endl;
    os << std::endl;
    
    // Imprimir informació del joc (format idèntic al Dominator)
    os << game_def->getGameName() << " " << game_def->getVersion() << std::endl;
    os << "nb_players     " << game_def->getMaxPlayers() << std::endl;
    os << "nb_rounds      " << game_def->getDefaultRounds() << std::endl;
    os << "nb_farmers     " << game_def->getGameConstant("nb_farmers", "20") << std::endl;
    os << "nb_knights     " << game_def->getGameConstant("nb_knights", "10") << std::endl;
    os << "nb_witches     " << game_def->getGameConstant("nb_witches", "2") << std::endl;
    os << "farmers_health " << game_def->getGameConstant("farmers_health", "100") << std::endl;
    os << "knights_health " << game_def->getGameConstant("knights_health", "200") << std::endl;
    os << "farmers_regen  " << game_def->getGameConstant("farmers_regen", "30") << std::endl;
    os << "knights_regen  " << game_def->getGameConstant("knights_regen", "30") << std::endl;
    os << "damage_min     " << game_def->getGameConstant("damage_min", "60") << std::endl;
    os << "damage_max     " << game_def->getGameConstant("damage_max", "90") << std::endl;
    os << "rows           " << board->getRows() << std::endl;
    os << "cols           " << board->getCols() << std::endl;
    os << "names          ";
    for (int i = 0; i < static_cast<int>(players.size()); ++i) {
        if (i > 0) os << " ";
        if (i == 1) os << "Null";  // Jugador 1 és "Null" com en el Dominator original
        else os << "Demo";
    }
    os << std::endl;
    os << std::endl;
    os << std::endl;
    
    // Imprimir estat inicial (format Dominator)
    board->printRoundState(os);
    
    // Executar totes les rondes (començant per la ronda 1)
    for (current_round = 1; current_round <= game_def->getDefaultRounds(); ++current_round) {
        std::cerr << "Iniciant ronda " << current_round << std::endl;
        playRound(os);
    }
    
    std::cerr << "Partida completada" << std::endl;
}

void Game::playRound(std::ostream& os) {
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
    }
    
    // Aplicar totes les accions al tauler
    Action actions_done;
    std::shared_ptr<Board> new_board = board->next(actions, actions_done);
    
    // Imprimir moviments realitzats (format Dominator)
    os << "movements" << std::endl;
    actions_done.print(os);
    
    // Imprimir estat de la ronda (format Dominator)
    new_board->printRoundState(os);
    
    // Actualitzar tauler
    board = new_board;
}
