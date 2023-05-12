#ifndef A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B
#define A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B

#include "../testsetup.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;

enum PaymentStructure { BulletOrZero, EqualRedemptions };

template <class T, typename adouble>
void inline testStructure(const T& instrument, const Schedule& schedule, PaymentStructure structure) {
    const auto& leg         = instrument.leg();
    const auto& coupons     = leg.coupons();
    const auto& redemptions = leg.redemptions();

    const auto& dates = schedule.dates();

    // test that the number of coupons is equal to the number of dates - 1
    EXPECT_EQ(coupons.size(), dates.size() - 1);

    switch (structure) {
        case PaymentStructure::BulletOrZero:
            EXPECT_EQ(redemptions.size(), 1);
            if constexpr (std::is_same_v<adouble, double>) {
                EXPECT_EQ(redemptions.front().amount(), instrument.notional());
            } else {
                EXPECT_EQ(val(redemptions.front().amount()), instrument.notional());
            }

            break;
        case PaymentStructure::EqualRedemptions:
            EXPECT_EQ(redemptions.size(), coupons.size());
            for (const auto& redemption : redemptions) {
                if constexpr (std::is_same_v<adouble, double>) {
                    EXPECT_EQ(redemption.amount(), instrument.notional() / coupons.size());
                } else {
                    EXPECT_EQ(val(redemption.amount()), instrument.notional() / coupons.size());
                }
            }
            break;
        default:
            break;
    }
};

template <typename adouble>
void inline testInterest(const FixedRateInstrument<adouble>& instrument, const Schedule& schedule, const InterestRate<adouble>& rate) {
    const auto& leg         = instrument.leg();
    const auto& coupons     = leg.coupons();
    const auto& redemptions = leg.redemptions();

    const auto& dates = schedule.dates();

    size_t i = 0;
    for (const FixedRateCoupon<adouble>& coupon : coupons) {
        if constexpr (std::is_same_v<adouble, double>) {
            EXPECT_EQ(coupon.amount(), coupon.notional() * (rate.compoundFactor(dates.at(i), dates.at(i + 1)) - 1));
        } else {
            EXPECT_EQ(val(coupon.amount()), coupon.notional() * (val(rate.compoundFactor(dates.at(i), dates.at(i + 1))) - 1));
        }
        EXPECT_EQ(coupon.startDate(), dates.at(i));
        EXPECT_EQ(coupon.endDate(), dates.at(i + 1));
        EXPECT_EQ(coupon.rate().rate(), rate.rate());

        ++i;
    }
};

#endif /* A48CDCC5_BB81_4A85_AA59_E7D536EFAC4B */
