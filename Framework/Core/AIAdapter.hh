#ifndef AI_ADAPTER_HH
#define AI_ADAPTER_HH

#include "AIFunctions.hh"
#include "GameBoard.hh"
#include <memory>

/**
 * Adaptador que converteix les crides de l'IA a funcions del framework
 * Implementa AIInterface i mapeja les crides a GameBoard
 */
namespace AIAdapter {

class AIAdapter : public AIFunctions::AIInterface {
private:
    std::shared_ptr<GameBoard> game_board;
    AIFunctions::AIFunctionManager ai_manager;
    int current_player_id;
    
public:
    AIAdapter(std::shared_ptr<GameBoard> board, int player_id);
    ~AIAdapter() override = default;
    
    // Carregar configuració d'IA
    bool loadAIConfiguration(const std::string& config_file);
    
    // Implementació de AIInterface - Funcions de consulta
    std::vector<int> farmers(int player_id) override;
    std::vector<int> knights(int player_id) override;
    std::vector<int> witches(int player_id) override;
    std::string unit(int unit_id) override;
    std::string cell(int x, int y) override;
    double status(int player_id) override;
    int total_score(int player_id) override;
    int land(int player_id) override;
    int round() override;
    int nb_players() override;
    int nb_units() override;
    int nb_farmers(int player_id) override;
    int nb_knights(int player_id) override;
    int nb_witches(int player_id) override;
    
    // Implementació de AIInterface - Accions
    void command(int unit_id, const std::string& direction) override;
    void attack(int attacker_id, int target_id) override;
    void capture(int unit_id) override;
    
    // Implementació de AIInterface - Utilitats
    int random(int min, int max) override;
    std::vector<int> random_permutation(int n) override;
    bool pos_ok(int x, int y) override;
    
    // Funcions auxiliars
    void setCurrentPlayer(int player_id) { current_player_id = player_id; }
    int getCurrentPlayer() const { return current_player_id; }
    
private:
    // Funcions auxiliars de mapeig
    std::vector<int> getUnitsByTypeAndPlayer(const std::string& unit_type, int player_id);
    int getNbUnitsByTypeAndPlayer(const std::string& unit_type, int player_id);
    std::string getUnitInfo(int unit_id);
    std::string getCellContent(int x, int y);
    double getPlayerStatus(int player_id);
    int getPlayerScore(int player_id);
    int getPlayerLand(int player_id);
    int getCurrentRound();
    int getNbPlayers();
    int getNbUnits();
    void executeMove(int unit_id, const std::string& direction);
    void executeAttack(int attacker_id, int target_id);
    void executeCapture(int unit_id);
    int generateRandom(int min, int max);
    std::vector<int> generateRandomPermutation(int n);
    bool isValidPosition(int x, int y);
    
    // Conversió de tipus
    std::string directionToString(int direction);
    int stringToDirection(const std::string& direction);
};

} // namespace AIAdapter

#endif // AI_ADAPTER_HH
