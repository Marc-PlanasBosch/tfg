#include <algorithm>

#include "Board.hh"
#include "Action.hh"
#include "UnitTypeRegistry.hh"

#include <sstream>
#include <fstream>

#include <cctype>

using namespace std;

Board::Board (istream& is, bool secgame, int seed) {
    string s, v;

    // Carregar configuració dinàmica
    loadDynamicConfiguration();

    //Version, compared part by part
    istringstream vs(version());
    while (!vs.eof()) {
        is >> s;
        vs >> v;
        assert(s == v);
    };

    is >> s >> nb_players_;
    assert(s == "nb_players");
    assert(nb_players_ == 2 || nb_players_ == 4);

    is >> s >> nb_rounds_;
    assert(s == "nb_rounds");
    assert(nb_rounds_ >= 1);

    // FASE 2: Llegir configuració del mapa com a key-value pairs
    std::cerr << "[DEBUG] === FASE 2: Llegint configuració del mapa ===" << std::endl;
    
    // Llegir tot el fitxer del mapa com a key-value pairs
    map<string, string> map_config;
    string key, value;
    int round_value = 0;
    
    // Llegir fins a trobar "round" (inici del mapa)
    while (is >> key && key != "round") {
        if (key == "names") {
            // Cas especial: names té 4 valors
            string name1, name2, name3, name4;
            is >> name1 >> name2 >> name3 >> name4;
            map_config["names"] = name1 + " " + name2 + " " + name3 + " " + name4;
            std::cerr << "[DEBUG]   names = " << map_config["names"] << std::endl;
        } else {
            is >> value;
            map_config[key] = value;
            std::cerr << "[DEBUG]   " << key << " = " << value << std::endl;
        }
    }
    
    // Llegir round quan el trobem
    if (key == "round") {
        is >> round_value;
        std::cerr << "[DEBUG]   round = " << round_value << std::endl;
    } else {
        // Si no hi ha "round", començar des del principi
        round_value = 0;
        std::cerr << "[DEBUG] No s'ha trobat 'round', començant des del principi" << std::endl;
    }
    
    // Establir seed si s'ha proporcionat
    if (seed >= 0) {
        srand(seed);
        std::cerr << "[DEBUG] Seed establida: " << seed << std::endl;
    }
    
    // Processar configuració del mapa
    std::cerr << "[DEBUG] Processant configuració del mapa..." << std::endl;
    
    // Llegir unitats del mapa
    for (const string& unit_type : game_config.getUnitTypes()) {
        string key = "nb_" + unit_type;
        if (map_config.find(key) != map_config.end()) {
            nb_units_by_type_[unit_type] = stoi(map_config[key]);
            std::cerr << "[DEBUG]   " << key << " = " << nb_units_by_type_[unit_type] << std::endl;
            assert(nb_units_by_type_[unit_type] >= 0);
        }
    }
    
    // Llegir paràmetres d'unitats del mapa
    for (const string& unit_type : game_config.getUnitTypes()) {
        string health_key = unit_type + "_health";
        string regen_key = unit_type + "_regen";
        
        if (map_config.find(health_key) != map_config.end()) {
            unit_health_by_type_[unit_type] = stoi(map_config[health_key]);
            std::cerr << "[DEBUG]   " << health_key << " = " << unit_health_by_type_[unit_type] << std::endl;
            assert(unit_health_by_type_[unit_type] >= 1);
        }
        
        if (map_config.find(regen_key) != map_config.end()) {
            unit_regen_by_type_[unit_type] = stoi(map_config[regen_key]);
            std::cerr << "[DEBUG]   " << regen_key << " = " << unit_regen_by_type_[unit_type] << std::endl;
            assert(unit_regen_by_type_[unit_type] >= 0);
        }
    }
    
    // Llegir damage_min i damage_max del mapa
    if (map_config.find("damage_min") != map_config.end()) {
        damage_min_ = stoi(map_config["damage_min"]);
        std::cerr << "[DEBUG]   damage_min = " << damage_min_ << std::endl;
        assert(damage_min_ >= 0);
    }
    
    if (map_config.find("damage_max") != map_config.end()) {
        damage_max_ = stoi(map_config["damage_max"]);
        std::cerr << "[DEBUG]   damage_max = " << damage_max_ << std::endl;
        assert(damage_max_ >= damage_min_);
    }
    
    std::cerr << "[DEBUG] Paràmetres del mapa llegits correctament" << std::endl;
    
    // Llegir dimensions del mapa
    if (map_config.find("rows") != map_config.end()) {
        rows_ = stoi(map_config["rows"]);
        std::cerr << "[DEBUG]   rows = " << rows_ << std::endl;
        assert(rows_ >= 4);
    }
    
    if (map_config.find("cols") != map_config.end()) {
        cols_ = stoi(map_config["cols"]);
        std::cerr << "[DEBUG]   cols = " << cols_ << std::endl;
        assert(cols_ >= 4);
    }

    // Llegir secgame, names i round des del map_config
    if (map_config.find("secgame") != map_config.end()) {
        secgame_ = (map_config["secgame"] == "true");
        std::cerr << "[DEBUG] secgame = " << map_config["secgame"] << std::endl;
    } else {
        secgame_ = false;
        std::cerr << "[DEBUG] secgame no trobat, utilitzant false per defecte" << std::endl;
    }
    
    if (map_config.find("names") != map_config.end()) {
        names_ = vector<string>(nb_players_);
        istringstream names_stream(map_config["names"]);
        for (int pl = 0; pl < nb_players_; ++pl) {
            names_stream >> names_[pl];
        }
        std::cerr << "[DEBUG] names llegits correctament" << std::endl;
    } else {
        names_ = vector<string>(nb_players_);
        for (int pl = 0; pl < nb_players_; ++pl) {
            names_[pl] = "Player" + to_string(pl);
        }
        std::cerr << "[DEBUG] names no trobats, utilitzant noms per defecte" << std::endl;
    }

    // Utilitzar round llegit anteriorment
    round_ = round_value;
    std::cerr << "[DEBUG] round final = " << round_ << std::endl;
    assert(round_ < nb_rounds_);

    cells_ = vector< vector<Cell> >(rows_, vector<Cell>(cols_));
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            cells_[i][j].unit = -1;
            cells_[i][j].pos = Pos(i, j);
            cells_[i][j].owner = -1;
            cells_[i][j].type = Empty;
            char c;
            is >> c;
            c = toupper(c);
            //cerr << c;
            switch (c) {
                case '0':
                    cells_[i][j].owner = 0;
                    break;
                case '1':
                    cells_[i][j].owner = 1; 
                    break;
                case '2': 
                    cells_[i][j].owner = 2;
                    break;
                case '3': 
                    cells_[i][j].owner = 3; 
                    break;
                case '.': 
                    break; //Empty cell
                case 'X': 
                    cells_[i][j].type = Wall;
                    break;
                default:
                    cerr << "Unexpected '" << c << "' in board definition" << endl;
                    assert(false);
            } 
        }
        //cerr << endl;
    }
    
    is >> s;
    assert(s == "score");
    score_ = vector<int>(nb_players_);
    for (int i = 0; i < nb_players_; ++i) {
        is >> score_[i];
    }

    is >> s;
    assert(s == "status");
    status_ = vector<double>(nb_players_);
    for (int i = 0; i < nb_players_; ++i) {
        is >> status_[i];
    }
    

    // Calcular nombre total d'unitats dinàmicament
    int total_units = 0;
    for (const string& unit_type : game_config.getUnitTypes()) {
        total_units += nb_units_by_type_[unit_type] * nb_players_;
    }
    units_ = vector<Unit>(total_units);
    
    unsigned int id = 0;
    char type;
    while (is >> type) {
        int i, j;
        int player;
        int health;
        
        is >> player >> i >> j >> health;
        
        assert(player >= 0 && player < nb_players_);
        assert(health > 0);
        assert(i >= 0 && i < rows_);
        assert(j >= 0 && j < cols_);        
        
        units_[id].id = id;
        units_[id].player = player;
        units_[id].health = health;
        units_[id].pos.i = i;
        units_[id].pos.j = j;
        cells_[i][j].unit = id;
        
        // Determinar tipus d'unitat dinàmicament
        units_[id].type = getUnitTypeFromString(string(1, type));
        assert(units_[id].type != UNKNOWN);
        
        id++;
        
        if (id >= units_.size()) break;
    }
    
    if (id == 0) { //Empty board given, generate units
        generateUnitsDynamically();
    }
    else if (id < units_.size()) { //Half-full board given, error
        cerr << "Read " << id << " units, expected " << units_.size() << endl;
        assert(false);
    }
    else { //Full board given, ok
        //cerr << "Full board given" << endl;
    }
        
    // Inicialitzar vectors dinàmics per jugador
    for (const string& unit_type : game_config.getUnitTypes()) {
        units_by_player_[unit_type] = vector< vector<int> >(nb_players_);
    }
    vectors_by_player_updated_ = false;

    //cerr << "Board constructor ok" << endl;
    
}


