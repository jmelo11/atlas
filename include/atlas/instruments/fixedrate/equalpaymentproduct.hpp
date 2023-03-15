#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateproduct.hpp>

namespace Atlas {
    class Redemption;
    class FixedRateCoupon;

    class EqualPaymentProduct : public FixedRateProduct {
       public:
        EqualPaymentProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate, QuantLib::Frequency freq, double notional,
                            const QuantLib::InterestRate& rate, bool recalcNotionals = false);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;

        void rate(const QuantLib::InterestRate& rate) override;

        void rate(double rate) override;

        QuantLib::InterestRate rate() const { return rate_; };

       private:
        void calculateRedemptions(const std::vector<QuantLib::Date>& dates, const QuantLib::InterestRate& rate, double nominal);

        bool recalcNotionals_;
        std::vector<QuantLib::Date> dates_;
    };

     class EqualPaymentInstrument : public FixedRateInstrument {
       public:
        EqualPaymentInstrument(const QuantLib::Date& startDate, const QuantLib::Date& endDate, QuantLib::Frequency freq, double notional,
                            const QuantLib::InterestRate& rate, bool recalcNotionals = false);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;

        void rate(const QuantLib::InterestRate& rate) override;

        void rate(double rate) override;

        QuantLib::InterestRate rate() const { return rate_; };

       private:
        void calculateRedemptions(const std::vector<QuantLib::Date>& dates, const QuantLib::InterestRate& rate, double nominal);

        bool recalcNotionals_;
        std::vector<QuantLib::Date> dates_;
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
