#ifndef AE9605EA_FF75_4EC9_86EB_F69E772E237E
#define AE9605EA_FF75_4EC9_86EB_F69E772E237E

#include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <vector>

namespace Atlas {

    // #ifdef USE_STATIC_POLYMORPHISM
    //     // static polymorphism -> no virtual table but +mem/std::visit (slow)?
    //     template <typename adouble>
    //     using CouponVariant = std::variant<FixedRateCoupon<adouble>, FloatingRateCoupon<adouble>, Cashflow<adouble>>;
    //     using Cashflows     = std::vector<CouponVariant>;

    //     template <class CF>
    //     inline void addCashflow(Cashflows& cashflows, const CF& c) {
    //         cashflows.push_back(CouponVariant(c));
    //     }

    // #elif USE_DYNAMIC_POLYMORPHISM
    //     // dynamic polymorphism -> implied virtual table
    //     template <typename adouble>
    //     using Cashflows = std::vector<std::unique_ptr<Coupon<adouble>>>;

    //     template <class CF>
    //     inline void addCashflow(Cashflows& cashflows, const CF& c) {
    //         cashflows.push_back(std::make_unique<CF>(c));
    //     }
    // #else

    //     template <class CFs, class CF>
    //     inline void addCashflow(CFs& cashflows, const CF& c) {
    //         if constexpr (std::is_same_v<FixedRateCoupon, CF>) {
    //             cashflows.addFixedRateCoupon(c);
    //         } else if constexpr (std::is_same_v<FloatingRateCoupon, CF>) {
    //             cashflows.addFloatingRateCoupon(c);
    //         } else {
    //             cashflows.addCashflow(c);
    //         }
    //     }

    // #endif

    // enum class Cashflows { FixedRateCoupon, FloatingRateCoupon, Redemption, Disbursement };

    // template <typename adouble>
    // struct Coupons {
    //     Cashflows type_;
    //     union {
    //         FixedRateCoupon<adouble> fixedRateCoupon_;
    //         FloatingRateCoupon<adouble> floatingRateCoupon_;
    //         Cashflow<adouble> cashflow_;
    //     };

    //     Coupons(const FixedRateCoupon<adouble>& c) : type_(Cashflows::FixedRateCoupon), fixedRateCoupon_(c) {}

    //     Coupons(const FloatingRateCoupon<adouble>& c) : type_(Cashflows::FloatingRateCoupon), floatingRateCoupon_(c) {}

    //     Coupons(const Cashflow<adouble>& c, Cashflows type) : type_(type), cashflow_(c) {}
    // }; 


    // template <typename adouble>
    // Coupons Cashflows::operator()(const FixedRateCoupon<adouble>& c) const {
    //     return Coupons(c);
    // }

    // template <typename adouble>
    // Coupons Cashflows::operator()(const FloatingRateCoupon<adouble>& c) const {
    //     return Coupons(c);
    // }

    // template <typename adouble>
    // Coupons Cashflows::Redemption::operator()(const Cashflow<adouble>& c) const {
    //     return Coupons(c, Cashflows::Redemption);
    // }

    // template <typename adouble>
    // Coupons Cashflows::Disbursement::operator()(const Cashflow<adouble>& c) const {
    //     return Coupons(c, Cashflows::Disbursement);
    // }

}  // namespace Atlas

#endif /* AE9605EA_FF75_4EC9_86EB_F69E772E237E */
