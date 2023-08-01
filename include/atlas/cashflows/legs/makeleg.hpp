#ifndef F41148D8_C863_45B8_B7B2_E88435ADFC89
#define F41148D8_C863_45B8_B7B2_E88435ADFC89

#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/rates/index/interestrateindex.hpp>
#include <numeric>

namespace Atlas {

    /**
     * @class MakeLeg
     * @brief Helper class to build legs with different types of coupons and parameters.
     * @ingroup Legs
     *
     * @tparam adouble
     * @tparam LegType
     */
    template <template <typename> class LegType, typename adouble = double>
    class MakeLeg {
       public:
        MakeLeg() = default;

        /**
         * @brief Sets the notional on which the leg is calculated.
         * @details Notional must be positive.
         * @param notional
         * @return MakeLeg&
         */
        MakeLeg& notional(double notional) {
            if (notional <= 0) { throw std::invalid_argument("Notional must be greather that 0."); }
            notional_ = notional;
            return *this;
        }

        MakeLeg& paymentFrequency(Frequency frequency) {
            paymentFrequency_ = frequency;
            return *this;
        }

        MakeLeg& startDate(const Date& startDate) {
            startDate_ = startDate;
            return *this;
        }

        MakeLeg& endDate(const Date& endDate) {
            endDate_ = endDate;
            return *this;
        }

        /**
         * @brief Set the dates vector.
         * @details If dates vector is set, startDate and endDate are ignored and not schedule is calculated.
         * @param dates
         * @return MakeLeg&
         */
        MakeLeg& dates(const std::vector<Date>& dates) {
            dates_ = dates;
            return *this;
        }

        MakeLeg& currency(const Currency& currency) {
            currency_ = currency;
            return *this;
        }

        MakeLeg& discountContextIdx(size_t idx) {
            discountContextIdx_ = idx;
            return *this;
        }

        MakeLeg& paymentConvention(BusinessDayConvention paymentConvention) {
            paymentConvention_ = paymentConvention;
            return *this;
        }

        /**
         * @brief Set the redemptions vector.
         * @details Positive cashflows are recieved and negative cashflows are paid. Side does not change the sign of the cashflows, therefore
         * the redemptions vector should be positive for long positions and negative for short positions.
         *
         * @param redemptions
         * @return MakeLeg&
         */
        MakeLeg& redemptions(const std::vector<adouble>& redemptions) {
            redemptions_ = redemptions;
            return *this;
        }

        MakeLeg& calendar(const Calendar& calendar) {
            calendar_ = calendar;
            return *this;
        }

        /**
         * @brief Rate of the coupon. Only applies to fixed rate coupons.
         *
         * @param rate
         * @return MakeLeg&
         */
        MakeLeg& rate(const InterestRate<adouble>& rate) {
            static_assert(std::is_same_v<LegType<adouble>, FixedRateLeg<adouble>>, "Only FixedRateLeg is supported for rate.");
            rate_ = rate;
            return *this;
        }

        /**
         * @brief Spread to be added to the index fixing. Only applies to floating rate coupons.
         *
         * @param spread
         * @return MakeLeg&
         */
        MakeLeg& spread(adouble spread) {
            static_assert(std::is_same_v<LegType<adouble>, FloatingRateLeg<adouble>>, "Only FloatingRateLeg is supported for spread.");
            spread_ = spread;
            return *this;
        }

        MakeLeg& indexContextIdx(size_t idx) {
            indexContextIdx_ = idx;
            return *this;
        }

        MakeLeg& interestRateIndex(const InterestRateIndex<adouble>& index) {
            paymentFrequency_ = index.fixingFrequency();
            rateDef_          = index.rateDefinition();
            return *this;
        }

        /**
         * @brief Set the side of the cashflow. If a redemption vector is not given, this will change the sign of the payment.
         *
         * @param side
         * @return MakeLeg&
         */
        MakeLeg& side(Side side) {
            switch (side) {
                case Side::Long:
                    side_ = 1;
                    break;

                case Side::Short:
                    side_ = -1;
                    break;

                default:
                    throw std::invalid_argument("Side must be either long or short.");
            }
            return *this;
        }

        /**
         * @brief Sets the rate definition
         * @details interestRateIndex should be used instead
         * @param rateDef
         * @return MakeLeg&
         */
        MakeLeg& rateDefinition(const RateDefinition& rateDef) {
            rateDef_ = rateDef;
            return *this;
        }

        /**
         * @brief Flag to create a leg with redemptions.
         * @details some product might not need redemptions, for example a swap leg.
         * @param createRedemptions
         * @return MakeLeg&
         */
        MakeLeg& createRedemptions(bool createRedemptions) {
            createRedemptions_ = createRedemptions;
            return *this;
        }

