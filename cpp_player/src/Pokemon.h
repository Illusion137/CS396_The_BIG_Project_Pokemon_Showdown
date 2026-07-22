#ifndef POKEMON_H
#define POKEMON_H

#include "Ability.h"
#include "Item.h"
#include "Move.h"
#include "Nature.h"
#include "Type.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

struct StatSpread {
    std::int32_t hp;
    std::int32_t attack;
    std::int32_t defense;
    std::int32_t sp_atk;
    std::int32_t sp_def;
    std::int32_t speed;
};

typedef std::pair<Type, Type> TypePair;

class BasePokemon {
public:
    BasePokemon(const char *name, TypePair base_type, StatSpread stats, double weight): name_(name), base_type_(base_type), stats_(stats), weight_(weight) {}

    const char *name() const noexcept { return this->name_; }
    TypePair base_type() const noexcept { return this->base_type_; }
    std::int32_t hp() const noexcept { return this->stats_.hp; }
    std::int32_t attack() const noexcept { return this->stats_.attack; }
    std::int32_t defense() const noexcept { return this->stats_.defense; }
    std::int32_t sp_atk() const noexcept { return this->stats_.sp_atk; }
    std::int32_t sp_def() const noexcept { return this->stats_.sp_def; }
    std::int32_t speed() const noexcept { return this->stats_.speed; }
private:
    const char *name_;
    TypePair base_type_;
    StatSpread stats_;
    double weight_;
};

extern std::unordered_map<std::string, BasePokemon> name_to_pokemon_map;

class Pokemon {
public:
    Pokemon(std::string pokemon_name) : 
        base_pokemon_(name_to_pokemon_map.at(pokemon_name))
        {}
    std::int32_t attack() const noexcept;
    std::int32_t defense() const noexcept;
    std::int32_t sp_atk() const noexcept;
    std::int32_t sp_def() const noexcept;
    std::int32_t speed() const noexcept;

    void clear_stat_plus();
    void apply_stat_plus(std::int32_t atk_plus, std::int32_t def_plus, std::int32_t sp_atk_plus, std::int32_t sp_def_plus, std::int32_t speed_plus);
    // std::shared_ptr<BasePokemon> base_details() const noexcept { return this->base_pokemon_; }
private:
    BasePokemon base_pokemon_;   
    StatSpread evs_;
    StatSpread ivs_;
    std::array<std::unique_ptr<Move>, 4> moves_;
    Nature nature_;
    Item item_;
    Ability ability_;

    std::int32_t current_hp_;

    std::int32_t atk_plus_;
    std::int32_t def_plus_;
    std::int32_t sp_atk_plus_;
    std::int32_t sp_def_plus_;
    std::int32_t speed_plus_;
};

#endif // POKEMON_H