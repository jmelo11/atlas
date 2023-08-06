#ifndef CA42F9F0_E48D_4774_8366_AE0752280DAC
#define CA42F9F0_E48D_4774_8366_AE0752280DAC

#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/others/auxfuncs.hpp>

namespace Atlas {
    /**
     * @class CustomFixedRateInstrument
     * @brief A class for custom fixed rate instruments.
     * @ingroup FixedRateInstruments
     */
    template <typename adouble = double>
    class CustomFixedRateInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         *
         * @param disbursements
         * @param redemptions
         * @param rate
         * @param side
         */
        CustomFixedRateInstrument(const std::map<Date, double>& disbursements, const std::map<Date, double>& redemptions,
                                  const InterestRate<adouble>& rate, Side side = Side::Recieve)
        : FixedRateInstrument<adouble>(disbursements.begin()->first, redemptions.end()->first, rate, side) {
            // if there is a redemption on the first date, throw an error
            Date firstDisbursementDate = disbursements.begin()->first;
            Date firstRedemptionDate   = redemptions.begin()->first;
            if (firstDisbursementDate > firstRedemptionDate) { throw std::runtime_error("First redemption date is before first disbursement date"); }

            // calculate implied notional & add cashflows
            double impliedNotional = 0.0;

            auto invSide = side == Side::Recieve ? Side::Pay : Side::Recieve;
            for (const auto& [date, amount] : disbursements) {
                impliedNotional += amount;
                this->cashflows_.addDisbursement(Cashflow<adouble>(date, amount, invSide));
            }
            this->notional_ = impliedNotional;
            for (const auto& [date, amount] : redemptions) {
                impliedNotional -= amount;
                this->cashflows_.addRedemption(Cashflow<adouble>(date, amount, side));
            }
            if (impliedNotional != 0.0) { throw std::runtime_error("Disbursments are not fully paid with the given redemptions"); }
            this->notional_ = impliedNotional;

            auto outstrading = calculateOutstanding(disbursements, redemptions);

            for (const auto& o : outstrading) {
                Date startDate   = std::get<0>(o);
                Date endDate     = std::get<1>(o);
                double notional_ = std::get<2>(o);
                this->cashflows_.addFixedRateCoupon(FixedRateCoupon<adouble>(startDate, endDate, notional_, rate, side));
            }
        }

        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         *
         * @param disbursements
         * @param redemptions
         * @param rate
         * @param discountContextIdx
         * @param side
         */
        CustomFixedRateInstrument(const std::map<Date, double>& disbursements, const std::map<Date, double>& redemptions,
                                  const InterestRate<adouble>& rate, size_t discountContextIdx, Side side = Side::Recieve)
        : CustomFixedRateInstrument(disbursements, redemptions, rate, side) {
            this->cashflows_.discountContextIdx(discountContextIdx);
        }
    };
}  // namespace Atlas

#endif /* CA42F9F0_E48D_4774_8366_AE0752280DAC */
