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
            }
        }
    }
    return 1.0;
}
