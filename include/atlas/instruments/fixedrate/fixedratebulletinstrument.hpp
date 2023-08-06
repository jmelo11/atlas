#ifndef D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE
#define D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @class FixedRateBulletInstrument
     * @brief A class for fixed, single-legged, bullet instruments.
     * @ingroup FixedRateInstruments
     */
    template <typename adouble = double>
    class FixedRateBulletInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Fixed Rate Bullet Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param side side of the instrument
         */
        FixedRateBulletInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                                  Side side = Side::Recieve)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional) {
    
            Schedule schedule       = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
            std::vector<Date> dates = schedule.dates();

            auto invSide = side == Side::Recieve ? Side::Pay : Side::Recieve;
            this->cashflows_.addDisbursement(Cashflow<adouble>(startDate, notional, invSide));
            this->cashflows_.addRedemption(Cashflow<adouble>(endDate, notional, side));
            for (size_t i = 1; i < dates.size(); i++) {
                this->cashflows_.addFixedRateCoupon(FixedRateCoupon<adouble>(dates.at(i - 1), dates.at(i), notional, rate, side));
            }
        };

        /**
         * @brief Construct a new Fixed Rate Bullet Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountContextIdx discount curve context of the instrument
         * @param side side of the instrument
         */
        FixedRateBulletInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                                  size_t discountContextIdx, Side side = Side::Recieve)
        : FixedRateBulletInstrument(startDate, endDate, freq, notional, rate, side) {
             this->cashflows_.discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE */
