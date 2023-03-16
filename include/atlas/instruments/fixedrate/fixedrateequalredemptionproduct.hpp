#ifndef A4801BB0_4300_4C89_9784_55E269E00A56
#define A4801BB0_4300_4C89_9784_55E269E00A56

#include <atlas/instruments/fixedrateproduct.hpp>

namespace Atlas {
    class FixedRateEqualRedemptionProduct : public FixedRateProduct {
       public:
        FixedRateEqualRedemptionProduct(const Date& startDate, const Date& endDate, Frequency freq, double notional,
                                        const InterestRate& rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* A4801BB0_4300_4C89_9784_55E269E00A56 */
