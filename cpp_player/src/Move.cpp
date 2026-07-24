#include "Move.h"
#include "Pokemon.h"
#include "Game.h"
#include "Status.h"
#include "Utils.h"
#include "gen/MoveGen.h"
#include <cmath>
#include <cstdlib>
#include <memory>

std::int32_t Move::defending_stat(const Pokemon &target, bool critical) const noexcept {
    return this->move_info_.category() == MoveCategory::PHYSICAL ? target.defense(critical) : target.sp_def(critical);
}

std::int32_t Move::get_damage(const Pokemon &user, const Pokemon &target, bool critical) const noexcept {
    if(this->move_info_.category() == MoveCategory::NON_DAMAGING) return 0;
    const std::int32_t level_part = std::floor(((2.0 * user.level()) / 5.0) + 2.0);
    const double attacking_stat = this->move_info_.category() == MoveCategory::PHYSICAL ? user.attack() : user.sp_atk();
    constexpr std::int32_t min_damage = 2;
    const std::int32_t base_damage = std::floor((level_part * this->power(user, target) * attacking_stat / this->defending_stat(target, critical)) / 50.0) + min_damage;
    const double stab = user.types().first == this->move_info_.type() || user.types().second == this->move_info_.type() ? 1.5 : 1.0;
    const double rng = 0.85 + (double)rand() / RAND_MAX * (1.00 - 0.85);
    return (double)base_damage * stab * rng;
}

const MoveBase &switch_move_base() noexcept {
    static const MoveBase base("Switch", "Switch to another Pokemon.", MoveCategory::NON_DAMAGING, 1, 0, 0, SWITCH_ACTION_PRIORITY, MoveTarget::SELF, Type::NONE, {});
    return base;
}
void Move_Switch::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user_player.switch_to(this->switch_target_);
}

MoveUseResult Move::use(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player, bool ignore_status) noexcept {
    if(!user.alive()) return MoveUseResult::FAILED;
    const bool skip_status_force_sleep_talk = ignore_status || dynamic_cast<const Move_SleepTalk*>(this) != nullptr || dynamic_cast<const Move_Switch*>(this) != nullptr;
    if(!skip_status_force_sleep_talk && user.status_blocks_move()) return MoveUseResult::FAILED;
    this->pp_--;
    if(this->move_info_.accuracy()) {
        if(!chance_of(this->move_info_.accuracy())) return MoveUseResult::MISSED;
    }
    constexpr std::int32_t CRITICAL_HIT_CHANCE_PERCENT = 4;
    const bool critical = chance_of(CRITICAL_HIT_CHANCE_PERCENT);
    bool damaging = false;
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
            const std::int32_t damage = get_damage(user, target, critical);
            damaging = true;
            target.take_damage(damage);
            break;
        }
        case SCRIPTED: break;
    }
    this->effect(user, target, user_player, target_player);
    return damaging && critical ? MoveUseResult::CRITICAL : MoveUseResult::SUCCESSFUL;
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
std::int32_t Move_Psyshock::defending_stat(const Pokemon &target, bool critical) const noexcept {
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
    if(chance_of(10)) target.try_update_status(std::make_unique<Status_Freeze>());
}
void Move_FireBlast::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(10)) target.try_update_status(std::make_unique<Status_Burn>());
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
    if(chance_of(30)) target.try_update_status(std::make_unique<Status_Burn>());
}
void Move_Toxic::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_BadlyPoisioned>());
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
    user.try_update_status(std::make_unique<Status_Sleep>());
    user.heal_percent(1.0);
}
void Move_Scald::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(chance_of(30)) target.try_update_status(std::make_unique<Status_Burn>());
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
void Move_WillOWisp::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Burn>());
}

void Move_Acupressure::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    switch(std::rand() % 5) {
        case 0: user.boost_stats(2, 0, 0, 0, 0); break;
        case 1: user.boost_stats(0, 2, 0, 0, 0); break;
        case 2: user.boost_stats(0, 0, 2, 0, 0); break;
        case 3: user.boost_stats(0, 0, 0, 2, 0); break;
        case 4: user.boost_stats(0, 0, 0, 0, 2); break;
    }
}
void Move_Agility::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 0, 0, 2);
}
void Move_Amnesia::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 0, 2, 0);
}
void Move_Autotomize::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 0, 0, 2);
}
void Move_Barrier::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 2, 0, 0, 0);
}
void Move_BulkUp::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 1, 0, 0, 0);
}
void Move_Coil::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 1, 0, 0, 0);
}
void Move_CosmicPower::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 1, 0, 1, 0);
}
void Move_CottonGuard::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 3, 0, 0, 0);
}
void Move_DefenseCurl::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 1, 0, 0, 0);
}
void Move_DefendOrder::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 1, 0, 1, 0);
}
void Move_DragonDance::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 0, 0, 1);
}
void Move_Growth::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 1, 0, 0);
}
void Move_Harden::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 1, 0, 0, 0);
}
void Move_HoneClaws::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 0, 0, 0);
}
void Move_Howl::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 0, 0, 0);
}
void Move_IronDefense::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 2, 0, 0, 0);
}
void Move_Meditate::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 0, 0, 0);
}
void Move_NastyPlot::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 2, 0, 0);
}
void Move_QuiverDance::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 1, 1, 1);
}
void Move_RockPolish::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 0, 0, 2);
}
void Move_Sharpen::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 0, 0, 0);
}
void Move_ShellSmash::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(2, -1, 2, -1, 2);
}
void Move_ShiftGear::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 0, 0, 2);
}
void Move_Stockpile::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 1, 0, 1, 0);
}
void Move_TailGlow::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 0, 3, 0, 0);
}
void Move_Withdraw::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(0, 1, 0, 0, 0);
}
void Move_WorkUp::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(1, 0, 1, 0, 0);
}

