#include "Orchestrator.h"
#include "Utils.h"
#include <array>
#include <format>
#include <fstream>
#include <memory>
#include <stdexcept>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

struct CommandResult {
    std::string output;
    int exit_code;
};

// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
// https://stackoverflow.com/questions/27306764/capturing-exit-status-code-of-child-process
CommandResult execute_command(const std::string& command) {
    std::string full_command = command + " 2>&1"; 
    std::array<char, 256> buffer;
    std::string result_output;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(full_command.c_str(), "r"), pclose);
    
    if(!pipe) throw std::runtime_error("popen() failed!");

    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result_output += buffer.data();
    }

    int raw_status = pclose(pipe.release());
    
#ifndef _WIN32
    int exit_code = WEXITSTATUS(raw_status);
#else
    int exit_code = raw_status;
#endif

    return {result_output, exit_code};
}

std::pair<PokemonTeam, PokemonTeam> fetch_pokemon_teams(const std::string& player_showdown_path, const std::string& opponent_showdown_path) {
    const CommandResult parser_result = execute_command(std::format("racket ./scheme_parser/parser.rkt {} {}", player_showdown_path, opponent_showdown_path));
    if(parser_result.exit_code != 0) {
        print("[OUTPUT]: {}", parser_result.output);
        throw std::runtime_error(std::format("Failed to parse the showdown export files: [{}, {}]", player_showdown_path, opponent_showdown_path));
    }

    const char *validation_player_json_path = "./.tmp/player.json";
    const char *validation_opponent_json_path = "./.tmp/opponent.json";

    const CommandResult validator_result = execute_command(std::format("swipl ./prolog_validator/validator.pl -- {} {}", validation_player_json_path, validation_opponent_json_path));
    if(validator_result.exit_code != 0) {
        print("[OUTPUT]: {}", validator_result.output);
        throw std::runtime_error(std::format("Failed to validate the showdown export files: [{}, {}]", validation_player_json_path, validation_opponent_json_path));
    }

    std::ifstream player_json_file(validation_player_json_path);
    nlohmann::json player_json = nlohmann::json::parse(player_json_file);

    std::ifstream opponent_json_file(validation_opponent_json_path);
    nlohmann::json opponent_json = nlohmann::json::parse(opponent_json_file);

    PokemonTeam player_team = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    PokemonTeam opponent_team = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    for(std::size_t i = 0; i < player_json.size(); i++) player_team[i] = std::make_unique<Pokemon>(player_json[i]);
    for(std::size_t i = 0; i < opponent_json.size(); i++) opponent_team[i] = std::make_unique<Pokemon>(opponent_json[i]);

    return { std::move(player_team), std::move(opponent_team) };
}