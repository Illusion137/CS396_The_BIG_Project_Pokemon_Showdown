#include "Game.h"
#include "Utils.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <format>
#include <string>
#include <vector>

// Platform-specific for raw input (no-echo, no-enter-required) [FROM OOP_2]
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
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

namespace ansi {
    constexpr const char *RESET = "\033[0m";
    constexpr const char *BOLD = "\033[1m";
    constexpr const char *DIM = "\033[2m";
    constexpr const char *RED = "\033[31m";
    constexpr const char *GREEN = "\033[32m";
    constexpr const char *YELLOW = "\033[33m";
    constexpr const char *BLUE = "\033[34m";
    constexpr const char *MAGENTA = "\033[35m";
    constexpr const char *CYAN = "\033[36m";
    constexpr const char *GRAY = "\033[90m";
}

std::string colorize(const std::string &prefix, const std::string &text) {
    return prefix + text + ansi::RESET;
}

std::string pad_visible(const std::string &text, std::int32_t width) {
    const std::int32_t pad = width - (std::int32_t)text.size();
    return pad > 0 ? text + std::string(pad, ' ') : text;
}

void Player::force_random_switch() noexcept {
    std::vector<std::int32_t> alive_indices;
    for(std::size_t i = 0; i < this->team_.size(); i++) {
        if(i == (std::size_t)this->active_pokemon_index_) continue;
        if(this->team_[i] && this->team_[i]->alive()) alive_indices.push_back(i);
    }
    if(alive_indices.empty()) return;
    this->switch_to(this->team_[alive_indices[std::rand() % alive_indices.size()]].get());
}

bool Player::switch_to(Pokemon *target) noexcept {
    if(!target) return false;
    for(std::size_t i = 0; i < this->team_.size(); i++) {
        if(this->team_[i].get() == target) {
            this->active_pokemon_index_ = i;
            this->apply_hazard_damage();
            return true;
        }
    }
    return false;
}

void Player::apply_hazard_damage() noexcept {
    auto &active = this->get_active_pokemon();
    if(!active || !active->alive()) return;
    if(this->side_.stealth_rock) {
        const double multiplier = type_on_types_multiplier(Type::ROCK, active->types());
        active->take_damage_percent(0.125 * multiplier);
    }
    if(active->alive() && this->side_.spikes > 0) {
        constexpr double spikes_damage_fraction[] = {0.0, 1.0 / 8.0, 1.0 / 6.0, 1.0 / 4.0};
        active->take_damage_percent(spikes_damage_fraction[this->side_.spikes]);
    }
}

Pokemon *Player::get_switch_target(std::int32_t slot_index) noexcept {
    Pokemon *active = this->get_active_pokemon().get();
    std::int32_t slot = 0;
    for(auto &teammate : this->team_) {
        if(!teammate || teammate.get() == active) continue;
        if(slot == slot_index) return teammate.get();
        slot++;
    }
    return nullptr;
}

constexpr std::int32_t LEFT_WIDTH = 16;
constexpr std::int32_t BAR_WIDTH = 20;
constexpr std::int32_t RIGHT_FIELD_WIDTH = BAR_WIDTH + 7;
constexpr std::size_t MAX_LOG_LINES = 11;

std::array<char, MAX_POKEMON_TEAM_SIZE> team_status_chars(Player &player) noexcept {
    std::array<char, MAX_POKEMON_TEAM_SIZE> chars{};
    for(std::size_t i = 0; i < MAX_POKEMON_TEAM_SIZE; i++) {
        const auto &pokemon = player.get_team()[i];
        chars[i] = !pokemon ? '_' : (pokemon->alive() ? 'O' : 'X');
    }
    return chars;
}

const char *team_slot_color(char slot) noexcept {
    switch(slot) {
        case 'O': return ansi::GREEN;
        case 'X': return ansi::RED;
        default: return ansi::GRAY;
    }
}

std::string team_status_row(const std::array<char, MAX_POKEMON_TEAM_SIZE> &chars, std::size_t row) {
    std::string result = "[";
    for(std::size_t i = 0; i < 3; i++) {
        if(i > 0) result += ' ';
        const char slot = chars[row * 3 + i];
        result += colorize(team_slot_color(slot), std::string(1, slot));
    }
    result += "]";
    return result;
}

