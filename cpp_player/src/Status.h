#ifndef STATUS_H
#define STATUS_H

#include <cstdint>
#include <cstdlib>

class Pokemon;

enum class NonVolitileStatusCondition {
    NONE,
    BURN,
    FREEZE,
    PARALYSIS,
    POISON,
    BADLY_POISONED,
    SLEEP
};

class NonVolitileStatus {
public:
    NonVolitileStatus(NonVolitileStatusCondition condition): condition_(condition) {}
    virtual ~NonVolitileStatus() = default;
    virtual void post_turn_effect(Pokemon &affected) {};
    virtual bool blocks_move(Pokemon &affected) { return false; }
    NonVolitileStatusCondition get_condition() const noexcept { return this->condition_; }
protected:
    const NonVolitileStatusCondition condition_;
    std::int32_t active_turns_ = 0;
};

class Status_Burn : public NonVolitileStatus {
public:
    Status_Burn(): NonVolitileStatus(NonVolitileStatusCondition::BURN) {}
    void post_turn_effect(Pokemon &affected) override;
};
class Status_Freeze : public NonVolitileStatus {
public:
    Status_Freeze(): NonVolitileStatus(NonVolitileStatusCondition::FREEZE) {}
    void post_turn_effect(Pokemon &affected) override;
    bool blocks_move(Pokemon &affected) override;
};
class Status_Paralysis : public NonVolitileStatus {
public:
    Status_Paralysis(): NonVolitileStatus(NonVolitileStatusCondition::PARALYSIS) {}
    void post_turn_effect(Pokemon &affected) override;
    bool blocks_move(Pokemon &affected) override;
};
class Status_Poison : public NonVolitileStatus {
public:
    Status_Poison(): NonVolitileStatus(NonVolitileStatusCondition::POISON) {}
    void post_turn_effect(Pokemon &affected) override;
};
class Status_BadlyPoisioned : public NonVolitileStatus {
public:
    Status_BadlyPoisioned(): NonVolitileStatus(NonVolitileStatusCondition::BADLY_POISONED) {}
    void post_turn_effect(Pokemon &affected) override;
};
class Status_Sleep : public NonVolitileStatus {
public:
    Status_Sleep() noexcept : NonVolitileStatus(NonVolitileStatusCondition::SLEEP), turns_remaining_(1 + std::rand() % 3) {}
    void post_turn_effect(Pokemon &affected) override;
    bool blocks_move(Pokemon &affected) override;
private:
    std::int32_t turns_remaining_;
};

enum class VolitileStatusCondition {
    CONFUSION,
    FLINCH,
    TAUNT,
    ROOST,
    LOCKED_MOVE,
    SWITCHING_OUT
};

class VolitileStatus {
public:
    virtual ~VolitileStatus() = default;
    virtual VolitileStatusCondition condition() const noexcept = 0;
};

class TurnStatus : public VolitileStatus {
public:
    TurnStatus(std::int32_t turns): turns_remaining_(turns) {}
    std::int32_t turns_remaining() const noexcept { return this->turns_remaining_; }
    bool tick() noexcept { return --this->turns_remaining_ <= 0; }
private:
    std::int32_t turns_remaining_;
};

class Status_Confusion : public VolitileStatus {
public:
    VolitileStatusCondition condition() const noexcept override { return VolitileStatusCondition::CONFUSION; }
};
class Status_Flinch : public VolitileStatus {
public:
    VolitileStatusCondition condition() const noexcept override { return VolitileStatusCondition::FLINCH; }
};
class Status_Roost : public VolitileStatus {
public:
    VolitileStatusCondition condition() const noexcept override { return VolitileStatusCondition::ROOST; }
};
class Status_SwitchingOut : public VolitileStatus {
public:
    VolitileStatusCondition condition() const noexcept override { return VolitileStatusCondition::SWITCHING_OUT; }
};
class Status_Taunt : public TurnStatus {
public:
    Status_Taunt(std::int32_t turns): TurnStatus(turns) {}
    VolitileStatusCondition condition() const noexcept override { return VolitileStatusCondition::TAUNT; }
};
class Status_LockedMove : public TurnStatus {
public:
    Status_LockedMove(std::int32_t turns): TurnStatus(turns) {}
    VolitileStatusCondition condition() const noexcept override { return VolitileStatusCondition::LOCKED_MOVE; }
};

extern const char *status_condition_abbreviation(NonVolitileStatusCondition condition) noexcept;

#endif // STATUS_H