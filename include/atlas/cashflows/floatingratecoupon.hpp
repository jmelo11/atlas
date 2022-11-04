#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    class FloatingRateCoupon : public Coupon {
       public:
        FloatingRateCoupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                           double notional, double spread);

        double spread() const { return spread_; }

        void spread(double spread) { spread_ = spread; }

        int fwdIdx() const { return fwdIdx_; }

        void fwdIdx(int idx) { fwdIdx_ = idx; }

       private:
        double spread_;  // ptr?
        int fwdIdx_;     // add spread to forecasted fwd rate?
    };

}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
