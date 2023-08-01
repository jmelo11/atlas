

#include "fixedratepricing.hpp"
#include "floatingratepricing.hpp"
#include <fstream>
#include <nanobench.h>

int main() {
    size_t n = 10000;
    ankerl::nanobench::Bench zcb;
    zcb.title("Pricing ZCB instruments").relative(true);
    zcb.run("QuantLib ZCB single-threaded", [&]() { PricingBenchmark::priceZCB_QL_ST(n); });
    zcb.run("Atlas ZCB single-threaded", [&]() { PricingBenchmark::priceZCB_AT_ST(n); });

    ankerl::nanobench::Bench bonds;

    bonds.title("Pricing fixed rate instruments").relative(true);
    double npv1;
    bonds.run("QuantLib fixed rate bond single-threaded", [&]() { npv1 = PricingBenchmark::priceBond_QL_ST(n); });
    double npv2;
    bonds.run("Atlas fixed rate bond single-threaded", [&]() { npv2 = PricingBenchmark::priceBond_AT_ST(n); });
    double npv3;
    bonds.run("QuantLib fixed rate bond multi-threaded", [&]() { npv3 = PricingBenchmark::priceBond_QL_MT(n); });
    double npv4;
    bonds.run("Atlas fixed rate bond multi-threaded", [&]() { npv4 = PricingBenchmark::priceBond_AT_MT(n); });
    double npv5;
    bonds.run("Atlas floating rate bond single-threaded", [&]() { npv5 = PricingBenchmark::priceFloatingBond_AT_ST(n); });
    double npv6;
    bonds.run("Atlas floating rate bond multi-threaded", [&]() { npv6 = PricingBenchmark::priceFloatingBond_AT_MT(n); });

    ankerl::nanobench::Bench floatingBonds;
    std::ofstream fout("pyperf.json");
    floatingBonds.output(nullptr)
        .run("Atlas floating rate bond multi-threaded", [&]() { npv6 = PricingBenchmark::priceFloatingBond_AT_MT(n); })
        .render(ankerl::nanobench::templates::json(), fout);
}