        LegType<adouble> build() {
            setLegDates();
            setRedemptionNotionals();
            setCouponNotionals();
            setLegRedemptions();
            setLegCoupons();
            return leg_;
        }

       private:
        /**
         * @brief Set the Cashflow object with the correct currency and discount curve.
         *
         * @tparam C
         * @param cashflow
         */
        template <class C>
        void setCashflow(C& cashflow) {
            if (currency_ != Currency()) cashflow.currency(currency_);
            if (discountContextIdx_ != SIZE_MAX) cashflow.discountContextIdx(discountContextIdx_);
            if constexpr (std::is_same_v<FloatingRateCoupon<adouble>, C>) {
                if (indexContextIdx_ != SIZE_MAX) cashflow.indexContextIdx(indexContextIdx_);
            }
        }

        /**
         * @brief Set the dates_ member variable if it is empty.
         */
        void setLegDates() {
            if (dates_.empty()) {
                if (startDate_ != Date() || endDate_ != Date() || paymentFrequency_ != Frequency::NoFrequency) {
                    Schedule schedule = MakeSchedule()
                                            .from(startDate_)
                                            .to(endDate_)
                                            .withFrequency(paymentFrequency_)
                                            .withCalendar(calendar_)
                                            .withConvention(paymentConvention_);
                    dates_ = schedule.dates();
                } else {
                    throw std::invalid_argument("Dates vector is empty and startDate, endDate and paymentFrequency are not set.");
                }
            }
        }

        /**
         * @brief Set the redemptions_ vector.
         */
        void setRedemptionNotionals() {
            if (redemptions_.empty()) {
                if (notional_ <= 0) throw std::runtime_error("Notional not set.");
                redemptions_.resize(dates_.size() - 1, 0.0);
                redemptions_.back() = notional_ * side_;
            }
            if (redemptions_.size() != dates_.size() - 1) {
                throw std::invalid_argument("Redemptions vector size does not match dates vector size.");
            }
        }

        /**
         * @brief Sets the coupon notional vector.
         * @details Requires that the redemption_ vector is ready to be used.
         */
        void setCouponNotionals() {
            couponNotionals_.resize(dates_.size() - 1);
            couponNotionals_[0] = std::reduce(redemptions_.begin(), redemptions_.end());
            for (size_t i = 1; i < dates_.size() - 1; i++) { couponNotionals_[i] = couponNotionals_[i - 1] - redemptions_[i - 1]; }
        }

        /**
         * @brief Set the leg redemptions.
         * @details requires that the dates_ and redemptions_ vectors are ready to be consumed.
         */
        void setLegRedemptions() {
            if (createRedemptions_) {
                for (size_t i = 0; i < dates_.size() - 1; i++) {
                    if (redemptions_.at(i) != 0.0) {
                        Redemption<adouble> redemption(dates_.at(i + 1), redemptions_.at(i));
                        setCashflow(redemption);
                        leg_.addRedemption(redemption);
                    }
                }
            }
        }

        /**
         * @brief Set the leg coupons.
         */
        void setLegCoupons() {
            for (size_t i = 0; i < dates_.size() - 1; i++) {
                if constexpr (std::is_same_v<LegType<adouble>, FixedRateLeg<adouble>>) {
                    FixedRateCoupon<adouble> coupon(dates_.at(i), dates_.at(i + 1), couponNotionals_.at(i), rate_);
                    setCashflow(coupon);
                    leg_.addCoupon(coupon);
                } else if constexpr (std::is_same_v<LegType<adouble>, FloatingRateLeg<adouble>>) {
                    FloatingRateCoupon<adouble> coupon(dates_.at(i), dates_.at(i + 1), couponNotionals_.at(i), spread_, rateDef_);
                    setCashflow(coupon);
                    leg_.addCoupon(coupon);
                }
            }
        }

        LegType<adouble> leg_                    = LegType<adouble>();
        Date startDate_                          = Date();
        Date endDate_                            = Date();
        int side_                                = 1;
        Calendar calendar_                       = Calendar();
        Currency currency_                       = Currency();
        bool createRedemptions_                  = true;
        double notional_                         = -1;
        size_t indexContextIdx_                  = SIZE_MAX;
        size_t discountContextIdx_               = SIZE_MAX;
        BusinessDayConvention paymentConvention_ = BusinessDayConvention::Unadjusted;
        Frequency paymentFrequency_              = Frequency::NoFrequency;
        RateDefinition rateDef_;
        std::vector<Date> dates_;
        std::vector<double> redemptions_;
        std::vector<double> couponNotionals_;
        InterestRate<adouble> rate_;
        adouble spread_;
    };
}  // namespace Atlas

#endif /* F41148D8_C863_45B8_B7B2_E88435ADFC89 */