void Board::spawn_unit(int id) {
    Unit& u = units_[id];
    int mini, minj, maxi, maxj;
    switch(u.player) {
        default: 
            assert(0);
        case 0:
            mini = 1;
            maxi = rows_/2 - 1;
            minj = 1;
            maxj = cols_/2 - 1;
            break;
        case 1:
            mini = rows_/2 + 1;
            maxi = rows_ - 2;
            maxj = cols_ - 2;
            minj = cols_/2 + 1;
            break;
        case 2:
            mini = 1;
            maxi = rows_/2 - 1;
            maxj = cols_ - 2;
            minj = cols_/2 + 1;
            break;
        case 3:
            mini = rows_/2 + 1;
            maxi = rows_ - 2;
            minj = 1;
            maxj = cols_/2 - 1;
            break;
    }
    
    int i, j;
    do {
        i = mini + rand() % (maxi-mini);
        j = minj + rand() % (maxj-minj);
    } while(cells_[i][j].type != Empty || cells_[i][j].unit != -1);

    if (pos_ok(u.pos.i,u.pos.j)) cells_[u.pos.i][u.pos.j].unit = -1;
    
    cells_[i][j].unit = id;
    
    u.pos.i = i;
    u.pos.j = j;
    
    if (u.type == FARMER) {
        cells_[i][j].owner = u.player;
    }
    
}

