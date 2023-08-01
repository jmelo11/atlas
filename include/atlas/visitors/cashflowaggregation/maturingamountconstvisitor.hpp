#ifndef A2B4A4CA_D3B1_4C49_A206_1AC976BF6F32
#define A2B4A4CA_D3B1_4C49_A206_1AC976BF6F32

#include <atlas/visitors/cashflowaggregation/cashflowaggregationconstvisitor.hpp>

namespace Atlas {

    template <typename adouble = double>
    class MaturingAmountConstVisitor : public CashflowAggregationConstVisitor<adouble> {
       public:
        MaturingAmountConstVisitor(const Date& refDate) : CashflowAggregationConstVisitor<adouble>(refDate, refDate) {
            this->couponInspector_ = [&](const Coupon<adouble>* coupon) {
                if (coupon->paymentDate() == this->startDate_)
                    return coupon->amount();
                else
                    return 0.0;
            };
            this->redemptionInspector_ = [&](const Redemption<adouble>* redemption) {
                if (redemption->paymentDate() == this->startDate_)
                    return redemption->amount();
                else
                    return 0.0;
            };
        };
    };
}  // namespace Atlas

#endif /* A2B4A4CA_D3B1_4C49_A206_1AC976BF6F32 */
