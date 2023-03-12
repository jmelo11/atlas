#ifndef FCBEE32E_AB43_4796_A9DB_9E6108A27EF5
#define FCBEE32E_AB43_4796_A9DB_9E6108A27EF5

#include <atlas/cashflows/redemption.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Atlas {

    class Leg : public Indexable {
       public:
        Leg(){};

        Leg(std::vector<Redemption> redemptions) : redemptions_(redemptions){};

        virtual ~Leg(){};

        void discountCurve(const std::string& curve) { discountCurve_ = curve; }

        const std::string& discountCurve() const { return discountCurve_; }

        std::vector<Redemption>& redemptions() { return redemptions_; }

        const std::vector<Redemption>& constRedemptions() const { return redemptions_; }

        void addRedemption(Redemption& redemption) { redemptions_.push_back(redemption); }

        Date startDate() const { return startDate_; }

       protected:
        Date startDate_ = Date();

       private:
        std::vector<Redemption> redemptions_;
        std::string discountCurve_ = "undefined";
        size_t dfIdx_              = 0;  // this should be changed for the numeraire
    };

    /***
     * Leg2 is a new version of Leg that uses Redemption2 instead of Redemption.
     */
    class Leg2 {
       public:
        Leg2(){};

        // copy coupons?
        Leg2(std::vector<Redemption2> redemptions, bool sort = false) : redemptions_(redemptions) {
            if (sort) sortCashflows(redemptions_);
        };

        virtual ~Leg2(){};

        std::vector<Redemption2>& redemptions() { return redemptions_; }

        const std::vector<Redemption2>& constRedemptions() const { return redemptions_; }

        void addRedemption(Redemption2& redemption) { redemptions_.push_back(redemption); }

        void sortRedemptions() { sortCashflows(redemptions_); }

       protected:
        template <typename C>
        void sortCashflows(std::vector<C>& cashflows) {
            auto f = [](const C& lhs, const C& rhs) { return lhs.paymentDate() < rhs.paymentDate(); };
            std::sort(cashflows.begin(), cashflows.end(), f);
        }

        template <typename C>
        void setDiscountCurveIdx(std::vector<C>& cashflows, size_t curveIdx) {
            for (auto& c : cashflows) { c.discountCurveIdx(curveIdx); }
        }

        std::vector<Redemption2> redemptions_;
    };
}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