const char *move_use_result_string(MoveUseResult result) noexcept {
    switch(result) {
        case MoveUseResult::SUCCESSFUL: return "success";
        case MoveUseResult::IMMUNE: return "immune";
        case MoveUseResult::MISSED: return "missed";
        case MoveUseResult::FAILED: return "failed";
        case MoveUseResult::CRITICAL: return "critical";
        case MoveUseResult::FLINCHED: return "flinched";
        case MoveUseResult::CONFUSED_SELF_HIT: return "hurt itself in confusion";
        default: return "";
    }
}

const char *log_result_color(MoveUseResult result) noexcept {
    switch(result) {
        case MoveUseResult::SUCCESSFUL: return ansi::GREEN;
        case MoveUseResult::MISSED: return ansi::YELLOW;
        case MoveUseResult::IMMUNE: return ansi::CYAN;
        case MoveUseResult::FAILED: return ansi::RED;
        case MoveUseResult::CRITICAL: return ansi::MAGENTA;
        case MoveUseResult::FLINCHED: return ansi::YELLOW;
        case MoveUseResult::CONFUSED_SELF_HIT: return ansi::RED;
        default: return ansi::RESET;
    }
}

std::int32_t hp_percent_of(const Pokemon &pokemon) noexcept {
    return std::llround((pokemon.current_hp() * 100.0) / pokemon.max_hp());
}

const char *hp_bar_color(std::int32_t hp_percent) noexcept {
    if(hp_percent > 50) return ansi::GREEN;
    if(hp_percent > 20) return ansi::YELLOW;
    return ansi::RED;
}

const char *pp_color(std::int32_t pp, std::int32_t max_pp) noexcept {
    if(pp <= 0 || max_pp <= 0) return ansi::RED;
    const double ratio = (double)pp / max_pp;
    if(ratio > 0.5) return ansi::GREEN;
    if(ratio > 0.2) return ansi::YELLOW;
    return ansi::RED;
}

const char *effectiveness_color(double multiplier) noexcept {
    if(multiplier <= 0.0) return ansi::GRAY;
    if(multiplier < 1.0) return ansi::RED;
    if(multiplier > 1.0) return ansi::GREEN;
    return ansi::RESET;
}

std::string effectiveness_string(double multiplier) {
    if(multiplier <= 0.0) return "No effect!";
    if(multiplier < 1.0) return "Not very effective...";
    if(multiplier > 1.0) return "Super effective!";
    return "Normal effectiveness.";
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
        return std::to_string((std::int32_t)std::round(multiplier));
    }
    std::string formatted = std::format("{:.2f}", multiplier);
    while(formatted.back() == '0') formatted.pop_back();
    if(formatted.back() == '.') formatted.pop_back();
    return formatted;
}

const char *status_color(NonVolitileStatusCondition condition) noexcept {
    switch(condition) {
        case NonVolitileStatusCondition::BURN: return ansi::RED;
        case NonVolitileStatusCondition::FREEZE: return ansi::CYAN;
        case NonVolitileStatusCondition::PARALYSIS: return ansi::YELLOW;
        case NonVolitileStatusCondition::POISON:
        case NonVolitileStatusCondition::BADLY_POISONED: return ansi::MAGENTA;
        case NonVolitileStatusCondition::SLEEP: return ansi::GRAY;
        default: return ansi::RESET;
    }
}

struct RenderedField {
    std::string line;
    std::int32_t visible_width;
};

