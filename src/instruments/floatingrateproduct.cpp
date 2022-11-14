
#include <atlas/instruments/floatingrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateProduct::FloatingRateProduct(const QuantLib::Date& startDate,
                                             const QuantLib::Date& endDate,
                                             const FloatingRateLeg& leg)
    : leg_(leg){
        startDate_ = startDate;
        endDate_ = endDate;
    };
}  // namespace Atlas