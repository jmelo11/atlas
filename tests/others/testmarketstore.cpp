#ifndef CDF79660_C142_4893_AE44_D9D945717DDB
#define CDF79660_C142_4893_AE44_D9D945717DDB

#include "../pch.hpp"
#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/marketstore.hpp>

using namespace Atlas;

TEST(Others, CreateCurveContext) {
    MarketStore store1_ = MarketStore();

    Date startDate(1, Month::Jan, 2020);
    FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    RateIndex index("LIBOR1M", Frequency::Monthly, Actual360());
    store1_.createCurveContext("LIBOR1M", curveStrategy, index);

    MarketStore store2_ = MarketStore();
    store2_.copyFromStore(store1_);

    EXPECT_TRUE(store2_.hasCurveContext("LIBOR1M"));

    auto& context  = store1_.curveContext("LIBOR1M");
    auto& context2 = store2_.curveContext("LIBOR1M");

    // check if different address
    EXPECT_NE(&context, &context2);

    // check if same curve
    EXPECT_NE(&context.curveStrategy(), &context2.curveStrategy());
}

#endif /* CDF79660_C142_4893_AE44_D9D945717DDB */
