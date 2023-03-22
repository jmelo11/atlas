#ifndef A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D
#define A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D

#include <ql/interestrate.hpp>
#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    /**
     * @class FixedRateInstrument
     * @brief An class for fixed, single-legged, rate instruments.
     */
    class FixedRateInstrument : public Instrument {
       public:
        /**
         * @brief Construct a new Fixed Rate Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param rate rate of the instrument
         * @param notional notional of the instrument
         * @param leg leg of the instrument
         */
        FixedRateInstrument(const Date& startDate, const Date& endDate, const InterestRate& rate, double notional = 0.0,
                            const FixedRateLeg& leg = FixedRateLeg());

        virtual ~FixedRateInstrument(){};

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return const FixedRateLeg&
         */
        const FixedRateLeg& constLeg() const { return leg_; };

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return FixedRateLeg&
         */
        FixedRateLeg& leg() { return leg_; };

        /**
         * @brief calculates the instrument's notional based on the given redemptions
         *
         * @param dates dates of the instrument
         * @param rate  rate of the instrument
         */
        void calculateNotionals(const std::vector<Date>& dates, const InterestRate& rate);

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        virtual void rate(const InterestRate& rate);

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        virtual void rate(double rate);

        /**
         * @brief Returns the rate of the instrument.
         *
         * @return InterestRate
         */
        InterestRate rate() const { return rate_; };

        /**
         * @brief Sets the discount curve context of the instrument.
         *
         * @param context
         */
        void discountCurveContex(std::shared_ptr<CurveContext> context) { leg_.discountCurveContext(context); }

        /**
         * @brief Returns the discount curve context of the instrument.
         *
         * @return std::shared_ptr<CurveContext>
         */
        std::shared_ptr<CurveContext> discountCurveContext() const { return leg_.discountCurveContext(); }

        /**
         * @brief accepts a visitor.
         *
         * @param visitor
         */
        virtual void accept(Visitor& visitor) override;

        /**
         * @brief accepts a const visitor.
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
         * @param disbursement Cashflow to be set
         */
        void disbursement(const Cashflow& disbursement) { disbursement_ = disbursement; }

       protected:
        void calculateFaceAmount();

        FixedRateLeg leg_;
        InterestRate rate_;
        Cashflow disbursement_;
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
