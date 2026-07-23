#ifndef MOVE_H
#define MOVE_H

#include "Type.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Pokemon;
class Player;

enum class MoveFlags {
    BYPASSES_SUBSTITUTE,
    CHARGE,
    CONTACT,
    DEFROST,
    DISTANCE,
    FAIL_COPYCAT,
    FAIL_ENCORE,
    FAIL_ME_FIRST,
    FAIL_MIMIC,
    FAIL_SKETCH,
    FUTURE_MOVE,
    GRAVITY,
    HEAL,
    METRONOME,
    MIRROR,
    MUST_PRESSURE,
    NO_ASSIST,
    NO_SLEEP_TALK,
    PLEDGE_COMBO,
    PROTECT,
    PUNCH,
    RECHARGE,
    REFLECTABLE,
    SNATCH,
    SOUND
};

enum MoveCategory {
    NON_DAMAGING,
    PHYSICAL,
    SPECIAL
};

enum MoveTarget {
    SELF,
    ADJACENT_ALLY_OR_SELF,
    ANY,
    NORMAL,
    ALLY_TEAM,
    ALL_ADJACENT_FOES,
    ALL,
    ADJACENT_ALLY,
    ALLY_SIDE,
    ADJACENT_FOE,
    FOE_SIDE,
    RANDOM_NORMAL,
    ALL_ADJACENT,
    SCRIPTED
};

class MoveBase {
public:
    MoveBase(const char *name, const char *description, MoveCategory category, std::int32_t pp, std::int32_t power, std::int32_t accuracy, std::int32_t priority, MoveTarget target, Type type, std::vector<MoveFlags> flags):
        name_(name), description_(description), category_(category), pp_(pp), power_(power), accuracy_(accuracy), priority_(priority), target_(target), type_(type), flags_(flags) {}

    const char *name() const noexcept { return this->name_; }
    const char *description() const noexcept { return this->description_; }
    MoveCategory category() const noexcept { return this->category_; }
    std::int32_t pp() const noexcept { return this->pp_; }
    std::int32_t power() const noexcept { return this->power_; }
    std::int32_t accuracy() const noexcept { return this->accuracy_; }
    std::int32_t priority() const noexcept { return this->priority_; }
    MoveTarget target() const noexcept { return this->target_; }
    Type type() const noexcept { return this->type_; }
    const std::vector<MoveFlags> &flags() const noexcept { return this->flags_; }
private:
    const char *name_;
    const char *description_;
    MoveCategory category_;
    std::int32_t pp_;
    std::int32_t power_;
    std::int32_t accuracy_;
    std::int32_t priority_;
    MoveTarget target_;
    Type type_;
    std::vector<MoveFlags> flags_;
};

extern std::unordered_map<std::string, MoveBase> name_to_move_map;

constexpr std::int32_t SWITCH_ACTION_PRIORITY = 98;

enum class MoveUseResult {
    SUCCESSFUL,
    IMMUNE,
    MISSED,
    FAILED
};

class Move {
public:
    explicit Move(const MoveBase &info) noexcept: move_info_(info), pp_(info.pp()) {}
    virtual ~Move() = default;
    const MoveBase &info() const noexcept { return this->move_info_; }
    std::int32_t pp() const noexcept { return this->pp_; }
    std::int32_t get_damage(const Pokemon &user, const Pokemon &target) const noexcept;
    std::int32_t priority(const Pokemon &user, const Pokemon &target) const noexcept { return this->effective_priority(user, target); }
    MoveUseResult use(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player, bool ignore_status = false) noexcept;
protected:
    virtual void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) {}
    virtual std::int32_t power(const Pokemon &user, const Pokemon &target) const noexcept { return this->move_info_.power(); }
    virtual std::int32_t defending_stat(const Pokemon &target) const noexcept;
    virtual std::int32_t effective_priority(const Pokemon &user, const Pokemon &target) const noexcept { return this->move_info_.priority(); }
    const MoveBase &move_info_;
    std::int32_t pp_;
};

extern const MoveBase &switch_move_base() noexcept;

class Move_Switch : public Move {
public:
    explicit Move_Switch(Pokemon *switch_target) noexcept: Move(switch_move_base()), switch_target_(switch_target) {}
    Pokemon *target() const noexcept { return this->switch_target_; }
protected:
    void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override;
private:
    Pokemon *switch_target_;
};

#endif // MOVE_H