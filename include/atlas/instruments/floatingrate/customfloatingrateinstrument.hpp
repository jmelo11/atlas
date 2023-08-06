#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/others/auxfuncs.hpp>
#include <atlas/rates/index/interestrateindex.hpp>

namespace Atlas {
    /**
     * @class CustomFloatingRateInstrument
     * @brief A class for custom floating rate instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble = double>
    class CustomFloatingRateInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param index interest rate index of the instrument
         */
        CustomFloatingRateInstrument(const std::map<Date, double>& disbursements, const std::map<Date, double>& redemptions, adouble spread,
                                     const InterestRateIndex<adouble>& index, Side side = Side::Recieve)
        : FloatingRateInstrument<adouble>(disbursements.begin()->first, redemptions.begin()->first, 0, spread, side) {
            Date firstDisbursementDate = disbursements.begin()->first;
            Date firstRedemptionDate   = redemptions.begin()->first;
            if (firstDisbursementDate > firstRedemptionDate) { throw std::runtime_error("First redemption date is before first disbursement date"); }

            auto invSide = side == Side::Recieve ? Side::Pay : Side::Recieve;
            // calculate implied notional & add cashflows
            double impliedNotional = 0.0;
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

            // calculate the outstanding notional and create the correponding fixed rate coupons
            auto outstrading = calculateOutstanding(disbursements, redemptions);

            for (const auto& o : outstrading) {
                Date startDate   = std::get<0>(o);
                Date endDate     = std::get<1>(o);
                double notional_ = std::get<2>(o);
                this->cashflows_.addFloatingRateCoupon(
                    FloatingRateCoupon<adouble>(startDate, endDate, notional_, spread, index.rateDefinition(), side));
            }
        }

        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param index interest rate index of the instrument
         * @param discountContextIdx index of the discount curve context of the instrument
         * @param indexContextIdx index of the interest rate index context of the instrument
         */
        CustomFloatingRateInstrument(const std::map<Date, double>& disbursements, const std::map<Date, double>& redemptions, adouble spread,
                                     const InterestRateIndex<adouble>& index, size_t discountContextIdx, size_t indexContextIdx,
                                     Side side = Side::Recieve)
        : CustomFloatingRateInstrument(disbursements, redemptions, spread, index, side) {
            this->cashflows_.discountContextIdx(discountContextIdx);
            this->cashflows_.indexContextIdx(indexContextIdx);
        };
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
