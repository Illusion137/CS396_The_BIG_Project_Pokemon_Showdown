#include "Type.h"

double type_on_type_multiplier(const Type attacker_type, const Type defender_type) {
    switch(attacker_type) {
        case Type::BUG: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 2.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 0.5;
                case Type::FIRE: return 0.5;
                case Type::FLYING: return 0.5;
                case Type::GHOST: return 0.5;
                case Type::GRASS: return 2.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 0.5;
                case Type::PSYCHIC: return 2.0;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::DARK: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 0.5;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 0.5;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 2.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 2.0;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::DRAGON: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 2.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::ELECTRIC: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 0.5;
                case Type::ELECTRIC: return 0.5;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 2.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 0.5;
                case Type::GROUND: return 0.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 1.0;
                case Type::WATER: return 2.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::FIGHTING: {
            switch(defender_type) {
                case Type::BUG: return 0.5;
                case Type::DARK: return 2.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 0.5;
                case Type::GHOST: return 0.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 2.0;
                case Type::NORMAL: return 2.0;
                case Type::POISON: return 0.5;
                case Type::PSYCHIC: return 0.5;
                case Type::ROCK: return 2.0;
                case Type::STEEL: return 2.0;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::FIRE: {
            switch(defender_type) {
                case Type::BUG: return 2.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 0.5;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 0.5;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 2.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 2.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 0.5;
                case Type::STEEL: return 2.0;
                case Type::WATER: return 0.5;
                case Type::NONE: return 1.0;
            }
        }
        case Type::FLYING: {
            switch(defender_type) {
                case Type::BUG: return 2.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 0.5;
                case Type::FIGHTING: return 2.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 2.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 0.5;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::GHOST: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 0.5;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 2.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 0.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 2.0;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::GRASS: {
            switch(defender_type) {
                case Type::BUG: return 0.5;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 0.5;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 0.5;
                case Type::FLYING: return 0.5;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 0.5;
                case Type::GROUND: return 2.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 0.5;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 2.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 2.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::GROUND: {
            switch(defender_type) {
                case Type::BUG: return 0.5;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 2.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 2.0;
                case Type::FLYING: return 0.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 0.5;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 2.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 2.0;
                case Type::STEEL: return 2.0;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::ICE: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 2.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 0.5;
                case Type::FLYING: return 2.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 2.0;
                case Type::GROUND: return 2.0;
                case Type::ICE: return 0.5;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 0.5;
                case Type::NONE: return 1.0;
            }
        }
        case Type::NORMAL: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 0.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 0.5;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::POISON: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 0.5;
                case Type::GRASS: return 2.0;
                case Type::GROUND: return 0.5;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 0.5;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 0.5;
                case Type::STEEL: return 0.0;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::PSYCHIC: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 0.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 2.0;
                case Type::FIRE: return 1.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 2.0;
                case Type::PSYCHIC: return 0.5;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::ROCK: {
            switch(defender_type) {
                case Type::BUG: return 2.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 0.5;
                case Type::FIRE: return 2.0;
                case Type::FLYING: return 2.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 0.5;
                case Type::ICE: return 2.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 1.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 1.0;
                case Type::NONE: return 1.0;
            }
        }
        case Type::STEEL: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 1.0;
                case Type::ELECTRIC: return 0.5;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 0.5;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 1.0;
                case Type::GROUND: return 1.0;
                case Type::ICE: return 2.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 2.0;
                case Type::STEEL: return 0.5;
                case Type::WATER: return 0.5;
                case Type::NONE: return 1.0;
            }
        }
        case Type::WATER: {
            switch(defender_type) {
                case Type::BUG: return 1.0;
                case Type::DARK: return 1.0;
                case Type::DRAGON: return 0.5;
                case Type::ELECTRIC: return 1.0;
                case Type::FIGHTING: return 1.0;
                case Type::FIRE: return 2.0;
                case Type::FLYING: return 1.0;
                case Type::GHOST: return 1.0;
                case Type::GRASS: return 0.5;
                case Type::GROUND: return 2.0;
                case Type::ICE: return 1.0;
                case Type::NORMAL: return 1.0;
                case Type::POISON: return 1.0;
                case Type::PSYCHIC: return 1.0;
                case Type::ROCK: return 2.0;
                case Type::STEEL: return 1.0;
                case Type::WATER: return 0.5;
                case Type::NONE: return 1.0;
            }
        }
        case Type::NONE: return 1.0;
    }
    return 1.0;
}

double type_on_types_multiplier(const Type attacker_type, const std::pair<Type, Type> defender_types) {
    return type_on_type_multiplier(attacker_type, defender_types.first) * type_on_type_multiplier(attacker_type, defender_types.second);
}

const char *type_to_string(Type type) noexcept {
    switch(type) {
        case Type::NONE: return "None";
        case Type::BUG: return "Bug";
        case Type::DARK: return "Dark";
        case Type::DRAGON: return "Dragon";
        case Type::ELECTRIC: return "Electric";
        case Type::FIGHTING: return "Fighting";
        case Type::FIRE: return "Fire";
        case Type::FLYING: return "Flying";
        case Type::GHOST: return "Ghost";
        case Type::GRASS: return "Grass";
        case Type::GROUND: return "Ground";
        case Type::ICE: return "Ice";
        case Type::NORMAL: return "Normal";
        case Type::POISON: return "Poison";
        case Type::PSYCHIC: return "Psychic";
        case Type::ROCK: return "Rock";
        case Type::STEEL: return "Steel";
        case Type::WATER: return "Water";
    }
    return "None";
}