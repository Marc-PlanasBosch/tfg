#include "Action.hh"
#include <iostream>
#include <sstream>

Action::Action() {
}

Action::Action(std::istream& is) {
    read(is);
}

bool Action::command(int unit_id, int direction) {
    return command(Movement(unit_id, direction));
}

bool Action::command(const Movement& movement) {
    // Verificar si la unitat ja ha actuat aquesta ronda
    if (units_acted_.find(movement.unit_id) != units_acted_.end()) {
        std::cerr << "warning: action already requested for unit " << movement.unit_id << std::endl;
        return false;
    }
    
    // Afegir el moviment
    movements_.push(movement);
    units_acted_.insert(movement.unit_id);
    
    return true;
}

void Action::move(int direction) {
    // Per BolaDeDrac2015, només hi ha un Goku per jugador
    // Utilitzem unit_id = 0 per simplicitat
    command(Movement(0, direction, Moving));
}

void Action::throw_kamehame(int direction) {
    // Per BolaDeDrac2015, només hi ha un Goku per jugador
    // Utilitzem unit_id = 0 per simplicitat
    command(Movement(0, direction, Throwing));
}

void Action::move(int unit_id, int direction) {
    command(Movement(unit_id, direction, Moving));
}

void Action::attack(int direction) {
    // Per Dominator, utilitzem unit_id = 0 per simplicitat
    command(Movement(0, direction, Moving));
}

void Action::attack(int unit_id, int direction) {
    command(Movement(unit_id, direction, Moving));
}

bool Action::hasUnitActed(int unit_id) const {
    return units_acted_.find(unit_id) != units_acted_.end();
}

void Action::clear() {
    units_acted_.clear();
    while (!movements_.empty()) {
        movements_.pop();
    }
}

void Action::print(std::ostream& os) const {
    std::queue<Movement> temp = movements_;
    while (!temp.empty()) {
        Movement m = temp.front();
        temp.pop();
        os << m.unit_id << " " << directionToChar(m.direction) << std::endl;
    }
    os << -1 << std::endl;
}

void Action::read(std::istream& is) {
    clear();
    
    int unit_id;
    while (is >> unit_id && unit_id != -1) {
        char direction_char;
        if (is >> direction_char) {
            int direction = charToDirection(direction_char);
            command(unit_id, direction);
        } else {
            std::cerr << "warning: only half an operation given for unit " << unit_id << std::endl;
            return;
        }
    }
}

int Action::charToDirection(char c) {
    switch (c) {
        case 'b': return 4; // Bottom (S)
        case 'w': return 5; // BR (SW)
        case 'r': return 2; // Right (E)
        case 'x': return 3; // RT (SE)
        case 't': return 0; // Top (N)
        case 'y': return 7; // TL (NW)
        case 'l': return 6; // Left (W)
        case 'z': return 1; // LB (NE)
        case 'n': return -1; // None
        default: return -1;
    }
}

char Action::directionToChar(int dir) {
    switch (dir) {
        case 0: return 't'; // N
        case 1: return 'z'; // NE
        case 2: return 'r'; // E
        case 3: return 'x'; // SE
        case 4: return 'b'; // S
        case 5: return 'w'; // SW
        case 6: return 'l'; // W
        case 7: return 'y'; // NW
        default: return 'n';
    }
}