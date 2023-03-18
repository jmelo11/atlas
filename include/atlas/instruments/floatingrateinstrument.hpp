#ifndef EA6DF8D1_F778_4FBF_9CDE_CD72C677D078
#define EA6DF8D1_F778_4FBF_9CDE_CD72C677D078

#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    class FloatingRateInstrument : public Instrument {
       public:
        FloatingRateInstrument(const Date& startDate, const Date& endDate, double notional = 0.0, double spread = 0.0,
                               const FloatingRateLeg& leg = FloatingRateLeg());

        virtual ~FloatingRateInstrument(){};

        const FloatingRateLeg& constLeg() const { return leg_; };

        FloatingRateLeg& leg() { return leg_; };

        double spread() const { return spread_; };

        void spread(double s);

        void discountCurveIdx(size_t idx) { leg_.discountCurveIdx(idx); }

        void forecastCurveIdx(size_t idx) { leg_.forecastCurveIdx(idx); }

        virtual void accept(Visitor& visitor) override;

        virtual void accept(ConstVisitor& visitor) const override;

        Cashflow disbursement() const { return disbursement_; };

       protected:
        FloatingRateLeg leg_;
        Cashflow disbursement_;
        double spread_;
    };
}  // namespace Atlas

#endif /* EA6DF8D1_F778_4FBF_9CDE_CD72C677D078 */
