#include "Nature.h"
#include <string>
#include <unordered_map>

std::unordered_map<std::string, Nature> nature_from_string = {
    {"Adamant", Nature::ADAMANT},
    {"Bashful", Nature::BASHFUL},
    {"Bold",    Nature::BOLD},
    {"Brave",   Nature::BRAVE},
    {"Calm",    Nature::CALM},
    {"Careful", Nature::CAREFUL},
    {"Docile",  Nature::DOCILE},
    {"Gentle",  Nature::GENTLE},
    {"Hardy",   Nature::HARDY},
    {"Hasty",   Nature::HASTY},
    {"Impish",  Nature::IMPISH},
    {"Jolly",   Nature::JOLLY},
    {"Lax",     Nature::LAX},
    {"Lonely",  Nature::LONELY},
    {"Mild",    Nature::MILD},
    {"Modest",  Nature::MODEST},
    {"Naive",   Nature::NAIVE},
    {"Naughty", Nature::NAUGHTY},
    {"Quiet",   Nature::QUIET},
    {"Quirky",  Nature::QUIRKY},
    {"Rash",    Nature::RASH},
    {"Relaxed", Nature::RELAXED},
    {"Sassy",   Nature::SASSY},
    {"Serious", Nature::SERIOUS},
    {"Timid",   Nature::TIMID},
};