RenderedField render_status_and_boosts(const Pokemon &pokemon) {
    std::string line;
    std::int32_t visible_width = 0;
    const char *status = status_condition_abbreviation(pokemon.status_condition());
    if(status[0] != '\0') {
        const std::string segment = std::format("[{}]", status);
        visible_width += segment.size();
        line += colorize(status_color(pokemon.status_condition()), segment);
    }
    struct StageText { std::int32_t stage; const char *string; };
    const StageText stages[] = {
        {pokemon.atk_stage(), "Atk"},
        {pokemon.def_stage(), "Def"},
        {pokemon.sp_atk_stage(), "SpA"},
        {pokemon.sp_def_stage(), "SpD"},
        {pokemon.speed_stage(), "Spe"},
    };
    for(const auto &entry : stages) {
        if(entry.stage == 0) continue;
        const std::string segment = std::format("[{}x {}]", format_multiplier(stage_multiplier(entry.stage)), entry.string);
        visible_width += segment.size();
        line += colorize(entry.stage > 0 ? ansi::GREEN : ansi::RED, segment);
    }
    return {line, visible_width};
}

void Game::render_frame() {
    const auto &opponent_active = this->opponent_.get_active_pokemon();
    const auto &player_active = this->player_.get_active_pokemon();

    const auto opponent_status = team_status_chars(this->opponent_);
    const auto player_status = team_status_chars(this->player_);

    const std::int32_t opponent_hp_percent = hp_percent_of(*opponent_active);
    const std::string turn_string = std::format("[TURN {}]", this->turn_);
    const std::string opponent_header = std::format("{} L{}", opponent_active->name(), opponent_active->level());
    const std::string opponent_hp_plain = std::format("{}% [{}]", opponent_hp_percent, render_hp_bar(opponent_hp_percent));
    const std::string opponent_hp_field = colorize(hp_bar_color(opponent_hp_percent), pad_visible(opponent_hp_plain, RIGHT_FIELD_WIDTH));
    const RenderedField opponent_boosts = render_status_and_boosts(*opponent_active);
    ll::println("{:<{}}{:<{}}  {}", turn_string, LEFT_WIDTH, opponent_header, RIGHT_FIELD_WIDTH, team_status_row(opponent_status, 0));
    ll::println("{:>{}}{}  {}", "", LEFT_WIDTH, opponent_hp_field, team_status_row(opponent_status, 1));
    ll::println("{:>{}}{}", "", LEFT_WIDTH, opponent_boosts.line);

    const std::int32_t player_hp_percent = hp_percent_of(*player_active);
    const std::string player_hp_plain = std::format("[{}] {}%", render_hp_bar(player_hp_percent), player_hp_percent);
    const std::string player_hp_field = colorize(hp_bar_color(player_hp_percent), pad_visible(player_hp_plain, RIGHT_FIELD_WIDTH));
    const RenderedField player_boosts = render_status_and_boosts(*player_active);
    const std::string player_boosts_field = player_boosts.line + std::string(std::max(0, RIGHT_FIELD_WIDTH - player_boosts.visible_width), ' ');
    ll::println("{} L{}", player_active->name(), player_active->level());
    ll::println("{} {}", player_hp_field, team_status_row(player_status, 0));
    ll::println("{} {}", player_boosts_field, team_status_row(player_status, 1));

    ll::println();
    ll::println("What will ({}) do? [HP {}/{}]", player_active->name(), player_active->current_hp(), player_active->max_hp());
    ll::println();

    ll::println("ATTACK");
    const std::string highlight_color = std::string(ansi::BOLD) + ansi::CYAN;
    std::string moves_line;
    const Move *selected_move = nullptr;
    for(std::size_t i = 0; i < player_active->moves().size(); i++) {
        const auto &move = player_active->moves()[i];
        if(!move) continue;
        if(!moves_line.empty()) moves_line += ' ';
        if(i == (std::size_t)selected_move_index_ && on_move_line_) {
            moves_line += colorize(highlight_color, std::format("[{}]", move->info().name()));
            selected_move = move.get();
        } else {
            moves_line += colorize(ansi::DIM, std::format("_{}_", move->info().name()));
        }
    }
    ll::println("{}", moves_line);
    if(selected_move) {
        const std::string pp_text = colorize(pp_color(selected_move->pp(), selected_move->info().pp()), std::format("{}/{}", selected_move->pp(), selected_move->info().pp()));
        ll::println("[{:<7}{}]", type_to_string(selected_move->info().type()), pp_text);
        if(selected_move->info().category() != MoveCategory::NON_DAMAGING) {
            const double multiplier = type_on_types_multiplier(selected_move->info().type(), opponent_active->types());
            ll::println("{}", colorize(effectiveness_color(multiplier), effectiveness_string(multiplier)));
        }
    }
    ll::println();

    ll::println("SWITCH");
    std::string switch_line;
    std::int32_t switch_slot = 0;
    for(auto &teammate : this->player_.get_team()) {
        if(!teammate || teammate.get() == player_active.get()) continue;
        if(!switch_line.empty()) switch_line += ' ';
        if(switch_slot == selected_switch_index_ && !on_move_line_) {
            switch_line += colorize(highlight_color, std::format("[{}]", teammate->name()));
        } else {
            switch_line += colorize(ansi::DIM, std::format("_{}_", teammate->name()));
        }
        switch_slot++;
    }
    ll::println("{}", switch_line);
    if(Pokemon *selected_switch_target = this->player_.get_switch_target(selected_switch_index_)) {
        const std::int32_t switch_hp_percent = hp_percent_of(*selected_switch_target);
        ll::println("[{}]", colorize(hp_bar_color(switch_hp_percent), std::format("{}%", switch_hp_percent)));
    }

    ll::println();
    ll::println("[log]----------------");
    for(auto it = this->log_.crbegin(); it != this->log_.crend(); it++) ll::println("{}", it->data());
}

