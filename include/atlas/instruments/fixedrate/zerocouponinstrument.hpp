#ifndef B50701D3_345F_46BA_A774_CC39E13E6595
#define B50701D3_345F_46BA_A774_CC39E13E6595

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {
    class ZeroCouponInstrument : public FixedRateInstrument {
       public:
        ZeroCouponInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate);
    };
}  // namespace Atlas

#endif /* B50701D3_345F_46BA_A774_CC39E13E6595 */
