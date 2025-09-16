#ifndef UnitTypeRegistry_hh
#define UnitTypeRegistry_hh

#include <string>
#include <map>
#include <vector>

enum UnitType {
    FARMER = 0,
    KNIGHT = 1,
    WITCH = 2,
    // Afegir nous tipus segons necessitat
    UNKNOWN = -1
};

class UnitTypeRegistry {
private:
    std::map<std::string, UnitType> name_to_enum_;
    std::map<UnitType, std::string> enum_to_name_;
    std::vector<std::string> registered_types_;
    
public:
    UnitTypeRegistry();
    ~UnitTypeRegistry() = default;
    
    // Registrar un tipus d'unitat
    void registerUnitType(const std::string& name, UnitType type);
    
    // Convertir entre string i enum
    UnitType getUnitTypeEnum(const std::string& name) const;
    std::string getUnitTypeName(UnitType type) const;
    
    // Obtenir tots els tipus registrats
    const std::vector<std::string>& getRegisteredTypes() const { return registered_types_; }
    
    // Verificar si un tipus està registrat
    bool isRegistered(const std::string& name) const;
    bool isRegistered(UnitType type) const;
    
    // Obtenir el nombre de tipus registrats
    int getTypeCount() const { return registered_types_.size(); }
};

#endif
