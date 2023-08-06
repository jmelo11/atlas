#ifndef A2B4A4CA_D3B1_4C49_A206_1AC976BF6F32
#define A2B4A4CA_D3B1_4C49_A206_1AC976BF6F32

#include <atlas/visitors/cashflowaggregation/cashflowaggregationconstvisitor.hpp>

namespace Atlas {

    template <typename adouble = double>
    class MaturingAmountConstVisitor : public CashflowAggregationConstVisitor<adouble> {
       public:
        enum class Mode { OnlyInterests, OnlyRedemptions, OnlyDisbursements, InterestAndRedemption, All };

        MaturingAmountConstVisitor(const Date& refDate, Mode mode = Mode::All)
        : CashflowAggregationConstVisitor<adouble>(refDate, refDate), mode_(mode) {
            this->couponInspector_ = [&](const Coupon<adouble>* coupon) {
                if (coupon->paymentDate() == this->startDate_)
                    return coupon->amount();
                else
                    return 0.0;
            };
            this->cashflowInspector_ = [&](const Cashflow<adouble>* cashflow) {
                if (cashflow->paymentDate() == this->startDate_)
                    return cashflow->amount();
                else
                    return 0.0;
            };
        };

       private:
        Mode mode_;
    };
}  // namespace Atlas

#endif /* A2B4A4CA_D3B1_4C49_A206_1AC976BF6F32 */
