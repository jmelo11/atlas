#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>
#include <atlas/curves/rateindex.hpp>

namespace Atlas {

    class FloatingRateCoupon : public Coupon {
       public:
        FloatingRateCoupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double notional, double spread, const RateIndex& index);

        double spread() const { return spread_; }

        void spread(double spread) {
            spread_ = spread;
            amount_ = accruedAmount(startDate(), endDate());
        }

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

        bool isFloating() const { return isFloating_; }

        void fix() { isFloating_ = false; }

       private:
        double fixing_   = 0.0;
        double spread_   = 0.0;
        size_t fwdIdx_   = 0;
        bool isFloating_ = true;
        RateIndex index_;
    };

}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
