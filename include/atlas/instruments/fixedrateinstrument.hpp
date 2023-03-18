#ifndef A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D
#define A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D

#include <ql/interestrate.hpp>
#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    class FixedRateInstrument : public Instrument {
       public:
        FixedRateInstrument(const Date& startDate, const Date& endDate, const InterestRate& rate, double notional = 0.0,
                            const FixedRateLeg& leg = FixedRateLeg());

        virtual ~FixedRateInstrument(){};

        const FixedRateLeg& constLeg() const { return leg_; };

        FixedRateLeg& leg() { return leg_; };

        void calculateNotionals(const std::vector<Date>& dates, const InterestRate& rate);

        virtual void rate(const InterestRate& rate);

        virtual void rate(double rate);

        InterestRate rate() const { return rate_; };

        void discountCurveIdx(size_t idx) { leg_.discountCurveIdx(idx); }

        virtual void accept(Visitor& visitor) override;

        virtual void accept(ConstVisitor& visitor) const override;

        Cashflow disbursement() const { return disbursement_; };

       protected:
        void calculateFaceAmount();

        FixedRateLeg leg_;
        Cashflow disbursement_;
        InterestRate rate_;
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
