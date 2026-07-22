#ifndef TYPE_H
#define TYPE_H

enum class Type {
    NONE,
    BUG,
    DARK,
    DRAGON,
    ELECTRIC,
    FIGHTING,
    FIRE,
    FLYING,
    GHOST,
    GRASS,
    GROUND,
    ICE,
    NORMAL,
    POISON,
    PSYCHIC,
    ROCK,
    STEEL,
    WATER
};
extern double type_on_type_multiplier(const Type attacker_type, const Type defender_type);

#endif // TYPE_H