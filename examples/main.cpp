
#include "currencyconversion.hpp"
#include "pricefixedinstrument.hpp"
#include "pricefloatinginstrument.hpp"
#include "senscalculation.hpp"
using namespace Atlas;

int main() {
    pricingFixedRateInstruments();
    pricingFloatingRateInstruments();
    currencyConversion();
    sensCalculation();
    return 0;
}