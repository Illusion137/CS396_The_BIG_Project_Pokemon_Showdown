#include "Game.h"
#include "Orchestrator.h"
#include <cstdint>
#include <cstdlib>
#include <print>

std::int32_t main(std::int32_t argc, char **argv) {
    if(argc != 3) { // 3 because of the default first arg
        std::println("Expected 2 input args: -- player_file.showdown opponent_file.showdown");
        return EXIT_FAILURE;
    }
    auto [player_team, opponent_team] = fetch_pokemon_teams(argv[1], argv[2]);
    Game game(std::move(player_team), std::move(opponent_team));
    game.start();
    return EXIT_SUCCESS;
}