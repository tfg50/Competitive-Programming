#ifndef RNG_H
#define RNG_H

#include <random>
#include <chrono>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

#endif