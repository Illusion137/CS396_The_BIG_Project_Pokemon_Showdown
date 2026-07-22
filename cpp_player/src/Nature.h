#ifndef NATURE_H
#define NATURE_H

#include <cstdint>
#include <string>
#include <unordered_map>

enum class Nature {
    ADAMANT  = 0b10000'00100,
    BASHFUL  = 0b00100'00100,
    BOLD     = 0b01000'10000,
    BRAVE    = 0b10000'00001,
    CALM     = 0b00010'10000,
    CAREFUL  = 0b00010'00100,
    DOCILE   = 0b01000'01000,
    GENTLE   = 0b00010'01000,
    HARDY    = 0b10000'10000,
    HASTY    = 0b00001'01000,
    IMPISH   = 0b01000'00100,
    JOLLY    = 0b00001'00100,
    LAX      = 0b01000'00010,
    LONELY   = 0b10000'01000,
    MILD     = 0b00100'01000,
    MODEST   = 0b00100'10000,
    NAIVE    = 0b00001'00010,
    NAUGHTY  = 0b10000'00010,
    QUIET    = 0b00100'00001,
    QUIRKY   = 0b00010'00010,
    RASH     = 0b00100'00010,
    RELAXED  = 0b01000'00001,
    SASSY    = 0b00010'00001,
    SERIOUS  = 0b00001'00001,
    TIMID    = 0b00001'10000,
};
constexpr std::int32_t NATURE_ATK_MASK = 0b10000;
constexpr std::int32_t NATURE_DEF_MASK = 0b01000;
constexpr std::int32_t NATURE_SP_ATK_MASK = 0b00100;
constexpr std::int32_t NATURE_SP_DEF_MASK = 0b00010;
constexpr std::int32_t NATURE_SPEED_MASK = 0b00001;

extern std::unordered_map<std::string, Nature> nature_from_string;

#endif // NATURE_H