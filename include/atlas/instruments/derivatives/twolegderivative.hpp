#ifndef D5FE712D_E293_4049_9A71_A2C0C44665A8
#define D5FE712D_E293_4049_9A71_A2C0C44665A8

#include <atlas/atlasconfig.hpp>

namespace Atlas {
    template <typename adouble, class FirstLeg, class SecondLeg>
    class TwoLegDerivative : public Instrument<adouble> {
       public:
        TwoLegDerivative(const Date& startDate, const Date& endDate, double notional, Side side) : side_(side) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
        }

        const FirstLeg& firstLeg() const { return firstLeg_; }

        FirstLeg& firstLeg() { return firstLeg_; }

        const SecondLeg& secondLeg() const { return secondLeg_; }

        SecondLeg& secondLeg() { return secondLeg_; }

       private:
        Side side_;
        FirstLeg firstLeg_;
        SecondLeg secondLeg_;
    };
}  // namespace Atlas

#endif /* D5FE712D_E293_4049_9A71_A2C0C44665A8 */
