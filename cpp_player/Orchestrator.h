#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include "Pokemon.h"
#include <expected>
#include <string>

namespace orch {
    extern std::expected<Pokemon, std::string> get_pokemon_teams();
}

#endif // ORCHESTRATOR_H