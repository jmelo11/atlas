#ifndef D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE
#define D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE

#include <ql/interestrate.hpp>
#include <atlas/cashflows/legs/makeleg.hpp>
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
                                  Side side = Side::Long)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional) {
            this->leg_ = MakeLeg<FixedRateLeg, adouble>()
                             .startDate(this->startDate_)
                             .endDate(this->endDate_)
                             .notional(this->notional_)
                             .rate(this->rate_)
                             .side(this->side_)
                             .paymentFrequency(freq)
                             .build();

            int flag             = (this->side_ == Side::Long) ? 1 : -1;
            adouble disbursement = -this->notional_ * flag;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
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
                                  size_t discountContextIdx, Side side = Side::Long)
        : FixedRateBulletInstrument(startDate, endDate, freq, notional, rate, side) {
            this->leg().discountContextIdx(discountContextIdx);
            this->disbursement().discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE */
