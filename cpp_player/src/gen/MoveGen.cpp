// AUTO GENERATED DO NOT MODIFY
#include "MoveGen.h"

std::unique_ptr<Move> create_move(const std::string &name) {
    const MoveBase &info = name_to_move_map.at(name);
    if(name == "Acid Armor") return std::make_unique<Move_AcidArmor>(info);
    if(name == "Low Kick") return std::make_unique<Move_LowKick>(info);
    if(name == "Grass Knot") return std::make_unique<Move_GrassKnot>(info);
    if(name == "Pursuit") return std::make_unique<Move_Pursuit>(info);
    if(name == "Psyshock") return std::make_unique<Move_Psyshock>(info);
    if(name == "Crunch") return std::make_unique<Move_Crunch>(info);
    if(name == "Iron Head") return std::make_unique<Move_IronHead>(info);
    if(name == "Swords Dance") return std::make_unique<Move_SwordsDance>(info);
    if(name == "Rapid Spin") return std::make_unique<Move_RapidSpin>(info);
    if(name == "Outrage") return std::make_unique<Move_Outrage>(info);
    if(name == "U-turn") return std::make_unique<Move_UTurn>(info);
    if(name == "Recover") return std::make_unique<Move_Recover>(info);
    if(name == "Ice Beam") return std::make_unique<Move_IceBeam>(info);
    if(name == "Fire Blast") return std::make_unique<Move_FireBlast>(info);
    if(name == "Draco Meteor") return std::make_unique<Move_DracoMeteor>(info);
    if(name == "Calm Mind") return std::make_unique<Move_CalmMind>(info);
    if(name == "Roost") return std::make_unique<Move_Roost>(info);
    if(name == "Stealth Rock") return std::make_unique<Move_StealthRock>(info);
    if(name == "Roar") return std::make_unique<Move_Roar>(info);
    if(name == "Lava Plume") return std::make_unique<Move_LavaPlume>(info);
    if(name == "Toxic") return std::make_unique<Move_Toxic>(info);
    if(name == "Taunt") return std::make_unique<Move_Taunt>(info);
    if(name == "Hurricane") return std::make_unique<Move_Hurricane>(info);
    if(name == "Superpower") return std::make_unique<Move_Superpower>(info);
    if(name == "Rest") return std::make_unique<Move_Rest>(info);
    if(name == "Scald") return std::make_unique<Move_Scald>(info);
    if(name == "Sleep Talk") return std::make_unique<Move_SleepTalk>(info);
    if(name == "Spikes") return std::make_unique<Move_Spikes>(info);
    if(name == "Dark Pulse") return std::make_unique<Move_DarkPulse>(info);
    if(name == "Focus Blast") return std::make_unique<Move_FocusBlast>(info);
    return std::make_unique<Move>(info);
}
