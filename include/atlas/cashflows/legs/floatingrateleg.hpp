#ifndef C06DA827_18B3_4441_9F99_0EA936D79F04
#define C06DA827_18B3_4441_9F99_0EA936D79F04

#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/cashflows/legs/leg.hpp>

namespace Atlas {
    class FloatingRateLeg : public Leg {
       public:
        FloatingRateLeg() : Leg(){};

        FloatingRateLeg(std::vector<FloatingRateCoupon> coupons, std::vector<Redemption> redemptions) : Leg(redemptions), coupons_(coupons) {
            startDate_ = coupons_.at(0).startDate();
        };

        std::vector<FloatingRateCoupon>& coupons() { return coupons_; }

        const std::vector<FloatingRateCoupon>& constCoupons() const { return coupons_; }

        void addCoupon(FloatingRateCoupon& coupon) {
            coupons_.push_back(coupon);
            if (startDate() == Date() || startDate() > coupon.startDate()) { startDate_ = coupon.startDate(); }
        }

        void forecastCurve(const std::string& curve) { forecastCurve_ = curve; }

        const std::string& forecastCurve() const { return forecastCurve_; }

       private:
        std::vector<FloatingRateCoupon> coupons_;
        std::string forecastCurve_ = "undefined";
    };

    class FloatingRateLeg2 : public Leg2 {
       public:
        FloatingRateLeg2() : Leg2(){};

        FloatingRateLeg2(std::vector<FloatingRateCoupon2> coupons, std::vector<Redemption2> redemptions, bool sort = false)
        : Leg2(redemptions, sort), coupons_(coupons) {
            if (sort) sortCashflows(coupons_);
        };

        std::vector<FloatingRateCoupon2>& coupons() { return coupons_; }

        const std::vector<FloatingRateCoupon2>& constCoupons() const { return coupons_; }

        void addCoupon(FloatingRateCoupon2& coupon, bool sort = false) {
            coupons_.push_back(coupon);
            if (sort) sortCashflows(coupons_);
        }

        void sort() {
            sortCashflows(redemptions_);
            sortCashflows(coupons_);
        }

        void discountCurve(size_t idx) {
            for (auto& coupon : coupons_) { coupon.discountCurveIdx(idx); }
            for (auto& redemptions_ : redemptions_) { redemptions_.discountCurveIdx(idx); }
        }

        void forecastCurve(size_t idx) {
            for (auto& coupon : coupons_) { coupon.forecastCurveIdx(idx); }
        }

        void indexIdx(size_t idx) {
            for (auto& coupon : coupons_) { coupon.indexIdx(idx); }
        }

       private:
        std::vector<FloatingRateCoupon2> coupons_;
    };
}  // namespace Atlas

#endif /* C06DA827_18B3_4441_9F99_0EA936D79F04 */
