#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {

    class EqualPaymentInstrument : public FixedRateInstrument {
       public:
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate& rate,
                               bool recalcNotionals = false);

        void rate(const InterestRate& rate) override;

        void rate(double rate) override;

        InterestRate rate() const { return rate_; };

       private:
        void calculateRedemptions(const std::vector<Date>& dates, const InterestRate& rate, double nominal);

        bool recalcNotionals_;
        std::vector<Date> dates_;
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
