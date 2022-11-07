#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/interestrate.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {
    class Redemption;
    class FixedRateCoupon;

    class EqualPaymentProduct : public Instrument {
       public:
        EqualPaymentProduct(const QuantLib::Date& start, const QuantLib::Date& end,
                            QuantLib::Frequency freq, double notional, QuantLib::InterestRate rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;

       protected:
        std::vector<Redemption> calculateRedemptions(const std::vector<QuantLib::Date>& dates,
                                                     const QuantLib::InterestRate& rate,
                                                     double nominal) const;
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
