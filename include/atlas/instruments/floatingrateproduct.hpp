#ifndef EA6DF8D1_F778_4FBF_9CDE_CD72C677D078
#define EA6DF8D1_F778_4FBF_9CDE_CD72C677D078

#include <atlas/cashflows/leg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    class FloatingRateProduct : public Instrument {
       public:
        FloatingRateProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double notional = 0.0,
                            const FloatingRateLeg& leg = FloatingRateLeg());

        virtual ~FloatingRateProduct(){};

        const FloatingRateLeg& constLeg() const { return leg_; };

        FloatingRateLeg& leg() { return leg_; };

        double spread() const { return spread_; };

        void spread(double s);

        void forecastCurve(const std::string& curve) { leg_.forecastCurve(curve); }

        void discountCurve(const std::string& curve) { leg_.discountCurve(curve); }
       protected:
        FloatingRateLeg leg_;
        double spread_ = 0.0;
    };
}  // namespace Atlas

#endif /* EA6DF8D1_F778_4FBF_9CDE_CD72C677D078 */
