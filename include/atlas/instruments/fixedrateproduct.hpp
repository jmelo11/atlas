#ifndef A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D
#define A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D

#include <ql/interestrate.hpp>
#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {
    class FixedRateCoupon;
    class Redemption;

    // general porpuse class
    class FixedRateProduct : public Instrument {
       public:
        FixedRateProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate, const QuantLib::InterestRate& rate, double notional = 0.0,
                         const FixedRateLeg& leg = FixedRateLeg());

        virtual ~FixedRateProduct(){};

        const FixedRateLeg& constLeg() const { return leg_; };

        FixedRateLeg& leg() { return leg_; };

        void calculateNotionals(const std::vector<QuantLib::Date>& dates, const QuantLib::InterestRate& rate);

        virtual void rate(const QuantLib::InterestRate& rate);

        virtual void rate(double rate);

        QuantLib::InterestRate rate() const { return rate_; };

        void discountCurve(const std::string& curve) { leg_.discountCurve(curve); }

       protected:
        void calculateFaceAmount();

        FixedRateLeg leg_;
        QuantLib::InterestRate rate_;
    };

    class FixedRateInstrument : public Instrument {
       public:
        FixedRateInstrument(const QuantLib::Date& startDate, const QuantLib::Date& endDate, const QuantLib::InterestRate& rate, double notional = 0.0,
                            const FixedRateLeg2& leg = FixedRateLeg2());

        virtual ~FixedRateInstrument(){};

        const FixedRateLeg2& constLeg() const { return leg_; };

        FixedRateLeg2& leg() { return leg_; };

        void calculateNotionals(const std::vector<QuantLib::Date>& dates, const QuantLib::InterestRate& rate);

        virtual void rate(const QuantLib::InterestRate& rate);

        virtual void rate(double rate);

        QuantLib::InterestRate rate() const { return rate_; };

        void discountCurveIdx(size_t idx) { leg_.discountCurveIdx(idx); }

        virtual void accept(Visitor& visitor) override;

        virtual void accept(ConstVisitor& visitor) const override;

       protected:
        void calculateFaceAmount();

        FixedRateLeg2 leg_;
        QuantLib::InterestRate rate_;
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
