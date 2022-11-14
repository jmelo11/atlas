#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateproduct.hpp>

namespace Atlas {
    class Redemption;
    class FixedRateCoupon;

    class EqualPaymentProduct : public FixedRateProduct {
       public:
        EqualPaymentProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                            QuantLib::Frequency freq, double notional,
                            const QuantLib::InterestRate& rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;

       private:
        void calculateRedemptions(const std::vector<QuantLib::Date>& dates,
                                  const QuantLib::InterestRate& rate, double nominal);
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
