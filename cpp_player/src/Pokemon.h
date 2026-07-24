#ifndef POKEMON_H
#define POKEMON_H

#include "Ability.h"
#include "Item.h"
#include "Move.h"
#include "Nature.h"
#include "Status.h"
#include "Type.h"
#include "gen/MoveGen.h"
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <nlohmann/json.hpp>
#include <vector>

struct StatSpread {
    StatSpread(std::int32_t _hp, std::int32_t _atk, std::int32_t _def, std::int32_t _spa, std::int32_t _spd, std::int32_t _spe):
        hp(_hp),
        attack(_atk),
        defense(_def),
        sp_atk(_spa),
        sp_def(_spd),
        speed(_spe) {}
    StatSpread(nlohmann::json j);
    
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
    double weight() const noexcept { return this->weight_; }
private:
    const char *name_;
    TypePair base_type_;
    StatSpread stats_;
    double weight_;
};

extern std::unordered_map<std::string, BasePokemon> name_to_pokemon_map;

class Player;

class Pokemon {
public:
    Pokemon(std::string pokemon_name, std::string item_name, std::string ability_name, std::string nature_name, nlohmann::json evs_json, nlohmann::json ivs_json, nlohmann::json moves, std::int32_t lvl) : 
        base_pokemon_(name_to_pokemon_map.at(pokemon_name)),
        item_(item_name == "" ? Item(ItemBase("", "")) : item_from_string.at(item_name)),
        ability_(ability_from_string.at(ability_name)),
        nature_(nature_from_string.at(nature_name)),
        evs_(evs_json),
        ivs_(ivs_json),
        level_(lvl)
        {
            this->current_hp_ = max_hp();
            for(std::size_t i = 0; i < moves.size(); i++) {
                this->moves_[i] = create_move(moves[i]);
            }
        }
    Pokemon(nlohmann::json pokemon_json): Pokemon(
        pokemon_json.at("name").get<std::string>(),
        pokemon_json.contains("item") ? pokemon_json.at("item").get<std::string>() : "",
        pokemon_json.at("ability").get<std::string>(),
        pokemon_json.at("nature").get<std::string>(),
        pokemon_json.at("evs"),
        pokemon_json.at("ivs"),
        pokemon_json.at("moves").get<std::vector<std::string>>(),
        pokemon_json.at("level").get<std::int32_t>()
    ) {}
    std::string name() const noexcept;
    std::int32_t max_hp() const noexcept;
    std::int32_t current_hp() const noexcept;
    bool alive() const noexcept;

    std::int32_t level() const noexcept;
    std::int32_t attack() const noexcept;
    std::int32_t defense(bool critical = false) const noexcept;
    std::int32_t sp_atk() const noexcept;
    std::int32_t sp_def(bool critical = false) const noexcept;
    std::int32_t speed() const noexcept;

    TypePair types() const noexcept;

    bool has_available_moves() const noexcept;
    std::unique_ptr<Move> random_move() noexcept;
    
    void take_damage(std::int32_t damage) noexcept;
    void take_damage_percent(double percent) noexcept;

    void clear_stat_plus();
    void boost_stats(std::int32_t atk_plus, std::int32_t def_plus, std::int32_t sp_atk_plus, std::int32_t sp_def_plus, std::int32_t speed_plus) noexcept;

    double weight() const noexcept;
    void heal(std::int32_t amount) noexcept;
    void heal_percent(double percent) noexcept;

    bool has_volatile_status(VolitileStatusCondition condition) const noexcept;
    VolitileStatus *get_volatile_status(VolitileStatusCondition condition) const noexcept;
    void add_volatile_status(std::unique_ptr<VolitileStatus> status) noexcept;
    void remove_volatile_status(VolitileStatusCondition condition) noexcept;
    void clear_volatile_statuses() noexcept;

    std::int32_t consume_lock_turn(std::int32_t min_turns, std::int32_t max_turns) noexcept;

    MoveUseResult use_random_move_for_sleep_talk(Pokemon &target, Player &user_player, Player &target_player);

    const std::array<std::unique_ptr<Move>, 4> &moves() const noexcept;
    std::int32_t moves_count() const noexcept;

    std::int32_t atk_stage() const noexcept;
    std::int32_t def_stage() const noexcept;
    std::int32_t sp_atk_stage() const noexcept;
    std::int32_t sp_def_stage() const noexcept;
    std::int32_t speed_stage() const noexcept;

    NonVolitileStatus &status() const noexcept;
    NonVolitileStatusCondition status_condition() const noexcept;
    void try_update_status(std::unique_ptr<NonVolitileStatus> new_status) noexcept;
    void cure_status() noexcept;
    bool status_blocks_move() noexcept;
    void apply_end_of_turn_status() noexcept;
private:
    BasePokemon base_pokemon_;   
    Item item_;
    Ability ability_;
    Nature nature_;
    StatSpread evs_;
    StatSpread ivs_;
    std::array<std::unique_ptr<Move>, 4> moves_ = std::array<std::unique_ptr<Move>, 4>{nullptr, nullptr, nullptr, nullptr};
    std::int32_t level_;

    std::int32_t current_hp_;
    std::unique_ptr<NonVolitileStatus> non_volitile_status_ = std::make_unique<NonVolitileStatus>(NonVolitileStatusCondition::NONE);

    std::int32_t atk_plus_ = 0;
    std::int32_t def_plus_ = 0;
    std::int32_t sp_atk_plus_ = 0;
    std::int32_t sp_def_plus_ = 0;
    std::int32_t speed_plus_ = 0;

    std::vector<std::unique_ptr<VolitileStatus>> volatile_statuses_;
};

#endif // POKEMON_H