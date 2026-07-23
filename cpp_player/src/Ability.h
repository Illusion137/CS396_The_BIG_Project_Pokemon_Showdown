#ifndef ABILITY_H
#define ABILITY_H

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

#endif // ABILITY_H