void Game::log_move(const Pokemon &actor, const Move &move, MoveUseResult result) {
    if(const auto *switch_move = dynamic_cast<const Move_Switch*>(&move)) {
        if(result == MoveUseResult::SUCCESSFUL && switch_move->target()) {
            const std::string switch_color = std::string(ansi::BOLD) + ansi::BLUE;
            this->push_log(colorize(switch_color, std::format("{} switched to {}.", actor.name(), switch_move->target()->name())));
            this->log_faint_maybe(*switch_move->target());
        }
        return;
    }
    this->push_log(colorize(log_result_color(result), std::format("{} used {} [{}].", actor.name(), move.info().name(), move_use_result_string(result))));
}

void Game::log_faint_maybe(const Pokemon &pokemon) {
    if(!pokemon.alive()) this->push_log(colorize(std::string(ansi::BOLD) + ansi::RED, std::format("{} fainted!", pokemon.name())));
}

void Game::log_switch_in(const Pokemon &pokemon) {
    this->push_log(colorize(std::string(ansi::BOLD) + ansi::BLUE, std::format("{} was sent out!", pokemon.name())));
    this->log_faint_maybe(pokemon);
}

void Game::log_turn_divider() {
    this->push_log(colorize(ansi::DIM, "--"));
}

void Game::push_log(std::string entry) {
    this->log_.insert(this->log_.begin(), std::move(entry));
    if(this->log_.size() > MAX_LOG_LINES) this->log_.pop_back();
}

