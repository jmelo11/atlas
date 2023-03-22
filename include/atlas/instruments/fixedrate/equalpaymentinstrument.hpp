#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {

    /**
     * @brief An class for equalk payment instruments.
     *
     */
    class EqualPaymentInstrument : public FixedRateInstrument {
       public:
        /**
         * @brief Construct a new Equal Payment Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param recalcNotionals recalculate notionals based on the given rate
         * @param discountCurveContext discount curve context of the instrument
         */
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate& rate,
                               bool recalcNotionals = false, std::shared_ptr<CurveContext> discountCurveContext = nullptr);

        void rate(const InterestRate& rate) override;

        void rate(double rate) override;

        InterestRate rate() const { return rate_; };

       private:
        void calculateRedemptions(const std::vector<Date>& dates, const InterestRate& rate, double nominal);

        bool recalcNotionals_;
        std::vector<Date> dates_;
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