// Implementacions dels mètodes dinàmics

void Board::loadDynamicConfiguration() {
    std::cerr << "[DEBUG] Board::loadDynamicConfiguration() - Iniciant càrrega de configuració dinàmica" << std::endl;
    
    // Carregar configuració si no s'ha fet encara
    if (!game_config.isLoaded()) {
        std::cerr << "[DEBUG] Carregant configuració des de game_config.cnf" << std::endl;
        game_config.loadFromFile("game_config.cnf");
    } else {
        std::cerr << "[DEBUG] Configuració ja carregada" << std::endl;
    }
    
    // Inicialitzar mapes amb valors de la configuració
    std::cerr << "[DEBUG] Inicialitzant mapes per " << game_config.getUnitTypes().size() << " tipus d'unitats" << std::endl;
    for (const string& unit_type : game_config.getUnitTypes()) {
        nb_units_by_type_[unit_type] = 0; // S'omplirà quan es llegeixi el fitxer
        unit_health_by_type_[unit_type] = game_config.getUnitHealth(unit_type);
        unit_regen_by_type_[unit_type] = game_config.getUnitRegen(unit_type);
        std::cerr << "[DEBUG]   - " << unit_type << ": salut=" << unit_health_by_type_[unit_type] 
                  << ", regen=" << unit_regen_by_type_[unit_type] << std::endl;
    }
    std::cerr << "[DEBUG] Board::loadDynamicConfiguration() - Finalitzat" << std::endl;
}

