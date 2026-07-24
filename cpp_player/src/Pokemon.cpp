#include "Pokemon.h"
#include "Utils.h"
#include "gen/MoveGen.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <vector>

StatSpread::StatSpread(nlohmann::json j) {
    this->hp = j.at("hp").get<std::int32_t>();
    this->attack = j.at("atk").get<std::int32_t>();
    this->defense = j.at("def").get<std::int32_t>();
    this->sp_atk = j.at("spa").get<std::int32_t>();
    this->sp_def = j.at("spd").get<std::int32_t>();
    this->speed = j.at("spe").get<std::int32_t>();
}

std::int32_t calculate_stat_plus(const std::int32_t stat, const std::int32_t stat_plus, bool critical = false) {
    if(critical) return stat;
    if(stat_plus > 0) return stat * (2.0 + stat_plus) / 2.0;
    if(stat_plus < 0) return stat * (2.0 / (2.0 - stat_plus));
    return stat;
}
std::int32_t calculate_stat_initial_pass(std::int32_t base_stat, std::int32_t stat_iv, std::int32_t stat_ev, std::int32_t level) {
    double top = (2 * base_stat + stat_iv + std::floor(0.25 * stat_ev)) * level;
    return std::floor(top / 100.0);
}
std::int32_t calculate_stat(std::int32_t base_stat, std::int32_t stat_iv, std::int32_t stat_ev, std::int32_t level, Nature nature, std::int32_t stat_mask) {
    const std::int32_t base = calculate_stat_initial_pass(base_stat, stat_iv, stat_ev, level) + 5;
    return std::floor(base * nature_stat_multiplier(nature, stat_mask));
}

std::string Pokemon::name() const noexcept { return this->base_pokemon_.name(); }
std::int32_t Pokemon::max_hp() const noexcept {
    return calculate_stat_initial_pass(this->base_pokemon_.hp(), this->ivs_.hp, this->evs_.hp, this->level()) + this->level() + 10;
}
std::int32_t Pokemon::current_hp() const noexcept { return this->current_hp_; }
bool Pokemon::alive() const noexcept { return this->current_hp_ > 0; }
std::int32_t Pokemon::level() const noexcept { return this->level_; }

std::int32_t Pokemon::attack() const noexcept {
    const std::int32_t stat = calculate_stat_plus(calculate_stat(this->base_pokemon_.attack(), this->ivs_.attack, this->evs_.attack, this->level(), this->nature_, NATURE_ATK_MASK), this->atk_plus_);
    if(this->status_condition() == NonVolitileStatusCondition::BURN) return stat * 0.5;
    return stat;
}
std::int32_t Pokemon::defense(bool critical) const noexcept {
    return calculate_stat_plus(calculate_stat(this->base_pokemon_.defense(), this->ivs_.defense, this->evs_.defense, this->level(), this->nature_, NATURE_DEF_MASK), this->def_plus_, critical);
}
std::int32_t Pokemon::sp_atk() const noexcept {
    return calculate_stat_plus(calculate_stat(this->base_pokemon_.sp_atk(), this->ivs_.sp_atk, this->evs_.sp_atk, this->level(), this->nature_, NATURE_SP_ATK_MASK), this->sp_atk_plus_);
}
std::int32_t Pokemon::sp_def(bool critical) const noexcept {
    return calculate_stat_plus(calculate_stat(this->base_pokemon_.sp_def(), this->ivs_.sp_def, this->evs_.sp_def, this->level(), this->nature_, NATURE_SP_DEF_MASK), this->sp_def_plus_, critical);
}
std::int32_t Pokemon::speed() const noexcept {
    const std::int32_t stat = calculate_stat_plus(calculate_stat(this->base_pokemon_.speed(), this->ivs_.speed, this->evs_.speed, this->level(), this->nature_, NATURE_SPEED_MASK), this->speed_plus_);
    if(this->status_condition() == NonVolitileStatusCondition::PARALYSIS) return stat * 0.5;
    return stat;
}

