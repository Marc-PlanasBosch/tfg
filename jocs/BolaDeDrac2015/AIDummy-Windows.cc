#include <iostream>
#include <random>
#include <vector>

// Definició simple de les classes necessàries per evitar dependències de Linux
class Player {
public:
    int player_id;
    int* board_ptr; // Simplificat
    
    virtual ~Player() = default;
    virtual void play() = 0;
    virtual void reset(int id, int* board) {
        player_id = id;
        board_ptr = board;
    }
};

// Dummy player simple per Windows
class AIDummy : public Player {
private:
    std::mt19937 rng;
    
public:
    AIDummy() : rng(std::random_device{}()) {}
    
    void play() override {
        // Lògica simple del dummy
        std::uniform_int_distribution<int> action_dist(0, 1);
        std::uniform_int_distribution<int> dir_dist(0, 7);
        
        int action_type = action_dist(rng);
        int direction = dir_dist(rng);
        
        // Simular acció (en un joc real, això es comunicaria amb el framework)
        std::cout << "Player " << player_id << " action: " 
                  << (action_type == 0 ? "move" : "throw") 
                  << " direction " << direction << std::endl;
    }
};

// Factory function
extern "C" Player* new_player() {
    return new AIDummy();
}
