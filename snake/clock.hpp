#ifndef clock_hpp
#define clock_hpp

#include <chrono>
#include <type_traits>

using Scene_Clock = std::conditional_t<
    std::chrono::high_resolution_clock::is_steady,
    std::chrono::high_resolution_clock,
    std::chrono::steady_clock
>;

#endif /* clock_hpp */