TypePair Pokemon::types() const noexcept {
    TypePair types = this->base_pokemon_.base_type();
    if(this->has_volatile_status(VolitileStatusCondition::ROOST)) {
        if(types.first == Type::FLYING) types.first = Type::NONE;
        if(types.second == Type::FLYING) types.second = Type::NONE;
    }
    return types;
}

bool Pokemon::has_available_moves() const noexcept {
    for(const auto &move : this->moves()) {
        if(!move) continue;
        if(move->pp()) return true;
    }
    return false;
}
std::unique_ptr<Move> Pokemon::random_move() noexcept {
    if(!this->has_available_moves()) return create_move("Struggle");
    std::vector<Move*> eligible_moves;
    for(const auto &move : this->moves()) {
        if(move && move->pp()) eligible_moves.push_back(move.get());
    }
    return create_move(eligible_moves[std::rand() % eligible_moves.size()]->info().name());
}

void Pokemon::take_damage(std::int32_t damage) noexcept {
    this->current_hp_ -= damage;
    this->current_hp_ = std::max(this->current_hp_, 0);
}
void Pokemon::take_damage_percent(double percent) noexcept {
    this->take_damage(percent * max_hp());
}


void Pokemon::clear_stat_plus() {
    this->atk_plus_ = 0;
    this->def_plus_ = 0;
    this->sp_atk_plus_ = 0;
    this->sp_def_plus_ = 0;
    this->speed_plus_ = 0;
}
void Pokemon::boost_stats(std::int32_t atk_plus, std::int32_t def_plus, std::int32_t sp_atk_plus, std::int32_t sp_def_plus, std::int32_t speed_plus) noexcept {
    this->atk_plus_ = std::min(std::max(this->atk_plus_ + atk_plus, -6), 6);
    this->def_plus_ = std::min(std::max(this->def_plus_ + def_plus, -6), 6);
    this->sp_atk_plus_ = std::min(std::max(this->sp_atk_plus_ + sp_atk_plus, -6), 6);
    this->sp_def_plus_ = std::min(std::max(this->sp_def_plus_ + sp_def_plus, -6), 6);
    this->speed_plus_ = std::min(std::max(this->speed_plus_ + speed_plus, -6), 6);
}

double Pokemon::weight() const noexcept {
    return this->base_pokemon_.weight();
}

void Pokemon::heal(std::int32_t amount) noexcept {
    this->current_hp_ = std::min(this->current_hp_ + std::max(amount, 0), this->max_hp());
}
void Pokemon::heal_percent(double percent) noexcept {
    this->heal(this->max_hp() * percent);
}

bool Pokemon::has_volatile_status(VolitileStatusCondition condition) const noexcept {
    return this->get_volatile_status(condition) != nullptr;
}
VolitileStatus *Pokemon::get_volatile_status(VolitileStatusCondition condition) const noexcept {
    for(const auto &status : this->volatile_statuses_) {
        if(status->condition() == condition) return status.get();
    }
    return nullptr;
}
void Pokemon::add_volatile_status(std::unique_ptr<VolitileStatus> status) noexcept {
    this->remove_volatile_status(status->condition());
    this->volatile_statuses_.push_back(std::move(status));
}
void Pokemon::remove_volatile_status(VolitileStatusCondition condition) noexcept {
    this->volatile_statuses_.erase(
        std::remove_if(this->volatile_statuses_.begin(), this->volatile_statuses_.end(), [condition](const auto &status) {
            return status->condition() == condition;
        }),
        this->volatile_statuses_.end()
    );
}
void Pokemon::clear_volatile_statuses() noexcept {
    this->volatile_statuses_.clear();
}

