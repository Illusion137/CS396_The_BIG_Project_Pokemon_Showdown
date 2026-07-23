#include "Game.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <format>
#include <print>
#include <string>
#include <vector>

// Platform-specific for raw input (no-echo, no-enter-required) [FROM OOP_2]
#ifdef _WIN32
#include <conio.h>
#define getch _getch
#define GETCH_ENTER '\r'
#else
#include <termios.h>
#include <unistd.h>
// Raw character read on Unix (disable canonical mode + echo)
static char getch() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#define GETCH_ENTER '\n'
#endif

// from OOP_2
void cls() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

void Player::force_random_switch() noexcept {
    std::vector<std::int32_t> alive_indices;
    for(std::size_t i = 0; i < this->team_.size(); i++) {
        if(i == this->active_pokemon_index_) continue;
        if(this->team_[i] && this->team_[i]->alive()) alive_indices.push_back(i);
    }
    if(alive_indices.empty()) return;
    this->active_pokemon_index_ = alive_indices[std::rand() % alive_indices.size()];
}

constexpr std::int32_t LEFT_WIDTH = 16;
constexpr std::int32_t BAR_WIDTH = 20;
constexpr std::int32_t RIGHT_FIELD_WIDTH = BAR_WIDTH + 7;

std::int32_t hp_percent_of(const Pokemon &pokemon) noexcept {
    return static_cast<std::int32_t>(std::llround((pokemon.current_hp() * 100.0) / pokemon.max_hp()));
}

std::string render_hp_bar(std::int32_t hp_percent) {
    const std::int32_t filled = std::clamp((hp_percent * BAR_WIDTH) / 100, 0, BAR_WIDTH);
    return std::string(filled, '=') + std::string(BAR_WIDTH - filled, '-');
}

double stage_multiplier(std::int32_t stage) noexcept {
    return stage >= 0 ? (2.0 + stage) / 2.0 
        : 2.0 / (2.0 - stage);
}

std::string format_multiplier(double multiplier) {
    if(std::fabs(multiplier - std::round(multiplier)) < 1e-9) {
        return std::to_string(static_cast<std::int32_t>(std::round(multiplier)));
    }
    std::string formatted = std::format("{:.2f}", multiplier);
    while(formatted.back() == '0') formatted.pop_back();
    if(formatted.back() == '.') formatted.pop_back();
    return formatted;
}

std::string render_status_and_boosts(const Pokemon &pokemon) {
    std::string line;
    const char *status = status_condition_abbreviation(pokemon.status_condition());
    if(status[0] != '\0') line += std::format("[{}]", status);
    const struct { std::int32_t stage; const char *label; } stages[] = {
        {pokemon.atk_stage(), "Atk"},
        {pokemon.def_stage(), "Def"},
        {pokemon.sp_atk_stage(), "SpA"},
        {pokemon.sp_def_stage(), "SpD"},
        {pokemon.speed_stage(), "Spe"},
    };
    for(const auto &entry : stages) {
        if(entry.stage == 0) continue;
        line += std::format("[{}x {}]", format_multiplier(stage_multiplier(entry.stage)), entry.label);
    }
    return line;
}

void Game::render_frame() {
    const auto &opponent_active = this->opponent_.get_active_pokemon();
    const auto &player_active = this->player_.get_active_pokemon();

    const std::int32_t opponent_hp_percent = hp_percent_of(*opponent_active);
    const std::string opponent_header = std::format("{} L{}", opponent_active->name(), opponent_active->level());
    std::println("{:>{}}{:<{}}", "", LEFT_WIDTH, opponent_header, RIGHT_FIELD_WIDTH);
    std::println("{:>{}}{}% [{}]", "", LEFT_WIDTH, opponent_hp_percent, render_hp_bar(opponent_hp_percent));
    std::println("{:>{}}{}", "", LEFT_WIDTH, render_status_and_boosts(*opponent_active));

    const std::int32_t player_hp_percent = hp_percent_of(*player_active);
    std::println("{} L{}", player_active->name(), player_active->level());
    std::println("[{}] {}%", render_hp_bar(player_hp_percent), player_hp_percent);
    std::println("{}", render_status_and_boosts(*player_active));

    std::println();
    std::println("What will ({}) do? [HP {}/{}]", player_active->name(), player_active->current_hp(), player_active->max_hp());
    std::println();

    std::println("ATTACK");
    std::string moves_line;
    const Move *selected_move = nullptr;
    for(std::size_t i = 0; i < player_active->moves().size(); i++) {
        const auto &move = player_active->moves()[i];
        if(!move) continue;
        if(!moves_line.empty()) moves_line += ' ';
        if(i == selected_move_index_ && on_move_line_) {
            moves_line += std::format("[{}]", move->info().name());
            selected_move = move.get();
        } else {
            moves_line += std::format("_{}_", move->info().name());
        }
    }
    std::println("{}", moves_line);
    if(selected_move) {
        std::println("[{:<7}{}/{}]", type_to_string(selected_move->info().type()), selected_move->pp(), selected_move->info().pp());
    }
    std::println();

    std::println("SWITCH");
    std::string switch_line;
    Pokemon *selected_switch_target = nullptr;
    std::size_t switch_slot = 0;
    for(auto &teammate : this->player_.get_team()) {
        if(!teammate || teammate.get() == player_active.get()) continue;
        if(!switch_line.empty()) switch_line += ' ';
        if(switch_slot == selected_switch_index_ && !on_move_line_) {
            switch_line += std::format("[{}]", teammate->name());
            selected_switch_target = teammate.get();
        } else {
            switch_line += std::format("_{}_", teammate->name());
        }
        switch_slot++;
    }
    std::println("{}", switch_line);
    if(selected_switch_target) {
        std::println("[{}%]", hp_percent_of(*selected_switch_target));
    }
}

void Game::simulate_turn() {

}

bool Game::game_over() {
    const auto team_wiped = [](Player &player) {
        for(const auto &pokemon : player.get_team()) {
            if(pokemon && pokemon->alive()) return false;
        }
        return true;
    };
    return team_wiped(this->player_) || team_wiped(this->opponent_);
}

void Game::start() {
    while(!this->game_over()) {
        cls();
        this->render_frame();
        const char input = getch();
        if(input == 'q') return;
        else if(input == GETCH_ENTER) {
            if(on_move_line_) {
                if(player_.get_active_pokemon()->moves()[selected_move_index_]->pp()) {
                    // play
                }
            }
            else {
                if(player_.get_team()[selected_switch_index_]->alive()) {
                    // switch
                }
            }
        }
        else {
            switch(input) {
                case 'w': case 'W':
                    on_move_line_ = !on_move_line_;
                    selected_move_index_ = 0;
                    selected_switch_index_ = 0;
                    break;
                case 'a': case 'A':
                    if(on_move_line_) selected_move_index_ = (player_.get_active_pokemon()->moves_count() + selected_move_index_ - 1) % player_.get_active_pokemon()->moves_count(); 
                    else selected_switch_index_ = (player_.get_team_count() - 1 + selected_switch_index_ - 1) % (player_.get_team_count() - 1);
                    break;
                case 'd': case 'D':
                    if(on_move_line_) selected_move_index_ = (selected_move_index_ + 1) % player_.get_active_pokemon()->moves_count();
                    else selected_switch_index_ = (selected_switch_index_ + 1) % (player_.get_team_count() - 1);
                    break;
                case 's': case 'S':
                    on_move_line_ = !on_move_line_;
                    selected_move_index_ = 0;
                    selected_switch_index_ = 0;
                    break;
            }
        }
    }
}