#include <atlas/cashflows/cashflowstreammixins.hpp>

using namespace Atlas;
int main() {
    using Cashflows = CashflowStream<double, RedemptionStreamMixin, DisbursementStreamMixin, FixedRateCouponStreamMixin, FloatingRateCouponStreamMixin>;
    Cashflows leg;

    Date startDate = Date(1, Month::January, 2019);
    Date endDate   = Date(1, Month::January, 2020);
    Cashflow cf(endDate, 100.0, Side::Recieve);
    leg.addRedemption(cf);
    leg.addDisbursement(cf);

    InterestRate rate(0.05, Actual360());
    leg.addFixedRateCoupon(FixedRateCoupon(startDate, endDate, 100.0, rate));
}