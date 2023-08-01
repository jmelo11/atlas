#ifndef CDF79660_C142_4893_AE44_D9D945717DDB
#define CDF79660_C142_4893_AE44_D9D945717DDB

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

TEST(Others, CreateCurveContext) {
    Date startDate(1, Month::Jan, 2020);
    MarketStore<double> store1_(startDate);
    YieldTermStructure<double> curve = FlatForwardTermStructure<double>(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    store1_.curveManager().addCurveContext("LIBOR1M", curve);

    MarketStore<double> store2_ = store1_;

    // EXPECT_TRUE(store2_.hasCurveContext("LIBOR1M"));

    auto& context  = store1_.curveManager().curveContext("LIBOR1M");
    auto& context2 = store2_.curveManager().curveContext("LIBOR1M");

    // check if different address
    EXPECT_NE(&context, &context2);

    // check if same curve
    EXPECT_NE(&context.curve(), &context2.curve());
    EXPECT_NE(&context.index(), &context2.index());
}

#endif /* CDF79660_C142_4893_AE44_D9D945717DDB */
