#include "Status.h"
#include "Pokemon.h"
#include "Utils.h"

void Status_Burn::post_turn_effect(Pokemon &affected) {
    affected.take_damage_percent(1.0 / 8.0);
}
void Status_Freeze::post_turn_effect(Pokemon &affected) {}
bool Status_Freeze::blocks_move(Pokemon &affected) {
    if(chance_of(20)) {
        affected.cure_status();
        return false;
    }
    return true;
}
void Status_Paralysis::post_turn_effect(Pokemon &affected) {}
bool Status_Paralysis::blocks_move(Pokemon &affected) {
    return chance_of(25);
}
void Status_Poison::post_turn_effect(Pokemon &affected) {
    affected.take_damage_percent(1.0 / 8.0);
}
void Status_BadlyPoisioned::post_turn_effect(Pokemon &affected) {
    affected.take_damage_percent((1.0 * ++active_turns_) / 16.0);
}
void Status_Sleep::post_turn_effect(Pokemon &affected) {}
bool Status_Sleep::blocks_move(Pokemon &affected) {
    if(--this->turns_remaining_ <= 0) {
        affected.cure_status();
        return false;
    }
    return true;
}

const char *status_condition_abbreviation(NonVolitileStatusCondition condition) noexcept {
    switch(condition) {
        case NonVolitileStatusCondition::NONE: return "";
        case NonVolitileStatusCondition::BURN: return "BRN";
        case NonVolitileStatusCondition::FREEZE: return "FRZ";
        case NonVolitileStatusCondition::PARALYSIS: return "PAR";
        case NonVolitileStatusCondition::POISON: return "PSN";
        case NonVolitileStatusCondition::BADLY_POISONED: return "TOX";
        case NonVolitileStatusCondition::SLEEP: return "SLP";
        default: return "";
    }
}