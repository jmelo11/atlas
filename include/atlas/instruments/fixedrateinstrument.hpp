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
    template <typename adouble>
    class FixedRateInstrument : public Instrument<adouble> {
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
        FixedRateInstrument(const Date& startDate, const Date& endDate, const InterestRate<adouble>& rate, double notional = 0.0,
                            const FixedRateLeg<adouble>& leg = FixedRateLeg<adouble>())
        : leg_(leg), rate_(rate) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
        };

        virtual ~FixedRateInstrument(){};

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return const FixedRateLeg&
         */
        inline const FixedRateLeg<adouble>& constLeg() const { return leg_; };

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return FixedRateLeg&
         */
        inline FixedRateLeg<adouble>& leg() { return leg_; };

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        virtual void rate(const InterestRate<adouble>& r) {
            rate_ = r;
            for (auto& coupon : leg_.coupons()) { coupon.rate(r); }
        };

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        virtual void rate(adouble r) {
            InterestRate tmpR(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
            rate(tmpR);
        };

        /**
         * @brief Returns the rate of the instrument.
         *
         * @return InterestRate
         */
        InterestRate<adouble> rate() const { return rate_; };

        /**
         * @brief Sets the discount curve context of the instrument.
         *
         * @param context
         */
        inline void discountCurveContex(const CurveContext& context) { leg_.discountCurveContext(context); }

        /**
         * @brief accepts a visitor.
         *
         * @param visitor
         */
        virtual void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); };

        /**
         * @brief accepts a const visitor.
         *
         * @param visitor
         */
        virtual void accept(ConstVisitor<adouble>& visitor) const override { visitor.visit(*this); };

        /**
         * @brief Returns the disbursement of the instrument.
         *
         * @return Cashflow
         */
        inline Cashflow<adouble> disbursement() const { return disbursement_; };

        /**
         * @brief Sets the disbursement of the instrument.
         *
         * @param disbursement Cashflow to be set
         */
        inline void disbursement(const Cashflow<adouble>& disbursement) { disbursement_ = disbursement; }

       protected:
        /**
         * @brief calculates the instrument's notional based on the given redemptions
         *
         * @param dates dates of the instrument
         * @param rate  rate of the instrument
         */
        void calculateNotionals(const std::vector<Date>& dates, const InterestRate<adouble>& rate) {
            std::map<Date, double> notionals;
            double notional = 0.0;
            for (const auto& redemption : leg_.redemptions()) {
                double redemptionAmount = redemption.amount().val;
                notional += redemptionAmount;
                notionals[redemption.paymentDate()] = redemptionAmount;
            }
            this->notional_ = notional;
            for (size_t i = 0; i < dates.size() - 1; i++) {
                FixedRateCoupon<adouble> coupon(dates[i], dates[i + 1], notional, rate);
                leg_.addCoupon(coupon);
                if (notionals.find(dates[i + 1]) != notionals.end()) notional -= notionals[dates[i + 1]];
            }
        };

        FixedRateLeg<adouble> leg_;
        InterestRate<adouble> rate_;
        Cashflow<adouble> disbursement_;
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