void Board::generateUnitsDynamically() {
    std::cerr << "[DEBUG] Board::generateUnitsDynamically() - Generant unitats dinàmicament" << std::endl;
    unsigned int id = 0;
    for (int player = 0; player < nb_players_; ++player) {
        std::cerr << "[DEBUG]   - Jugador " << player << ":" << std::endl;
        for (const string& unit_type : game_config.getUnitTypes()) {
            std::cerr << "[DEBUG]     * " << unit_type << "s: " << nb_units_by_type_[unit_type] << " unitats" << std::endl;
            for (int rep = 0; rep < nb_units_by_type_[unit_type]; ++rep) {
                units_[id].type = getUnitTypeFromString(unit_type);
                units_[id].id = id;
                units_[id].player = player;
                units_[id].health = unit_health_by_type_[unit_type];
                spawn_unit(id);
                id++;
            }
        }
    }
    std::cerr << "[DEBUG] Total d'unitats generades: " << id << std::endl;
}

UnitType Board::getUnitTypeFromString(const string& type_str) const {
    // Mapeig de caràcters a tipus d'unitat
    if (type_str == "f" || type_str == "farmers") return FARMER;
    if (type_str == "k" || type_str == "knights") return KNIGHT;
    if (type_str == "w" || type_str == "witches") return WITCH;
    return UNKNOWN;
}

string Board::getUnitTypeString(UnitType type) const {
    switch (type) {
        case FARMER: return "farmers";
        case KNIGHT: return "knights";
        case WITCH: return "witches";
        default: return "unknown";
    }
}

bool Board::isUnitTypeValid(const string& type) const {
    return game_config.hasUnitType(type);
}

void Board::update_vectors_by_player() {
    // Netejar tots els vectors per jugador
    for (const string& unit_type : game_config.getUnitTypes()) {
        for (int p = 0; p < nb_players(); ++p) {
            units_by_player_[unit_type][p].clear();
        }
    }
    
    // Omplir vectors dinàmicament
    for (int id = 0; id < nb_units(); ++id) {
        const Unit& u = units_[id];
        string unit_type = getUnitTypeString(u.type);
        if (isUnitTypeValid(unit_type)) {
            units_by_player_[unit_type][u.player].push_back(id);
        } else {
            cerr << "Unknown unit type: " << u.type << endl;
            assert(0);
        }
    }
    vectors_by_player_updated_ = true;
}

void Board::update_scores() {
    //Update scores
    for (int player = 0; player < nb_players(); ++player) {
        score_[player] = 0;
    }
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            int o = cells_[i][j].owner;
            if (o >= 0) score_[o]++;
        }
    }
}

// Definir la instància estàtica
GameConfiguration Board::game_config;

string Board::version() {
    // Carregar configuració si no s'ha fet encara
    if (!game_config.isLoaded()) {
        game_config.loadFromFile("game_config.cnf");
    }
    
    return game_config.getFullVersion();
}

void Board::print_preamble (ostream& os) const {
    os << version() << endl;
    os << "nb_players " << nb_players() << endl;
    os << "nb_rounds " << nb_rounds() << endl;
    os << "nb_farmers " << nb_farmers() << endl;
    os << "nb_knights " << nb_knights() << endl;
    os << "farmers_health " << farmers_health() << endl;
    os << "knights_health " << knights_health() << endl;
    os << "farmers_regen " << farmers_regen() << endl;
    os << "knights_regen " << knights_regen() << endl;
    os << "damage_min " << damage_min() << endl;
    os << "damage_max " << damage_max() << endl;
    os << "rows " << rows() << endl;
    os << "cols " << cols() << endl;
    os << "secgame " << (secgame()? "true":"false") << endl;
    os << "names";
    for (int player = 0; player < nb_players(); ++player) os << " " << names_[player];
    os << endl << endl;
}


