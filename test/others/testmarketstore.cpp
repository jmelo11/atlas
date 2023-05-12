#ifndef CDF79660_C142_4893_AE44_D9D945717DDB
#define CDF79660_C142_4893_AE44_D9D945717DDB

#include <gtest/gtest.h>
#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;

TEST(Others, CreateCurveContext) {
    Date startDate(1, Month::Jan, 2020);
    MarketStore<double> store1_(startDate);

    FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    YieldTermStructure<double> curve(std::make_unique<FlatForwardStrategy<double>>(curveStrategy));
    RateIndex<double> index(startDate, Frequency::Monthly, Actual360());
    store1_.addCurve("LIBOR1M", curve, index);

    MarketStore<double> store2_(startDate);
    store2_.cloneFromStore(store1_);

    // EXPECT_TRUE(store2_.hasCurveContext("LIBOR1M"));

    auto& context  = store1_.curveContext("LIBOR1M");
    auto& context2 = store2_.curveContext("LIBOR1M");

    // check if different address
    EXPECT_NE(&context, &context2);

    // check if same curve
    EXPECT_NE(&context.object(), &context2.object());
}

#endif /* CDF79660_C142_4893_AE44_D9D945717DDB */
