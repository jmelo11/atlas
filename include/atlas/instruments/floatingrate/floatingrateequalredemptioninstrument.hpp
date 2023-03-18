#ifndef F4126FFB_083C_4F17_8D47_AFE69F5C4B5A
#define F4126FFB_083C_4F17_8D47_AFE69F5C4B5A

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {
    class FloatingRateEqualRedemptionInstrument : public FloatingRateInstrument {
       public:
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, double spread,
                                              const RateIndexConfiguration& index);
    };
}  // namespace Atlas

#endif /* F4126FFB_083C_4F17_8D47_AFE69F5C4B5A */
