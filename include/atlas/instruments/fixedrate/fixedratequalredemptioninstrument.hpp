#ifndef A4801BB0_4300_4C89_9784_55E269E00A56
#define A4801BB0_4300_4C89_9784_55E269E00A56

#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {
    class FixedRateEqualRedemptionInstrument : public FixedRateInstrument {
       public:
        FixedRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional,
                                        const InterestRate& rate);

     
    };
}  // namespace Atlas

#endif /* A4801BB0_4300_4C89_9784_55E269E00A56 */
