#ifndef EA6DF8D1_F778_4FBF_9CDE_CD72C677D078
#define EA6DF8D1_F778_4FBF_9CDE_CD72C677D078

#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    /**
     * @class FloatingRateInstrument
     * @brief An class for floating, single-legged, rate instruments.
     */
    class FloatingRateInstrument : public Instrument {
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
        FloatingRateInstrument(const Date& startDate, const Date& endDate, double notional = 0.0, double spread = 0.0,
                               const FloatingRateLeg& leg = FloatingRateLeg());

        virtual ~FloatingRateInstrument(){};

        /**
         * @brief Returns the leg (const) of the instrument.
         *
         * @return const FloatingRateLeg&
         */
        const FloatingRateLeg& constLeg() const { return leg_; };

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return FloatingRateLeg&
         */
        FloatingRateLeg& leg() { return leg_; };

        /**
         * @brief Returns the spread of the instrument.
         *
         * @return double
         */
        double spread() const { return spread_; };

        /**
         * @brief Sets the spread of the instrument.
         *
         * @param s
         */
        void spread(double s);

        /**
         * @brief Sets the discount curve context of the instrument.
         *
         * @param context
         */
        void discountCurveContex(const CurveContext& context) { leg_.discountCurveContext(context); }

        /**
         * @brief Sets the forecast curve context of the instrument.
         *
         * @param context
         */
        void forecastCurveContext(const CurveContext& context) { leg_.forecastCurveContext(context); }

        /**
         * @brief  Accepts a visitor.
         *
         * @param visitor
         */
        virtual void accept(Visitor& visitor) override;

        /**
         * @brief Accepts a const visitor.
         *
         * @param visitor
         */
        virtual void accept(ConstVisitor& visitor) const override;

        /**
         * @brief Returns the disbursement of the instrument.
         *
         * @return Cashflow
         */
        Cashflow disbursement() const { return disbursement_; };

        /**
         * @brief Sets the disbursement of the instrument.
         *
         * @param disbursement
         */
        void disbursement(const Cashflow& disbursement) { disbursement_ = disbursement; }

       protected:
        FloatingRateLeg leg_;
        Cashflow disbursement_;
        double spread_;
    };
}  // namespace Atlas

#endif /* EA6DF8D1_F778_4FBF_9CDE_CD72C677D078 */
