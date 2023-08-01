#include "legs.hpp"
#include <nanobench.h>

int main() {
    ankerl::nanobench::Bench legs;
    legs.title("Leg building benchmark").relative(true);
    size_t n = 10000;
    legs.run("Fixed rate leg", [&]() { LegsBenchmark::fixedRateLeg(n); });
    legs.run("Floating rate leg", [&]() { LegsBenchmark::floatingRateLeg(n); });
    legs.run("Fixed rate leg multi-threaded", [&]() { LegsBenchmark::fixedRateLeg_MT(n); });
    legs.run("Floating rate leg multi-threaded", [&]() { LegsBenchmark::floatingRateLeg_MT(n); });
}