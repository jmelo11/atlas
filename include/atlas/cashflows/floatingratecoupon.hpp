#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>
#include <atlas/curves/rateindex.hpp>

namespace Atlas {

    class FloatingRateCoupon : public Coupon {
       public:
        FloatingRateCoupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                           double notional, double spread, const RateIndex& index);

        double spread() const { return spread_; }

        void spread(double spread) { spread_ = spread; }

        size_t fwdIdx() const { return fwdIdx_; }

        void fwdIdx(size_t idx) { fwdIdx_ = idx; }

        void fixing(double value) {
            fixing_ = value;
            amount_ = accruedAmount(startDate(), endDate());
        }

        double fixing() const { return fixing_; }

        double accruedAmount(const QuantLib::Date& start, const QuantLib::Date& end) const override;

        QuantLib::DayCounter dayCounter() const override;

        const RateIndex& index() const { return index_; }
        
       private:
        double fixing_;
        double spread_;  // ptr?
        size_t fwdIdx_;  // add spread to forecasted fwd rate?
        RateIndex index_;
    };

}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
