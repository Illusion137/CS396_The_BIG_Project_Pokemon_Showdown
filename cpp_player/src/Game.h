#ifndef GAME_H
#define GAME_H

#include "Orchestrator.h"
#include "Side.h"
#include <memory>
#include <string>
#include <vector>

class Player {
public:
    Player(PokemonTeam team): team_(std::move(team)), active_pokemon_index_(0) {}
    std::unique_ptr<Pokemon> &get_active_pokemon() { return this->team_[this->active_pokemon_index_]; }
    PokemonTeam &get_team() { return team_; }
    std::int32_t get_team_count() {
        std::int32_t count = 0;
        for(const auto &p : this->get_team()) {
            if(p) count++;
        }
        return count;
    }

    Side &side() noexcept { return this->side_; }

    void request_switch() noexcept { this->pending_switch_ = true; }
    bool consume_pending_switch() noexcept {
        const bool was_pending = this->pending_switch_;
        this->pending_switch_ = false;
        return was_pending;
    }

    void force_random_switch() noexcept;
    bool switch_to(Pokemon *target) noexcept;
    Pokemon *get_switch_target(std::int32_t slot_index) noexcept;
private:
    void apply_hazard_damage() noexcept;

    PokemonTeam team_;
    std::int32_t active_pokemon_index_;
    Side side_;
    bool pending_switch_ = false;
};

struct Menu {

};

class Game {
public:
    Game(PokemonTeam player_team, PokemonTeam opponent_team): player_(std::move(player_team)), opponent_(std::move(opponent_team)) {};
    void start();
private:
    void render_frame();
    void simulate_turn(Move &player_action);
    void resolve_post_turn_switches();
    void log_move(const Pokemon &actor, const Move &move, MoveUseResult result);
    void log_faint_maybe(const Pokemon &pokemon);
    void log_switch_in(const Pokemon &pokemon);
    void log_turn_divider();
    void push_log(std::string entry);

    bool game_over();
    Player player_;
    Player opponent_;

    // player controls
    std::int32_t selected_move_index_ = 0;
    std::int32_t selected_switch_index_ = 0;
    bool on_move_line_ = true;
    bool must_switch_ = false;

    std::int32_t turn_ = 1;
    std::vector<std::string> log_;
};

#endif // GAME_H