#ifndef AI_FUNCTIONS_HH
#define AI_FUNCTIONS_HH

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <variant>

/**
 * Sistema de funcions per a IAs que permet compatibilitat amb jocs existents
 * i mapeig automàtic a funcions del framework
 */

namespace AIFunctions {

// Tipus de dades que poden retornar les funcions
using ReturnType = std::variant<int, double, bool, std::string, std::vector<int>>;

// Paràmetres de les funcions
struct Parameter {
    std::string name;
    std::string type;
    std::string value;
    
    Parameter(const std::string& n, const std::string& t, const std::string& v = "")
        : name(n), type(t), value(v) {}
};

// Definició d'una funció de l'IA
struct AIFunction {
    std::string name;
    std::string return_type;
    std::vector<Parameter> parameters;
    std::string description;
    std::string framework_mapping;
    std::map<std::string, std::string> param_mapping;
    
    AIFunction() = default;
    AIFunction(const std::string& n, const std::string& rt, const std::string& desc = "")
        : name(n), return_type(rt), description(desc) {}
};

// Definició d'una acció de l'IA
struct AIAction {
    std::string name;
    std::vector<Parameter> parameters;
    std::string description;
    std::string framework_mapping;
    std::map<std::string, std::string> param_mapping;
    
    AIAction() = default;
    AIAction(const std::string& n, const std::string& desc = "")
        : name(n), description(desc) {}
};

// Definició d'una funció d'utilitat
struct AIUtility {
    std::string name;
    std::string return_type;
    std::vector<Parameter> parameters;
    std::string description;
    std::string framework_mapping;
    std::map<std::string, std::string> param_mapping;
    
    AIUtility() = default;
    AIUtility(const std::string& n, const std::string& rt, const std::string& desc = "")
        : name(n), return_type(rt), description(desc) {}
};

// Interfície base per a les IAs
class AIInterface {
public:
    virtual ~AIInterface() = default;
    
    // Funcions de consulta
    virtual std::vector<int> farmers(int player_id) = 0;
    virtual std::vector<int> knights(int player_id) = 0;
    virtual std::vector<int> witches(int player_id) = 0;
    virtual std::string unit(int unit_id) = 0;
    virtual std::string cell(int x, int y) = 0;
    virtual double status(int player_id) = 0;
    virtual int total_score(int player_id) = 0;
    virtual int land(int player_id) = 0;
    virtual int round() = 0;
    virtual int nb_players() = 0;
    virtual int nb_units() = 0;
    virtual int nb_farmers(int player_id) = 0;
    virtual int nb_knights(int player_id) = 0;
    virtual int nb_witches(int player_id) = 0;
    
    // Accions
    virtual void command(int unit_id, const std::string& direction) = 0;
    virtual void attack(int attacker_id, int target_id) = 0;
    virtual void capture(int unit_id) = 0;
    
    // Utilitats
    virtual int random(int min, int max) = 0;
    virtual std::vector<int> random_permutation(int n) = 0;
    virtual bool pos_ok(int x, int y) = 0;
};

// Gestor de funcions de l'IA
class AIFunctionManager {
private:
    std::map<std::string, AIFunction> functions;
    std::map<std::string, AIAction> actions;
    std::map<std::string, AIUtility> utilities;
    std::map<std::string, std::string> function_mappings;
    
    // Funcions privades de parsing
    void parseAIFunction(const std::string& line);
    void parseAIAction(const std::string& line);
    void parseAIUtility(const std::string& line);
    void parseFunctionMapping(const std::string& line);
    void parseParameters(std::vector<Parameter>& params, const std::string& params_str);
    void parseParameterMapping(std::map<std::string, std::string>& mapping, const std::string& mapping_str);
    
public:
    AIFunctionManager() = default;
    
    // Carregar definicions des d'un fitxer de configuració
    bool loadFromFile(const std::string& filename);
    
    // Registrar funcions, accions i utilitats
    void registerFunction(const AIFunction& func);
    void registerAction(const AIAction& action);
    void registerUtility(const AIUtility& utility);
    
    // Obtenir definicions
    const AIFunction* getFunction(const std::string& name) const;
    const AIAction* getAction(const std::string& name) const;
    const AIUtility* getUtility(const std::string& name) const;
    
    // Mapeig de funcions
    std::string getFrameworkMapping(const std::string& ai_function) const;
    
    // Validar crides de funcions
    bool validateFunctionCall(const std::string& name, const std::vector<Parameter>& params) const;
    bool validateActionCall(const std::string& name, const std::vector<Parameter>& params) const;
    bool validateUtilityCall(const std::string& name, const std::vector<Parameter>& params) const;
    
    // Llistar totes les funcions disponibles
    std::vector<std::string> listAvailableFunctions() const;
    std::vector<std::string> listAvailableActions() const;
    std::vector<std::string> listAvailableUtilities() const;
};

} // namespace AIFunctions

#endif // AI_FUNCTIONS_HH