std::int32_t Pokemon::consume_lock_turn(std::int32_t min_turns, std::int32_t max_turns) noexcept {
    if(!this->has_volatile_status(VolitileStatusCondition::LOCKED_MOVE)) {
        const std::int32_t turns = min_turns + std::rand() % (max_turns - min_turns + 1);
        this->add_volatile_status(std::make_unique<Status_LockedMove>(turns));
    }
    TurnStatus *lock = static_cast<TurnStatus*>(this->get_volatile_status(VolitileStatusCondition::LOCKED_MOVE));
    const bool expired = lock->tick();
    if(expired) {
        this->remove_volatile_status(VolitileStatusCondition::LOCKED_MOVE);
        return 0;
    }
    return lock->turns_remaining();
}

bool Pokemon::consume_flinch() noexcept {
    if(!this->has_volatile_status(VolitileStatusCondition::FLINCH)) return false;
    this->remove_volatile_status(VolitileStatusCondition::FLINCH);
    return true;
}

bool Pokemon::tick_taunt() noexcept {
    VolitileStatus *taunt = this->get_volatile_status(VolitileStatusCondition::TAUNT);
    if(!taunt) return false;
    if(static_cast<TurnStatus*>(taunt)->tick()) {
        this->remove_volatile_status(VolitileStatusCondition::TAUNT);
        return false;
    }
    return true;
}

bool Pokemon::tick_confusion_and_check_self_hit() noexcept {
    VolitileStatus *confusion = this->get_volatile_status(VolitileStatusCondition::CONFUSION);
    if(!confusion) return false;
    if(static_cast<TurnStatus*>(confusion)->tick()) {
        this->remove_volatile_status(VolitileStatusCondition::CONFUSION);
        return false;
    }
    return chance_of(33);
}

MoveUseResult Pokemon::use_random_move_for_sleep_talk(Pokemon &target, Player &user_player, Player &target_player) {
    std::vector<Move*> eligible_moves;
    for(const auto &move : this->moves()) {
        if(!move) continue;
        const auto &flags = move->info().flags();
        if(std::find(flags.begin(), flags.end(), MoveFlags::NO_SLEEP_TALK) != flags.end()) continue;
        eligible_moves.push_back(move.get());
    }
    if(eligible_moves.empty()) return MoveUseResult::FAILED;
    Move *chosen = eligible_moves[std::rand() % eligible_moves.size()];
    return chosen->use(*this, target, user_player, target_player, true);
}

const std::array<std::unique_ptr<Move>, 4> &Pokemon::moves() const noexcept { return this->moves_; }
std::int32_t Pokemon::moves_count() const noexcept {
    std::int32_t count = 0;
    for(const auto &move : this->moves()) {
        if(move) count++;
    }
    return count;
}
std::int32_t Pokemon::atk_stage() const noexcept { return this->atk_plus_; }
std::int32_t Pokemon::def_stage() const noexcept { return this->def_plus_; }
std::int32_t Pokemon::sp_atk_stage() const noexcept { return this->sp_atk_plus_; }
std::int32_t Pokemon::sp_def_stage() const noexcept { return this->sp_def_plus_; }
std::int32_t Pokemon::speed_stage() const noexcept { return this->speed_plus_; }

NonVolitileStatus &Pokemon::status() const noexcept { return *this->non_volitile_status_; }
NonVolitileStatusCondition Pokemon::status_condition() const noexcept { return this->status().get_condition(); }
void Pokemon::try_update_status(std::unique_ptr<NonVolitileStatus> new_status) noexcept {
    if(this->status_condition() != NonVolitileStatusCondition::NONE) return;
    this->non_volitile_status_ = std::move(new_status);
}
void Pokemon::cure_status() noexcept { this->non_volitile_status_ = std::make_unique<NonVolitileStatus>(NonVolitileStatusCondition::NONE); }
bool Pokemon::status_blocks_move() noexcept { return this->non_volitile_status_->blocks_move(*this); }
void Pokemon::apply_end_of_turn_status() noexcept { this->non_volitile_status_->post_turn_effect(*this); }