void Board::print (ostream& os) const {
    os << endl;
    
    os << "round " << round() << endl;
    
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            const Cell& c = cells_[i][j];
            if (c.type == Wall) os << 'X';
            else if (c.owner < 0) os << '.';
            else os << c.owner;
        }
        os << endl;
    }

    os << "score";
    for (int i = 0; i < nb_players(); ++i) os << " " << score_[i];
    os << endl;
    
    os << "status";
    for (int i = 0; i < nb_players(); ++i) os << " " << status_[i];
    os << endl;
    
    for (int id = 0; id < nb_units(); ++id) {

        string s;
        if (unit(id).type == KNIGHT) s = "k";
        else if (unit(id).type == FARMER) s = "f";
        else if (unit(id).type == WITCH) s = "w";
        else assert(0);

        int i, j, health, player;
        os  << s << " "
            << unit(id).player << " "    
            << unit(id).pos.i << " "
            << unit(id).pos.j << " "
            << unit(id).health << endl;

    }
    os << endl;
}




void Board::print_debug (ostream& os) const {
    print(os);
}




Board Board::next (const vector<Action>& as, Action& actions_done) const {

    // b will be the new board we shall return
    Board b = *this;

    // increment the round
    ++b.round_;

    // randomize turns
    vector<int> turns(nb_players());
    for (int player = 0; player < nb_players(); ++player) {
        turns[player] = player;
    }
    random_shuffle(turns.begin(), turns.end());

    // move each unit
    vector<bool> moved(nb_units(), false);
    for (int turn = 0; turn < nb_players(); ++turn) {
        int player = turns[turn];
        queue<Movement> q = as[player].q_;
        while (not q.empty()) {
            Movement m = q.front();  q.pop();
            int id = m.i;
            Dir dir = m.d;
            if (!unit_ok(id)) {
                cerr << "Id out of range" << endl;
                continue;
            }
            if (dir != None and dir != Left and dir != Right and dir != Top and dir != Bottom) {
                cerr << "Direction not valid" << endl;
                continue;
            }
            if (moved[id]) continue;
            moved[id] = true;
            bool ok = b.move(player, id, dir);
            if (ok) {
                actions_done.q_.push(Movement(id, dir));
            }
        }
    }

    //Heal units that have not moved
    for (int id = 0; id < nb_units(); id++) {
        if (moved[id]) continue;
        Unit& u = b.units_[id];
        string unit_type = getUnitTypeString(u.type);
        if (isUnitTypeValid(unit_type)) {
            u.health += b.unit_regen_by_type_[unit_type];
            int max_health = b.unit_health_by_type_[unit_type];
            if (u.health > max_health) u.health = max_health;
        } else {
            cerr << "Unknown unit type for healing: " << u.type << endl;
            assert(0);
        }
    }

    b.update_scores();

    return b;
}



bool Board::move (int player, int id, Dir d) {

    if (d == None) return false;

    Unit& u1 = units_[id];
    if (u1.player != player) return false; //Trying to move another player's unit

    Pos p1 = u1.pos;
    if (not pos_ok(p1)) return false;

    Pos p2 = dest(p1, d);
    if (not pos_ok(p2)) return false;

    Cell& c1 = cells_[p1.i][p1.j];
    Cell& c2 = cells_[p2.i][p2.j];

    if (c2.type == Wall) return false;

    if (c2.unit != -1) { //There is a unit at our destination
        
        // Verificar si aquesta unitat pot atacar (només knights i witches)
        string unit_type = getUnitTypeString(u1.type);
        if (unit_type != "knights" && unit_type != "witches") return false;
            
        Unit& u2 = units_[c2.unit];
        if (u2.player == u1.player) return false; //Trying to attack your own unit
        
        int damage = damage_min_ + rand() % (damage_max_ - damage_min_);
        u2.health -= damage;
        if (u2.health <= 0) {
            u2.player = u1.player;
            string target_type = getUnitTypeString(u2.type);
            u2.health = unit_health_by_type_[target_type];
            spawn_unit(u2.id);
            vectors_by_player_updated_ = false;
        }
        
        return true;
        
    } else { //We can move there

        c2.unit = c1.unit;
        c1.unit = -1;
        u1.pos = p2;
        
        // Només farmers i witches poden conquerir territori
        string unit_type = getUnitTypeString(u1.type);
        if (unit_type == "farmers" || unit_type == "witches") {
            c2.owner = u1.player;
        }
        
        return true;

    }

    
}

