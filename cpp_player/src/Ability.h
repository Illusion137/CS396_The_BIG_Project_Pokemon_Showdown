#ifndef ABILITY_H
#define ABILITY_H

#include <string>
#include <unordered_map>

class Ability {
public:
    Ability(const char *name, const char *description):
        name_(name), description_(description){}
    const char *name() const noexcept { return this->name_; }
    const char *description() const noexcept { return this->description_; }
private:
    const char *name_;
    const char *description_;
};

extern std::unordered_map<std::string, Ability> ability_from_string;

#endif // ABILITY_H