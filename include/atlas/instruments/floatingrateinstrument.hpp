#ifndef EA6DF8D1_F778_4FBF_9CDE_CD72C677D078
#define EA6DF8D1_F778_4FBF_9CDE_CD72C677D078

#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    /**
     * @class FloatingRateInstrument
     * @brief An class for floating, single-legged, rate instruments.
     */
    template <typename adouble>
    class FloatingRateInstrument : public Instrument<adouble> {
       public:
        /**
         * @brief Construct a new Floating Rate Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param leg leg of the instrument
         */
        FloatingRateInstrument(const Date& startDate, const Date& endDate, double notional = 0.0, adouble spread = 0.0,
                               const FloatingRateLeg<adouble>& leg = FloatingRateLeg<adouble>())
        : leg_(leg), spread_(spread) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
        };

        virtual ~FloatingRateInstrument(){};

        /**
         * @brief Returns the leg (const) of the instrument.
         *
         * @return const FloatingRateLeg&
         */
        inline const FloatingRateLeg<adouble>& leg() const { return leg_; };

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return FloatingRateLeg&
         */
        inline FloatingRateLeg<adouble>& leg() { return leg_; };

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
            for (auto& coupon : leg_.coupons()) { coupon.spread(s); }
        };

        /**
         * @brief Sets the discount curve context of the instrument.
         *
         * @param context
         */
        inline void discountCurveContex(const CurveContext& context) {
            leg_.discountCurveContext(context);
            disbursement_.discountCurveContext(context);
        }

        /**
         * @brief Sets the forecast curve context of the instrument.
         *
         * @param context
         */
        inline void forecastCurveContext(const CurveContext& context) { leg_.forecastCurveContext(context); }

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

        /**
         * @brief Returns the disbursement of the instrument.
         *
         * @return Cashflow
         */
        inline Cashflow<adouble>& disbursement() { return disbursement_; };

        /**
         * @brief Sets the disbursement of the instrument.
         *
         * @param disbursement
         */
        inline void disbursement(const Cashflow<adouble>& disbursement) { disbursement_ = disbursement; }

       protected:
        FloatingRateLeg<adouble> leg_;
        Cashflow<adouble> disbursement_;
        adouble spread_;
    };
}  // namespace Atlas

#endif /* EA6DF8D1_F778_4FBF_9CDE_CD72C677D078 */
