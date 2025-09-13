#include "Core/Player.hh"
#include "Core/Board.hh"
#include "Core/Action.hh"
#include <iostream>
#include <random>
#include <vector>

/**
 * Dummy player per BolaDeDrac2015 compatible amb Windows
 * Implementa la mateixa lògica que l'original AIDemo.cc
 */
class BolaDeDracDummy : public Player {
private:
    std::mt19937 rng;
    int current_direction;
    int round_count;
    
    // Direccions del joc (compatibles amb l'original)
    enum Dir {
        Left = 6, Right = 2, Bottom = 4, Top = 0,
        None = -1
    };
    
public:
    BolaDeDracDummy() : rng(std::random_device{}()), current_direction(None), round_count(0) {}
    
    void play() override {
        round_count++;
        
        // Inicialització (com l'original)
        if (round_count == 1) {
            current_direction = rand_dir(None);
        }
        
        // Obtenir el Goku del jugador
        auto units = board->getUnitsByPlayer(player_id);
        if (units.empty()) return;
        
        // Lògica del dummy original
        if (randomize() % 2 == 0) {
            // Llançar Kamehame
            action->throw_kamehame(current_direction);
        } else {
            // Moure
            if (randomize() % 100 == 0 || !can_move(units[0], current_direction)) {
                current_direction = rand_dir(current_direction);
            }
            action->move(current_direction);
        }
    }
    
private:
    bool can_move(const Board::Unit& unit, int direction) {
        // Simplificat: sempre retorna true per ara
        // En una implementació completa, verificaríem obstacles i altres unitats
        return true;
    }
    
    int rand_dir(int not_dir) {
        std::vector<int> directions = {Left, Right, Bottom, Top};
        std::uniform_int_distribution<int> dist(0, 3);
        
        while (true) {
            int d = directions[dist(rng)];
            if (d != not_dir) return d;
        }
    }
    
    int randomize() {
        std::uniform_int_distribution<int> dist(0, 999);
        return dist(rng);
    }
};

// Factory function per compatibilitat
extern "C" Player* new_player() {
    return new BolaDeDracDummy();
}
