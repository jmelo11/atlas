#ifndef CD1A5552_BB39_4186_9CDA_3A1D38BE4A2B
#define CD1A5552_BB39_4186_9CDA_3A1D38BE4A2B

#include <atlas/visitors/newvisitors/cashflowaggregation/cashflowaggregationconstvisitor.hpp>

namespace Atlas {

    template <typename adouble = double>
    class AccruedAmountConstVisitor : public CashflowAggregationConstVisitor<adouble> {
       public:
        AccruedAmountConstVisitor(const Date& startDate, const Date& endDate) : CashflowAggregationConstVisitor<adouble>(startDate, endDate) {
            this->couponInspector_     = [&](const Coupon<adouble>* coupon) { return coupon->accruedAmount(this->startDate_, this->endDate_); };
            this->redemptionInspector_ = [&](const Redemption<adouble>* redemption) { return 0.0; };
        };
    };
}  // namespace Atlas

#endif /* CD1A5552_BB39_4186_9CDA_3A1D38BE4A2B */
