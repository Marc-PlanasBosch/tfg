#include "UnitTypeRegistry.hh"
#include <algorithm>

UnitTypeRegistry::UnitTypeRegistry() {
    // Registrar tipus bàsics per defecte
    registerUnitType("farmers", FARMER);
    registerUnitType("knights", KNIGHT);
    registerUnitType("witches", WITCH);
}

void UnitTypeRegistry::registerUnitType(const std::string& name, UnitType type) {
    name_to_enum_[name] = type;
    enum_to_name_[type] = name;
    
    // Afegir a la llista si no existeix
    if (std::find(registered_types_.begin(), registered_types_.end(), name) == registered_types_.end()) {
        registered_types_.push_back(name);
    }
}

UnitType UnitTypeRegistry::getUnitTypeEnum(const std::string& name) const {
    auto it = name_to_enum_.find(name);
    return (it != name_to_enum_.end()) ? it->second : UNKNOWN;
}

std::string UnitTypeRegistry::getUnitTypeName(UnitType type) const {
    auto it = enum_to_name_.find(type);
    return (it != enum_to_name_.end()) ? it->second : "unknown";
}

bool UnitTypeRegistry::isRegistered(const std::string& name) const {
    return name_to_enum_.find(name) != name_to_enum_.end();
}

bool UnitTypeRegistry::isRegistered(UnitType type) const {
    return enum_to_name_.find(type) != enum_to_name_.end();
}
