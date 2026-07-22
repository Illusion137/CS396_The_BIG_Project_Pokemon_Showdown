#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include "Pokemon.h"
#include <array>
#include <expected>
#include <memory>
#include <string>

constexpr std::size_t MAX_POKEMON_TEAM_SIZE = 6;
extern std::array<std::shared_ptr<Pokemon>, MAX_POKEMON_TEAM_SIZE> fetch_pokemon_teams(const std::string& player_showdown_path, const std::string& opponent_showdown_path);

#endif // ORCHESTRATOR_H