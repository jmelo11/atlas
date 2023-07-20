#include "../testsetup.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/visitors/newvisitors/cashflowaggregation/accruedamountconstvisitor.hpp>
#include <atlas/visitors/newvisitors/fixingvisitor.hpp>
#include <atlas/visitors/newvisitors/indexingvisitor.hpp>

using namespace Atlas;

TEST(AccruedAmountConstVisitor, FixedRateInstrument) {
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;

    auto& coupon = instrument.leg().coupons().at(0);
    Date startDate(coupon.startDate());
    Date endDate(coupon.endDate());
    AccruedAmountConstVisitor<double> accruedAmountVisitor(startDate, endDate);

    accruedAmountVisitor(instrument);

    EXPECT_DOUBLE_EQ(accruedAmountVisitor.getResults(), coupon.accruedAmount(startDate, endDate));
}