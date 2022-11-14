#ifndef FCBEE32E_AB43_4796_A9DB_9E6108A27EF5
#define FCBEE32E_AB43_4796_A9DB_9E6108A27EF5

#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <memory>
#include <string>
#include <vector>
namespace Atlas {

    class Leg {
       public:
        Leg(){};

        Leg(std::vector<Redemption> redemptions) : redemptions_(redemptions){};

        void discountCurve(const std::string& curve) { discountCurve_ = curve; }

        const std::string& discountCurve() const { return discountCurve_; }

        std::vector<Redemption>& redemptions() { return redemptions_; }

        const std::vector<Redemption>& constRedemptions() const { return redemptions_; }

        void addRedemption(Redemption& redemption) { redemptions_.push_back(redemption); }

       private:
        std::vector<Redemption> redemptions_;
        std::string discountCurve_ = "undefined";
    };

    class FixedRateLeg : public Leg {
       public:
        FixedRateLeg() : Leg(){};

        FixedRateLeg(std::vector<FixedRateCoupon> coupons, std::vector<Redemption> redemptions)
        : Leg(redemptions), coupons_(coupons){};

        std::vector<FixedRateCoupon>& coupons() { return coupons_; }

        const std::vector<FixedRateCoupon>& constCoupons() const { return coupons_; }

        void addCoupon(FixedRateCoupon& coupon) { coupons_.push_back(coupon); }

       private:
        std::vector<FixedRateCoupon> coupons_;
    };

    class FloatingRateLeg : public Leg {
       public:
        FloatingRateLeg() : Leg(){};

        FloatingRateLeg(std::vector<FloatingRateCoupon> coupons,
                        std::vector<Redemption> redemptions)
        : Leg(redemptions), coupons_(coupons){};

        std::vector<FloatingRateCoupon>& coupons() { return coupons_; }

        const std::vector<FloatingRateCoupon>& constCoupons() const { return coupons_; }

        void addCoupon(FloatingRateCoupon& coupon) { coupons_.push_back(coupon); }

        void forecastCurve(const std::string& curve) { forecastCurve_ = curve; }

        const std::string& forecastCurve() const { return forecastCurve_; }

       private:
        std::vector<FloatingRateCoupon> coupons_;
        std::string forecastCurve_ = "undefined";
    };

}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
