#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/basictypes/rateindex.hpp>
#include <atlas/basictypes/store.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <iostream>

using namespace Atlas;

int main() {
    LIBOR12M index;

    Store<RateIndex> store;
    size_t indexPos = store.addItem(index);

    
    Date startDate(1, Month::Jan, 2018);
    Date endDate(1, Month::Jan, 2019);

    FloatingRateCoupon2 coupon(startDate, endDate, 100, 0.0);

    coupon.forecastCurveIdx(indexPos);

    std::vector<FloatingRateCoupon2> coupons;
    coupons.push_back(coupon);

    Redemption2 redemption(endDate, 100);

    std::vector<Redemption2> redemptions;

    redemptions.push_back(redemption);

    FloatingRateLeg2 leg(coupons, redemptions);

    return 0;
}