#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <format>
#include <iostream>
#include <string>

extern bool chance_of(std::int32_t percent) noexcept;

#ifdef _WIN32
inline void println() { std::cout << '\n'; }
template<typename... Args>
void println(const std::string &fmt, Args&&... args) {
    std::cout << std::vformat(fmt, std::make_format_args(args...)) << '\n';
}
template<typename... Args>
void print(const std::string &fmt, Args&&... args) {
    std::cout << std::vformat(fmt, std::make_format_args(args...));
}
#elif
// I thought I might need this to be different for linux so idek; potential useless #elif
inline void println() { std::cout << '\n'; }
template<typename... Args>
void println(const std::string &fmt, Args&&... args) {
    std::cout << std::vformat(fmt, std::make_format_args(args...)) << '\n';
}
template<typename... Args>
void print(const std::string &fmt, Args&&... args) {
    std::cout << std::vformat(fmt, std::make_format_args(args...));
}
#endif

#endif // UTILS_H
