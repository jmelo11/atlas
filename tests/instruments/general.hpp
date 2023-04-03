#ifndef A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B
#define A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B

#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/rates/curvecontextstore.hpp>

using namespace Atlas;

enum PaymentStructure { BulletOrZero, EqualRedemptions };

template <typename adouble>
struct FixedInstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    adouble rateValue          = 0.03;
    InterestRate<adouble> rate = InterestRate<adouble>(rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
    CurveContextStore& store_  = CurveContextStore::instance();
    FixedInstrumentVars() {
        if (!store_.hasContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

template <typename adouble>
struct FloatingInstrumentVars {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    adouble spread             = 0.01;
    CurveContextStore& store_  = CurveContextStore::instance();
    FloatingInstrumentVars() {
        if (!store_.hasContext("TEST")) {
            FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
            RateIndex index("TEST", Frequency::Annual, Actual360());
            store_.createCurveContext("TEST", curveStrategy, index);
        }
    };
};

template <class T, typename adouble>
void inline testStructure(const T& instrument, const Schedule& schedule, PaymentStructure structure) {
    const auto& leg         = instrument.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    const auto& dates = schedule.dates();

    // test that the number of coupons is equal to the number of dates - 1
    EXPECT_EQ(coupons.size(), dates.size() - 1);

    switch (structure) {
        case PaymentStructure::BulletOrZero:
            EXPECT_EQ(redemptions.size(), 1);
            if constexpr (std::is_same_v<adouble, double>) {
                EXPECT_EQ(redemptions.front().amount(), instrument.notional());
            } else {
                EXPECT_EQ(redemptions.front().amount().val, instrument.notional());
            }

            break;
        case PaymentStructure::EqualRedemptions:
            EXPECT_EQ(redemptions.size(), coupons.size());
            for (const auto& redemption : redemptions) {
                if constexpr (std::is_same_v<adouble, double>) {
                    EXPECT_EQ(redemption.amount(), instrument.notional() / coupons.size());
                } else {
                    EXPECT_EQ(redemption.amount().val, instrument.notional() / coupons.size());
                }
            }
            break;
        default:
            break;
    }
};

template <typename adouble>
void inline testInterest(const FixedRateInstrument<adouble>& instrument, const Schedule& schedule, const InterestRate<adouble>& rate) {
    const auto& leg         = instrument.constLeg();
    const auto& coupons     = leg.constCoupons();
    const auto& redemptions = leg.constRedemptions();

    const auto& dates = schedule.dates();

    size_t i = 0;
    for (const FixedRateCoupon<adouble>& coupon : coupons) {
        if constexpr (std::is_same_v<adouble, double>) {
            EXPECT_EQ(coupon.amount(), coupon.notional() * (rate.compoundFactor(dates.at(i), dates.at(i + 1)) - 1));
        } else {
            EXPECT_EQ(coupon.amount().val, coupon.notional() * (rate.compoundFactor(dates.at(i), dates.at(i + 1)).val - 1));
        }
        EXPECT_EQ(coupon.startDate(), dates.at(i));
        EXPECT_EQ(coupon.endDate(), dates.at(i + 1));
        EXPECT_EQ(coupon.rate().rate(), rate.rate());

        ++i;
    }
};

#endif /* A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B */
