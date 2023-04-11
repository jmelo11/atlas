#ifndef DE65C371_BC26_40B8_AB7C_8D9E18223281
#define DE65C371_BC26_40B8_AB7C_8D9E18223281

#include <atlas/atlasconfig.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    /**
     * @brief A class for single leg instruments.
     *
     * @tparam adouble number type
     * @tparam FirstLeg type of the first leg
     */
    template <typename adouble, class FirstLeg>
    class SingleLegInstrument : public Instrument<adouble> {
       public:
        /**
         * @brief Construct a new Single Leg Instrument object
         *
         * @param startDate start date
         * @param endDate end date
         * @param notional notional
         * @param leg leg
         */
        SingleLegInstrument(const Date& startDate, const Date& endDate, double notional, const FirstLeg& leg) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
            leg_             = leg;
        }

        inline const FirstLeg& leg() const { return leg_; }

        inline FirstLeg& leg() { return leg_; }

       private:
        FirstLeg leg_;
    };
}  // namespace Atlas

#endif /* DE65C371_BC26_40B8_AB7C_8D9E18223281 */
