#include "Pokemon.h"
#include <algorithm>

std::int32_t calculate_stat_plus(const std::int32_t stat, const std::int32_t stat_plus) {
    if(stat_plus == 0) return stat;
    if(stat_plus > 0) return stat * (1.0 + (stat_plus * 0.5));
    if(stat_plus < 0) return stat * (-1.0 - (stat_plus * 0.5));
}
static std::int32_t calculate_stat_initial_pass(std::int32_t base_stat, std::int32_t base_stat, std::int32_t base_stat) {
    double f = 2 * base_stat + stat_iv + std::floor(0.25 * stat_ev);
    return std::floor(
        () * level)
        / 100.0);
}

std::int32_t Pokemon::attack() const noexcept {
    return calculate_stat_plus(this->base_pokemon_.get()->attack(), this->atk_plus_);
}
std::int32_t Pokemon::defense() const noexcept {
    return calculate_stat_plus(this->base_pokemon_.get()->defense(), this->def_plus_);
}
std::int32_t Pokemon::sp_atk() const noexcept {
    return calculate_stat_plus(this->base_pokemon_.get()->sp_atk(), this->sp_atk_plus_);
}
std::int32_t Pokemon::sp_def() const noexcept {
    return calculate_stat_plus(this->base_pokemon_.get()->sp_def(), this->sp_def_plus_);
}
std::int32_t Pokemon::speed() const noexcept {
    return calculate_stat_plus(this->base_pokemon_.get()->speed(), this->speed_plus_);
}

void Pokemon::clear_stat_plus() {
    this->atk_plus_ = 0;
    this->def_plus_ = 0;
    this->sp_atk_plus_ = 0;
    this->sp_def_plus_ = 0;
    this->speed_plus_ = 0;
}
void Pokemon::apply_stat_plus(std::int32_t atk_plus, std::int32_t def_plus, std::int32_t sp_atk_plus, std::int32_t sp_def_plus, std::int32_t speed_plus) {
    this->atk_plus_ = std::min(std::max(atk_plus, -6), 6);
    this->def_plus_ = std::min(std::max(def_plus, -6), 6);
    this->sp_atk_plus_ = std::min(std::max(sp_atk_plus, -6), 6);
    this->sp_def_plus_ = std::min(std::max(sp_def_plus, -6), 6);
    this->speed_plus_ = std::min(std::max(speed_plus, -6), 6);
}