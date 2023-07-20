

#include "fixedratepricing.hpp"
#include <nanobench.h>

int main() {
    size_t n = 100000;
    ankerl::nanobench::Bench zcb;
    zcb.title("Pricing ZCB instruments").relative(true);
    zcb.run("QuantLib ZCB single-threaded", [&]() { PricingBenchmark::priceZCB_QL_ST(n); });
    zcb.run("Atlas ZCB single-threaded", [&]() { PricingBenchmark::priceZCB_AT_ST(n); });

    ankerl::nanobench::Bench bonds;
    bonds.title("Pricing fixed rate instruments").relative(true);
    bonds.run("QuantLib fixed rate bond single-threaded", [&]() { PricingBenchmark::priceBond_QL_ST(n); });
    bonds.run("Atlas fixed rate bond single-threaded", [&]() { PricingBenchmark::priceBond_AT_ST(n); });
    bonds.run("QuantLib fixed rate bond multi-threaded", [&]() { PricingBenchmark::priceBond_QL_MT(n); });
    bonds.run("Atlas fixed rate bond multi-threaded", [&]() { PricingBenchmark::priceBond_AT_MT(n); });
}