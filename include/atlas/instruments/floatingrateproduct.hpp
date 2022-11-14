#ifndef EA6DF8D1_F778_4FBF_9CDE_CD72C677D078
#define EA6DF8D1_F778_4FBF_9CDE_CD72C677D078

#include <atlas/cashflows/leg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    class FloatingRateProduct : public Instrument {
       public:
        FloatingRateProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                            const FloatingRateLeg& leg = FloatingRateLeg());

        const FloatingRateLeg& constLeg() const { return leg_; };

        FloatingRateLeg& leg() { return leg_; };

       protected:
        FloatingRateLeg leg_;
    };
}  // namespace Atlas

#endif /* EA6DF8D1_F778_4FBF_9CDE_CD72C677D078 */
