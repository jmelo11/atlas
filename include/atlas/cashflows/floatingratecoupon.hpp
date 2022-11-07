#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>
#include <atlas/curves/rateindex.hpp>
#include <atlas/data/marketdata.hpp>

namespace Atlas {

    class FloatingRateCoupon : public Coupon {
       public:
        FloatingRateCoupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                           double notional, double spread, RateIndex index);

        double spread() const { return spread_; }

        void spread(double spread) { spread_ = spread; }

        int fwdIdx() const { return fwdIdx_; }

        void fwdIdx(int idx) { fwdIdx_ = idx; }

        void fixRate(const MarketData& data) { fixing_ = data.fwds.at(fwdIdx()); }

        double accruedAmount(const QuantLib::Date& start, const QuantLib::Date& end) const override;

        QuantLib::DayCounter dayCounter() const override;

        bool hasOcurred(const QuantLib::Date& date) const override;

       private:
        double fixing_;
        double spread_;  // ptr?
        int fwdIdx_;     // add spread to forecasted fwd rate?
        RateIndex index_;
    };

}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
