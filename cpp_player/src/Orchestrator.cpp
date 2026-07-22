#include "Orchestrator.h"
#include <array>
#include <format>
#include <print>
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

std::array<std::shared_ptr<Pokemon>, MAX_POKEMON_TEAM_SIZE> fetch_pokemon_teams(const std::string& player_showdown_path, const std::string& opponent_showdown_path) {
    const CommandResult parser_result = execute_command(std::format("racket ./scheme_parser/parser.rkt {} {}", player_showdown_path, opponent_showdown_path));
    if(parser_result.exit_code != 0) {
        std::print("[OUTPUT]: {}", parser_result.output);
        throw std::runtime_error(std::format("Failed to parse the showdown export files: [{}, {}]", player_showdown_path, opponent_showdown_path));
    }
    const CommandResult validator_result = execute_command(std::format("racket ./scheme_parser/parser.rkt {} {}", player_showdown_path, opponent_showdown_path));
}