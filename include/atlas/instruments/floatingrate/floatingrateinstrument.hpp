#ifndef EA6DF8D1_F778_4FBF_9CDE_CD72C677D078
#define EA6DF8D1_F778_4FBF_9CDE_CD72C677D078

#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    /**
     * @defgroup FloatingRateInstruments Floating Rate Instruments
     * @ingroup Instruments
     */

    /**
     * @class FloatingRateInstrument
     * @brief An class for floating, single-legged, rate instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble>
    class FloatingRateInstrument : public Instrument<adouble>, public OneLegMixin<adouble, FloatingRateLeg<adouble>> {
       public:
        /**
         * @brief Construct a new Floating Rate Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param side side of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param leg leg of the instrument
         */
        FloatingRateInstrument(const Date& startDate, const Date& endDate, Side side = Side::Long, double notional = 0.0, adouble spread = 0.0,
                               const FloatingRateLeg<adouble>& leg = FloatingRateLeg<adouble>())
        : OneLegMixin<adouble, FloatingRateLeg<adouble>>(leg), spread_(spread) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->side_      = side;
            this->notional_  = notional;
        };

        virtual ~FloatingRateInstrument(){};

        /**
         * @brief Returns the spread of the instrument.
         *
         * @return double
         */
        inline adouble spread() const { return spread_; };

        /**
         * @brief Sets the spread of the instrument.
         *
         * @param s
         */
        inline void spread(adouble s) {
            spread_ = s;
            for (auto& coupon : this->leg().coupons()) { coupon.spread(s); }
        };

        /**
         * @brief  Accepts a visitor.
         *
         * @param visitor
         */
        virtual void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); };

        /**
         * @brief Accepts a const visitor.
         *
         * @param visitor
         */
        virtual void accept(ConstVisitor<adouble>& visitor) const override { visitor.visit(*this); };

       protected:
        adouble spread_;
    };
}  // namespace Atlas

#endif /* EA6DF8D1_F778_4FBF_9CDE_CD72C677D078 */
