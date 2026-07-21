#ifndef POKEMON_H
#define POKEMON_H

#include "Type.h"
#include <cstdint>
#include <memory>
#include <string>

class BasePokemon {
    const char *get_name() { return this->name_; }
    const char *get_nickname() { return this->nickname_; }
    Type get_base_type() { return this->base_type_; }
    std::uint32_t get_hp() { return this->hp_; }
    std::uint32_t get_attack() { return this->attack_; }
    std::uint32_t get_defense() { return this->defense_; }
    std::uint32_t get_sp_atk() { return this->sp_atk_; }
    std::uint32_t get_sp_def() { return this->sp_def_; }
    std::uint32_t get_speed() { return this->speed_; }
private:
    const char *name_;
    const char *nickname_;
    Type base_type_;
    std::uint32_t hp_;
    std::uint32_t attack_;
    std::uint32_t defense_;
    std::uint32_t sp_atk_;
    std::uint32_t sp_def_;
    std::uint32_t speed_;
};

class Pokemon {
public:
    std::shared_ptr<BasePokemon> get_base_details() const noexcept { return this->base_pokemon_; }
private:
    std::shared_ptr<BasePokemon> base_pokemon_;
};

#endif // POKEMON_H