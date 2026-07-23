#include "Move.h"
#include "Pokemon.h"
#include "Game.h"
#include "Status.h"
#include "Utils.h"
#include "gen/MoveGen.h"
#include <cmath>
#include <cstdlib>
#include <memory>

std::int32_t Move::defending_stat(const Pokemon &target) const noexcept {
    return this->move_info_.category() == MoveCategory::PHYSICAL ? target.defense() : target.sp_def();
}

std::int32_t Move::get_damage(const Pokemon &user, const Pokemon &target) const noexcept {
    if(this->move_info_.category() == MoveCategory::NON_DAMAGING) return 0;
    const std::int32_t level_part = std::floor(((2.0 * user.level()) / 5.0) + 2.0);
    const std::int32_t power_part = this->move_info_.category() == MoveCategory::PHYSICAL ?
        std::floor((this->power(user, target) * (user.attack() / (double)this->defending_stat(target))) / 50.0) :
        std::floor((this->power(user, target) * (user.sp_atk() / (double)this->defending_stat(target))) / 50.0);
    constexpr std::int32_t min_damage = 2;
    const std::int32_t base_damage = std::floor(level_part * power_part) + min_damage;
    const double stab = user.types().first == this->move_info_.type() || user.types().second == this->move_info_.type() ? 1.5 : 1.0;
    const double rng = 0.85 + (double)rand() / RAND_MAX * (1.00 - 0.85);
    return (double)base_damage * stab * rng;
}

MoveUseResult Move::use(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player, bool ignore_status) noexcept {
    if(!user.alive()) return MoveUseResult::FAILED;
    const bool skip_status_force_sleep_talk = ignore_status || dynamic_cast<const Move_SleepTalk*>(this) != nullptr;
    if(!skip_status_force_sleep_talk && user.status_blocks_move()) return MoveUseResult::FAILED;
    this->pp_--;
    if(this->move_info_.accuracy()) {
        if(!chance_of(this->move_info_.accuracy())) return MoveUseResult::MISSED;
    }
    switch(this->move_info_.target()) {
        case ADJACENT_ALLY_OR_SELF:
        case ADJACENT_ALLY:
        case ALLY_TEAM:
        case ALLY_SIDE:
        case FOE_SIDE:
        case SELF: {
            break;
        }
        case ALL_ADJACENT_FOES:
        case RANDOM_NORMAL:
        case ALL_ADJACENT:
        case ADJACENT_FOE:
        case ANY:
        case ALL:
        case NORMAL: {
            const double multiplier = type_on_types_multiplier(this->move_info_.type(), target.types());
            if(multiplier == 0.0) return MoveUseResult::IMMUNE;
            const std::int32_t damage = get_damage(user, target);
            target.take_damage(damage);
            break;
        }
        case SCRIPTED: break;
    }
    this->effect(user, target, user_player, target_player);
    return MoveUseResult::SUCCESSFUL;
}


std::int32_t weight_based_power(double weight_kg) noexcept {
    if(weight_kg < 10.0) return 20;
    if(weight_kg < 25.0) return 40;
    if(weight_kg < 50.0) return 60;
    if(weight_kg < 100.0) return 80;
    if(weight_kg < 200.0) return 100;
    return 120;
}

void Move_AcidArmor::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 2, 0, 0, 0);
}

std::int32_t Move_LowKick::power(const Pokemon &user, const Pokemon &target) const noexcept {
    return weight_based_power(target.weight());
}
std::int32_t Move_GrassKnot::power(const Pokemon &user, const Pokemon &target) const noexcept {
    return weight_based_power(target.weight());
}
std::int32_t Move_Pursuit::power(const Pokemon &user, const Pokemon &target) const noexcept {
    const std::int32_t base_power = this->info().power();
    return target.has_volatile_status(VolitileStatusCondition::SWITCHING_OUT) ? base_power * 2 : base_power;
}
std::int32_t Move_Pursuit::effective_priority(const Pokemon &user, const Pokemon &target) const noexcept {
    return target.has_volatile_status(VolitileStatusCondition::SWITCHING_OUT) ? SWITCH_ACTION_PRIORITY + 1 : this->info().priority();
}
std::int32_t Move_Psyshock::defending_stat(const Pokemon &target) const noexcept {
    return target.defense();
}

void Move_Crunch::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(20)) target.boost_stats(0, -1, 0, 0, 0);
}
void Move_IronHead::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(30)) target.add_volatile_status(std::make_unique<Status_Flinch>());
}
void Move_SwordsDance::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(2, 0, 0, 0, 0);
}
void Move_RapidSpin::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user_player.side().stealth_rock = false;
    user_player.side().spikes = 0;
}
void Move_Outrage::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(user.consume_lock_turn(2, 3) == 0) user.add_volatile_status(std::make_unique<Status_Confusion>());
}
void Move_UTurn::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(user.alive()) user_player.request_switch();
}
void Move_Recover::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_IceBeam::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(10)) target.try_update_status(std::make_unique<NonVolitileStatus>(Status_Freeze()));
}
void Move_FireBlast::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(10)) target.try_update_status(std::make_unique<NonVolitileStatus>(Status_Burn()));
}
void Move_DracoMeteor::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, -2, 0, 0);
}
void Move_CalmMind::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 1, 1, 0);
}
void Move_Roost::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
    user.add_volatile_status(std::make_unique<Status_Roost>());
}
void Move_StealthRock::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target_player.side().stealth_rock = true;
}
void Move_Roar::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target_player.force_random_switch();
}
void Move_LavaPlume::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(30)) target.try_update_status(std::make_unique<NonVolitileStatus>(Status_Burn()));
}
void Move_Toxic::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<NonVolitileStatus>(Status_BadlyPoisioned()));
}
void Move_Taunt::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.add_volatile_status(std::make_unique<Status_Taunt>(3));
}
void Move_Hurricane::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(30)) target.add_volatile_status(std::make_unique<Status_Confusion>());
}
void Move_Superpower::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(-1, -1, 0, 0, 0);
}
void Move_Rest::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.cure_status();
    user.try_update_status(std::make_unique<NonVolitileStatus>(Status_Sleep()));
    user.heal_percent(1.0);
}
void Move_Scald::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(30)) target.try_update_status(std::make_unique<NonVolitileStatus>(Status_Burn()));
}
void Move_SleepTalk::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(user.status().get_condition() == NonVolitileStatusCondition::SLEEP) {
        user.use_random_move_for_sleep_talk(target, user_player, target_player);
    }
}
void Move_Spikes::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target_player.side().spikes = std::min(target_player.side().spikes + 1, 3);
}
void Move_DarkPulse::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(20)) target.add_volatile_status(std::make_unique<Status_Flinch>());
}
void Move_FocusBlast::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(10)) target.boost_stats(0, 0, 0, -1, 0);
}
void Move_Struggle::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.take_damage_percent(0.25);
}