void Move_Charm::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(-2, 0, 0, 0, 0);
}
void Move_CottonSpore::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, 0, 0, 0, -2);
}
void Move_FakeTears::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, 0, 0, -2, 0);
}
void Move_FeatherDance::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(-2, 0, 0, 0, 0);
}
void Move_Growl::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(-1, 0, 0, 0, 0);
}
void Move_Leer::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, -1, 0, 0, 0);
}
void Move_MetalSound::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, 0, 0, -2, 0);
}
void Move_ScaryFace::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, 0, 0, 0, -2);
}
void Move_Screech::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, -2, 0, 0, 0);
}
void Move_StringShot::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, 0, 0, 0, -1);
}
void Move_TailWhip::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(0, -1, 0, 0, 0);
}
void Move_Tickle::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(-1, -1, 0, 0, 0);
}

void Move_Haze::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.clear_stat_plus();
    target.clear_stat_plus();
}
void Move_PsychUp::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.boost_stats(
        target.atk_stage() - user.atk_stage(),
        target.def_stage() - user.def_stage(),
        target.sp_atk_stage() - user.sp_atk_stage(),
        target.sp_def_stage() - user.sp_def_stage(),
        target.speed_stage() - user.speed_stage()
    );
}

void Move_MilkDrink::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_Moonlight::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_MorningSun::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_SlackOff::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_SoftBoiled::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_Synthesis::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_HealOrder::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.heal_percent(0.5);
}
void Move_HealPulse::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.heal_percent(0.5);
}
void Move_BellyDrum::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    user.take_damage_percent(0.5);
    if(user.alive()) user.boost_stats(6, 0, 0, 0, 0);
}
void Move_PainSplit::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    const std::int32_t average_hp = (user.current_hp() + target.current_hp()) / 2;
    if(average_hp > user.current_hp()) user.heal(average_hp - user.current_hp());
    else user.take_damage(user.current_hp() - average_hp);
    if(average_hp > target.current_hp()) target.heal(average_hp - target.current_hp());
    else target.take_damage(target.current_hp() - average_hp);
}
void Move_Memento::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.boost_stats(-2, 0, -2, 0, 0);
    user.take_damage_percent(1.0);
}

void Move_Glare::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Paralysis>());
}
void Move_PoisonPowder::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Poison>());
}
void Move_PoisonGas::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Poison>());
}
void Move_StunSpore::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Paralysis>());
}
void Move_ThunderWave::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Paralysis>());
}
void Move_GrassWhistle::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Sleep>());
}
void Move_Sing::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Sleep>());
}
void Move_Hypnosis::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Sleep>());
}
void Move_LovelyKiss::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Sleep>());
}
void Move_SleepPowder::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Sleep>());
}
void Move_DarkVoid::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    target.try_update_status(std::make_unique<Status_Sleep>());
}
void Move_PsychoShift::effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {
    if(target.status_condition() != NonVolitileStatusCondition::NONE) return;
    switch(user.status_condition()) {
        case NonVolitileStatusCondition::BURN: target.try_update_status(std::make_unique<Status_Burn>()); break;
        case NonVolitileStatusCondition::FREEZE: target.try_update_status(std::make_unique<Status_Freeze>()); break;
        case NonVolitileStatusCondition::PARALYSIS: target.try_update_status(std::make_unique<Status_Paralysis>()); break;
        case NonVolitileStatusCondition::POISON: target.try_update_status(std::make_unique<Status_Poison>()); break;
        case NonVolitileStatusCondition::BADLY_POISONED: target.try_update_status(std::make_unique<Status_BadlyPoisioned>()); break;
        case NonVolitileStatusCondition::SLEEP: target.try_update_status(std::make_unique<Status_Sleep>()); break;
        default: return;
    }
    user.cure_status();
}
