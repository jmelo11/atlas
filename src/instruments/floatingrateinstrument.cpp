
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FloatingRateInstrument::FloatingRateInstrument(const Date& startDate, const Date& endDate, double notional, double spread,
                                                   const FloatingRateLeg& leg)
    : leg_(leg), spread_(spread) {
        startDate_ = startDate;
        endDate_   = endDate;
        notional_  = notional;
    };

    void FloatingRateInstrument::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FloatingRateInstrument::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas