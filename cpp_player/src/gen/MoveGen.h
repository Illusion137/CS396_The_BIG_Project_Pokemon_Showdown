// AUTO GENERATED DO NOT MODIFY
#ifndef MOVE_GEN_H
#define MOVE_GEN_H
#include "../Move.h"
#include <memory>
#include <string>

class Move_AcidArmor : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_LowKick : public Move { public: using Move::Move; std::int32_t power(const Pokemon &user, const Pokemon &target) const noexcept override; };
class Move_GrassKnot : public Move { public: using Move::Move; std::int32_t power(const Pokemon &user, const Pokemon &target) const noexcept override; };
class Move_Pursuit : public Move { public: using Move::Move; std::int32_t power(const Pokemon &user, const Pokemon &target) const noexcept override; std::int32_t effective_priority(const Pokemon &user, const Pokemon &target) const noexcept override; };
class Move_Psyshock : public Move { public: using Move::Move; std::int32_t defending_stat(const Pokemon &target) const noexcept override; };
class Move_Crunch : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_IronHead : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_SwordsDance : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_RapidSpin : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Outrage : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_UTurn : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Recover : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_IceBeam : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_FireBlast : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_DracoMeteor : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_CalmMind : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Roost : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_StealthRock : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Roar : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_LavaPlume : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Toxic : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Taunt : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Hurricane : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Superpower : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Rest : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Scald : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_SleepTalk : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Spikes : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_DarkPulse : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_FocusBlast : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };
class Move_Struggle : public Move { public: using Move::Move; void effect(Pokemon &user, Pokemon &target, Player &user_player, Player &target_player) override; };

extern std::unique_ptr<Move> create_move(const std::string &name);
#endif // MOVE_GEN_H
