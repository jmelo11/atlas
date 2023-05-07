#ifndef C3DACA02_A13E_4C45_95EE_D1B496237D23
#define C3DACA02_A13E_4C45_95EE_D1B496237D23

#include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/fundation/context.hpp>

namespace Atlas {

    template <typename adouble>
    class YieldTermStructure;
    /**
     * @brief A class for single leg instruments.
     *
     * @tparam adouble number type
     * @tparam FirstLeg type of the first leg
     */
    template <typename adouble, class FirstLeg>
    class OneLegMixin {
       public:
        OneLegMixin() = default;

        OneLegMixin(const FirstLeg& leg) : leg_(leg){};

        inline const FirstLeg& leg() const { return leg_; }

        inline FirstLeg& leg() { return leg_; }

        /**
         * @brief Sets the discount curve context of the instrument.
         *
         * @param context
         */
        inline void discountCurveContext(const Context<YieldTermStructure<adouble>>& context) {
            if constexpr (std::is_same_v<FirstLeg, FixedRateLeg<adouble>> || std::is_same_v<FirstLeg, FloatingRateLeg<adouble>>) {
                for (auto& coupon : leg_.coupons()) coupon.discountCurveContext(context);
            }
            disbursement_.discountCurveContext(context);
        };

        /**
         * @brief Sets the rate index / forecast curve of the instrument.
         *
         * @param context
         */
        inline void forecastCurveContext(const Context<RateIndex<adouble>>& context) {
            static_assert(std::is_same_v<FirstLeg, FloatingRateLeg<adouble>>, "Only FloatingRateLeg is supported.");
            leg_.forecastCurveContext(context);
        }

        /**
         * @brief Returns the disbursement of the instrument.
         *
         * @return Cashflow
         */
        inline Cashflow<adouble>& disbursement() { return disbursement_; };

        /**
         * @brief Sets the disbursement of the instrument.
         *
         * @param disbursement Cashflow to be set
         */
        inline void disbursement(const Cashflow<adouble>& disbursement) { disbursement_ = disbursement; }

        /**
         * @brief Sets the currency of the instrument.
         *
         * @param currency
         */
        inline void currency(const Currency& currency) {
            if constexpr (std::is_same_v<FirstLeg, FixedRateLeg<adouble>> || std::is_same_v<FirstLeg, FloatingRateLeg<adouble>>) {
                for (auto& coupon : leg_.coupons()) coupon.currency(currency);
            }
            for (auto& redemption : leg_.redemptions()) redemption.currency(currency);
            disbursement_.currency(currency);
        }

       protected:
        Cashflow<adouble> disbursement_;
        FirstLeg leg_;
    };
}  // namespace Atlas

#endif /* C3DACA02_A13E_4C45_95EE_D1B496237D23 */
