#ifndef FCBEE32E_AB43_4796_A9DB_9E6108A27EF5
#define FCBEE32E_AB43_4796_A9DB_9E6108A27EF5

#include <atlas/cashflows/cashflow.hpp>
#include <string>
#include <vector>

namespace Atlas {

    class Leg {
       public:
        Leg(std::vector<Cashflow> coupons, const std::string& discountCurve,
            const std::string& forecastCurve)
        : coupons_(coupons), discountCurve_(discountCurve), forecastCurve_(forecastCurve) {}
        void discountCurve(const std::string& curve) { discountCurve_ = curve; }

        const std::string& discountCurve() const { return discountCurve_; }

        void forecastCurve(const std::string& curve) { forecastCurve_ = curve; }

        const std::string& forecastCurve() const { return forecastCurve_; }

        void addCoupon(Cashflow c) { coupons.push_back(c); }

       private:
        std::vector<Cashflow> coupons_;
        std::string discountCurve_ = "undefined";
        std::string forecastCurve_ = "undefined";
    }

}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
