#include "AIFunctions.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

namespace AIFunctions {

// Implementació del gestor de funcions de l'IA

bool AIFunctionManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No s'ha pogut obrir el fitxer " << filename << std::endl;
        return false;
    }
    
    std::string line;
    std::string current_section = "";
    
    while (std::getline(file, line)) {
        // Ignorar línies buides i comentaris
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Detectar seccions
        if (line[0] == '[' && line[line.length()-1] == ']') {
            current_section = line.substr(1, line.length()-2);
            continue;
        }
        
        // Processar línies segons la secció
        if (current_section == "AI_FUNCTIONS") {
            parseAIFunction(line);
        } else if (current_section == "AI_ACTIONS") {
            parseAIAction(line);
        } else if (current_section == "AI_UTILITIES") {
            parseAIUtility(line);
        } else if (current_section == "FUNCTION_MAPPINGS") {
            parseFunctionMapping(line);
        }
    }
    
    file.close();
    return true;
}

void AIFunctionManager::parseAIFunction(const std::string& line) {
    std::istringstream iss(line);
    std::string name, return_type, description;
    
    // Format: function_name "return_type" "param1=type1 param2=type2 ..." "description"
    if (iss >> name) {
        // Llegir return_type amb cometes
        if (iss >> std::quoted(return_type)) {
            AIFunction func(name, return_type);
            
            // Llegir paràmetres si n'hi ha
            std::string params_str;
            if (iss >> std::quoted(params_str)) {
                parseParameters(func.parameters, params_str);
            }
            
            // Llegir descripció si n'hi ha
            if (iss >> std::quoted(description)) {
                func.description = description;
            }
            
            registerFunction(func);
        } else {
            std::cerr << "Warning: No es pot llegir return_type per a la funció: " << name << std::endl;
        }
    }
}

void AIFunctionManager::parseAIAction(const std::string& line) {
    std::istringstream iss(line);
    std::string name, description;
    
    // Format: action_name "param1=type1 param2=type2 ..." "description"
    if (iss >> name) {
        AIAction action(name);
        
        // Llegir paràmetres si n'hi ha
        std::string params_str;
        if (iss >> std::quoted(params_str)) {
            parseParameters(action.parameters, params_str);
        }
        
        // Llegir descripció si n'hi ha
        if (iss >> std::quoted(description)) {
            action.description = description;
        }
        
        registerAction(action);
    }
}

void AIFunctionManager::parseAIUtility(const std::string& line) {
    std::istringstream iss(line);
    std::string name, return_type, description;
    
    // Format: utility_name "return_type" "param1=type1 param2=type2 ..." "description"
    if (iss >> name) {
        // Llegir return_type amb cometes
        if (iss >> std::quoted(return_type)) {
            AIUtility utility(name, return_type);
            
            // Llegir paràmetres si n'hi ha
            std::string params_str;
            if (iss >> std::quoted(params_str)) {
                parseParameters(utility.parameters, params_str);
            }
            
            // Llegir descripció si n'hi ha
            if (iss >> std::quoted(description)) {
                utility.description = description;
            }
            
            registerUtility(utility);
        } else {
            std::cerr << "Warning: No es pot llegir return_type per a la utilitat: " << name << std::endl;
        }
    }
}

void AIFunctionManager::parseFunctionMapping(const std::string& line) {
    std::istringstream iss(line);
    std::string ai_function, equals, framework_function, mapping_str;
    
    // Format: ai_function_name = framework_function_name "param_mapping..."
    if (iss >> ai_function >> equals >> framework_function) {
        if (equals == "=") {
            // Llegir mapeig de paràmetres si n'hi ha
            if (iss >> std::quoted(mapping_str)) {
                // Crear un mapa temporal per al mapeig de paràmetres
                std::map<std::string, std::string> param_mapping;
                parseParameterMapping(param_mapping, mapping_str);
                // Aquí podríem emmagatzemar el mapeig si fos necessari
            }
            
            // Registrar el mapeig
            function_mappings[ai_function] = framework_function;
        }
    }
}

