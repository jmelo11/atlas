#ifndef C3DACA02_A13E_4C45_95EE_D1B496237D23
#define C3DACA02_A13E_4C45_95EE_D1B496237D23

#include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <type_traits>

namespace Atlas {

    template <typename adouble>
    class YieldTermStructure;

    /**
     * @class OneLegMixin
     * @brief A class for single leg instruments.
     *
     * @tparam adouble number type
     * @tparam FirstLeg<adouble> type of the first leg
     */
    template <template <typename> class FirstLeg, typename adouble = double>
    class OneLegMixin {
       public:
        OneLegMixin() = default;

        OneLegMixin(const FirstLeg<adouble>& leg) : leg_(leg){};

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return const FirstLeg<adouble>&
         */
        inline const FirstLeg<adouble>& leg() const { return leg_; }

        /**
         * @brief Returns the leg of the instrument.
         *
         * @return FirstLeg<adouble>&
         */
        inline FirstLeg<adouble>& leg() { return leg_; }

        /**
         * @brief Sets the discount curve context of the instrument.
         *
         * @param idx
         */
        inline void discountContextIdx(size_t idx) {
            if constexpr (std::is_same_v<FirstLeg<adouble>, FixedRateLeg<adouble>> || std::is_same_v<FirstLeg<adouble>, FloatingRateLeg<adouble>>) {
                for (auto& coupon : leg_.coupons()) coupon.discountContextIdx(idx);
            }
            for (auto& redemption : leg_.redemptions()) redemption.discountContextIdx(idx);
            disbursement_.discountContextIdx(idx);
        };

        /**
         * @brief Sets the rate index / forecast curve of the instrument.
         *
         * @param context
         */
        inline void indexContextIdx(size_t idx) {
            static_assert(std::is_same_v<FirstLeg<adouble>, FloatingRateLeg<adouble>>, "Only FloatingRateLeg is supported.");
            leg_.indexContextIdx(idx);
        }

        /**
         * @brief Returns the disbursement of the instrument.
         *
         * @return Cashflow
         */
        inline Cashflow<adouble>& disbursement() { return disbursement_; };

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

        /**
         * @brief Sets the currency of the instrument.
         *
         * @param currency
         */
        inline void currency(const Currency& currency) {
            if constexpr (std::is_same_v<FirstLeg<adouble>, FixedRateLeg<adouble>> || std::is_same_v<FirstLeg<adouble>, FloatingRateLeg<adouble>>) {
                for (auto& coupon : leg_.coupons()) coupon.currency(currency);
            }
            for (auto& redemption : leg_.redemptions()) redemption.currency(currency);
            disbursement_.currency(currency);
        }

        /**
         * @brief Flag to apply the currency of each cashflow.
         *
         * @param apply true to apply the currency of each cashflow
         */
        inline void applyCcy(bool apply) {
            if constexpr (std::is_same_v<FirstLeg<adouble>, FixedRateLeg<adouble>> || std::is_same_v<FirstLeg<adouble>, FloatingRateLeg<adouble>>) {
                for (auto& coupon : leg_.coupons()) coupon.applyCcy(apply);
            }
            for (auto& redemption : leg_.redemptions()) redemption.applyCcy(apply);
            disbursement_.applyCcy(apply);
        }

       protected:
        template <typename T = FirstLeg<adouble>, typename = std::enable_if_t<std::is_default_constructible_v<T>>>
        inline void reset() {
            leg_ = FirstLeg<adouble>();
        }

        Cashflow<adouble> disbursement_;
        FirstLeg<adouble> leg_;
    };
}  // namespace Atlas

#endif /* C3DACA02_A13E_4C45_95EE_D1B496237D23 */