void Game::simulate_turn(Move &player_action) {
    const auto opponent_selected_move = this->opponent_.get_active_pokemon()->random_move();

    const std::int32_t player_priority = player_action.priority(*this->player_.get_active_pokemon(), *this->opponent_.get_active_pokemon());
    const std::int32_t opponent_priority = opponent_selected_move->priority(*this->opponent_.get_active_pokemon(), *this->player_.get_active_pokemon());

    const bool player_higher_priority = player_priority > opponent_priority;
    const bool player_same_priority = player_priority == opponent_priority;
    const bool player_higher_speed = player_.get_active_pokemon()->speed() > this->opponent_.get_active_pokemon()->speed();
    const bool player_same_speed = player_.get_active_pokemon()->speed() == this->opponent_.get_active_pokemon()->speed();
    const bool player_rng_first = chance_of(50);

    const bool player_goes_first = player_higher_priority || (player_same_priority && player_higher_speed) || (player_same_priority && player_same_speed && player_rng_first);

    const auto use_player_action = [&]() {
        Pokemon *player_pokemon = this->player_.get_active_pokemon().get();
        Pokemon *opponent_pokemon_before = this->opponent_.get_active_pokemon().get();
        const MoveUseResult player_result = player_action.use(*player_pokemon, *opponent_pokemon_before, this->player_, this->opponent_);
        this->log_move(*player_pokemon, player_action, player_result);
        this->log_faint_maybe(*player_pokemon);
        Pokemon *opponent_pokemon_after = this->opponent_.get_active_pokemon().get();
        if(opponent_pokemon_after != opponent_pokemon_before) this->log_switch_in(*opponent_pokemon_after);
        else this->log_faint_maybe(*opponent_pokemon_before);
    };
    const auto use_opponent_action = [&]() {
        Pokemon *opponent_pokemon = this->opponent_.get_active_pokemon().get();
        Pokemon *player_pokemon_before = this->player_.get_active_pokemon().get();
        const MoveUseResult opponent_result = opponent_selected_move->use(*opponent_pokemon, *player_pokemon_before, this->opponent_, this->player_);
        this->log_move(*opponent_pokemon, *opponent_selected_move, opponent_result);
        this->log_faint_maybe(*opponent_pokemon);
        Pokemon *player_pokemon_after = this->player_.get_active_pokemon().get();
        if(player_pokemon_after != player_pokemon_before) this->log_switch_in(*player_pokemon_after);
        else this->log_faint_maybe(*player_pokemon_before);
    };

    if(player_goes_first) {
        use_player_action();
        if(this->opponent_.get_active_pokemon()->alive()) {
            use_opponent_action();
        }
    }
    else {
        use_opponent_action();
        if(this->player_.get_active_pokemon()->alive()) {
            use_player_action();
        }
    }
}

void Game::resolve_post_turn_switches() {
    // roost exists until the turn ends; so since this happens after turn good place to put it I guess, but could also be done polymorphically if I wanted to do more restructuring
    this->player_.get_active_pokemon()->remove_volatile_status(VolitileStatusCondition::ROOST);
    this->opponent_.get_active_pokemon()->remove_volatile_status(VolitileStatusCondition::ROOST);
    if(this->opponent_.consume_pending_switch() || !this->opponent_.get_active_pokemon()->alive()) {
        Pokemon *before = this->opponent_.get_active_pokemon().get();
        this->opponent_.force_random_switch();
        Pokemon *after = this->opponent_.get_active_pokemon().get();
        if(after != before) this->log_switch_in(*after);
    }
    if(this->player_.consume_pending_switch() || !this->player_.get_active_pokemon()->alive()) {
        this->must_switch_ = true;
        this->on_move_line_ = false;
        this->selected_switch_index_ = 0;
    }
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
            if(must_switch_) {
                Pokemon *target = player_.get_switch_target(selected_switch_index_);
                if(target && target->alive()) {
                    player_.switch_to(target);
                    this->log_switch_in(*target);
                    must_switch_ = false;
                    selected_switch_index_ = 0;
                    on_move_line_ = true;
                }
            }
            else if(on_move_line_) {
                auto &move = player_.get_active_pokemon()->moves()[selected_move_index_];
                if(move && move->pp()) {
                    simulate_turn(*move);
                    this->resolve_post_turn_switches();
                    this->log_turn_divider();
                    this->turn_++;
                }
            }
            else {
                Pokemon *target = player_.get_switch_target(selected_switch_index_);
                if(target && target->alive()) {
                    Pokemon *outgoing = player_.get_active_pokemon().get();
                    outgoing->add_volatile_status(std::make_unique<Status_SwitchingOut>());
                    Move_Switch switch_action(target);
                    simulate_turn(switch_action);
                    outgoing->remove_volatile_status(VolitileStatusCondition::SWITCHING_OUT);
                    this->resolve_post_turn_switches();
                    this->log_turn_divider();
                    this->turn_++;
                }
            }
        }
        else {
            switch(input) {
                case 'w': case 'W':
                    if(!must_switch_) {
                        on_move_line_ = !on_move_line_;
                        selected_move_index_ = 0;
                        selected_switch_index_ = 0;
                    }
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
                    if(!must_switch_) {
                        on_move_line_ = !on_move_line_;
                        selected_move_index_ = 0;
                        selected_switch_index_ = 0;
                    }
                    break;
            }
        }
    }
}