void AIFunctionManager::parseParameters(std::vector<Parameter>& params, const std::string& params_str) {
    std::istringstream iss(params_str);
    std::string param;
    
    while (std::getline(iss, param, ' ')) {
        if (param.empty()) continue;
        
        size_t equal_pos = param.find('=');
        if (equal_pos != std::string::npos) {
            std::string name = param.substr(0, equal_pos);
            std::string type = param.substr(equal_pos + 1);
            params.emplace_back(name, type);
        }
    }
}

void AIFunctionManager::parseParameterMapping(std::map<std::string, std::string>& mapping, const std::string& mapping_str) {
    std::istringstream iss(mapping_str);
    std::string param;
    
    while (std::getline(iss, param, ' ')) {
        if (param.empty()) continue;
        
        size_t equal_pos = param.find('=');
        if (equal_pos != std::string::npos) {
            std::string ai_param = param.substr(0, equal_pos);
            std::string framework_param = param.substr(equal_pos + 1);
            mapping[ai_param] = framework_param;
        }
    }
}

void AIFunctionManager::registerFunction(const AIFunction& func) {
    functions[func.name] = func;
}

void AIFunctionManager::registerAction(const AIAction& action) {
    actions[action.name] = action;
}

void AIFunctionManager::registerUtility(const AIUtility& utility) {
    utilities[utility.name] = utility;
}

const AIFunction* AIFunctionManager::getFunction(const std::string& name) const {
    auto it = functions.find(name);
    return (it != functions.end()) ? &it->second : nullptr;
}

const AIAction* AIFunctionManager::getAction(const std::string& name) const {
    auto it = actions.find(name);
    return (it != actions.end()) ? &it->second : nullptr;
}

const AIUtility* AIFunctionManager::getUtility(const std::string& name) const {
    auto it = utilities.find(name);
    return (it != utilities.end()) ? &it->second : nullptr;
}

std::string AIFunctionManager::getFrameworkMapping(const std::string& ai_function) const {
    auto it = function_mappings.find(ai_function);
    return (it != function_mappings.end()) ? it->second : "";
}

bool AIFunctionManager::validateFunctionCall(const std::string& name, const std::vector<Parameter>& params) const {
    const AIFunction* func = getFunction(name);
    if (!func) return false;
    
    // Verificar nombre de paràmetres
    if (func->parameters.size() != params.size()) return false;
    
    // Verificar tipus de paràmetres (implementació bàsica)
    for (size_t i = 0; i < params.size(); ++i) {
        if (func->parameters[i].name != params[i].name) return false;
    }
    
    return true;
}

bool AIFunctionManager::validateActionCall(const std::string& name, const std::vector<Parameter>& params) const {
    const AIAction* action = getAction(name);
    if (!action) return false;
    
    // Verificar nombre de paràmetres
    if (action->parameters.size() != params.size()) return false;
    
    return true;
}

bool AIFunctionManager::validateUtilityCall(const std::string& name, const std::vector<Parameter>& params) const {
    const AIUtility* utility = getUtility(name);
    if (!utility) return false;
    
    // Verificar nombre de paràmetres
    if (utility->parameters.size() != params.size()) return false;
    
    return true;
}

std::vector<std::string> AIFunctionManager::listAvailableFunctions() const {
    std::vector<std::string> result;
    for (const auto& pair : functions) {
        result.push_back(pair.first);
    }
    return result;
}

std::vector<std::string> AIFunctionManager::listAvailableActions() const {
    std::vector<std::string> result;
    for (const auto& pair : actions) {
        result.push_back(pair.first);
    }
    return result;
}

std::vector<std::string> AIFunctionManager::listAvailableUtilities() const {
    std::vector<std::string> result;
    for (const auto& pair : utilities) {
        result.push_back(pair.first);
    }
    return result;
}

} // namespace AIFunctions
