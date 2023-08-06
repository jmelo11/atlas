#ifndef F805FCBC_206D_43D0_9D24_1DDD7CC33163
#define F805FCBC_206D_43D0_9D24_1DDD7CC33163

#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>

namespace Atlas {

    /**
     * @class BaseCashflowStreamMixin
     * @brief Mixin class for cashflow streams
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class BaseCashflowStreamMixin {
       public:
        virtual ~BaseCashflowStreamMixin() = default;

        virtual void streamDiscountContextIdx(size_t idx)     = 0;
        virtual void streamIndexContextIdx(size_t idx)        = 0;
        virtual void streamCurrency(const Currency& currency) = 0;
        virtual void streamApplyCurrency(bool flag)           = 0;
        virtual void streamSort()                             = 0;

       protected:
        /**
         * @brief Index cashflows
         *
         * @tparam CashflowType
         * @param cf
         * @param idx
         */
        template <template <typename> class CashflowType>
        void indexCashflows(std::vector<CashflowType<adouble>>& cf, size_t idx, bool f) {
            if (f) {
                for (auto& c : cf) { c.discountContextIdx(idx); }
            } else {
                if constexpr (std::is_same_v<CashflowType<adouble>, FloatingRateCoupon<adouble>>) {
                    for (auto& c : cf) { c.indexContextIdx(idx); }
                }
            }
        }

        /**
         * @brief Sort cashflows
         *
         * @tparam CashflowType
         * @param cf
         */
        template <template <typename> class CashflowType>
        void sortCashflows(std::vector<CashflowType<adouble>>& cf) {
            std::sort(cf.begin(), cf.end(),
                      [](const CashflowType<adouble>& a, const CashflowType<adouble>& b) { return a.paymentDate() < b.paymentDate(); });
        }
    };

    /**
     * @class RedemptionStreamMixin
     * @brief Mixin class for redemptions streams
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class RedemptionStreamMixin : public BaseCashflowStreamMixin<adouble> {
       public:
        void streamDiscountContextIdx(size_t idx) override { this->indexCashflows(redemptions_, idx, true); };

        void streamIndexContextIdx(size_t idx) override{};

        void streamSort() override { this->sortCashflows(redemptions_); };

        void streamCurrency(const Currency& currency) override {
            for (auto& redemption : redemptions_) redemption.currency(currency);
        }

        void streamApplyCurrency(bool flag) override {
            for (auto& redemption : redemptions_) redemption.applyCurrency(flag);
        }

        void addRedemption(const Cashflow<adouble>& r) { redemptions_.push_back(r); }

        const Cashflow<adouble>& redemption(size_t idx) const { return redemptions_.at(idx); }

        Cashflow<adouble>& redemption(size_t idx) { return redemptions_[idx]; }

        const std::vector<Cashflow<adouble>>& redemptions() const { return redemptions_; }

        std::vector<Cashflow<adouble>>& redemptions() { return redemptions_; }

        size_t redemptionCount() const { return redemptions_.size(); }

       private:
        std::vector<Cashflow<adouble>> redemptions_;
    };

    /**
     * @class FixedRateCouponStreamMixin
     * @brief Mixin class for fixed rate coupon streams
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class FixedRateCouponStreamMixin : public BaseCashflowStreamMixin<adouble> {
       public:
        void streamDiscountContextIdx(size_t idx) override { this->indexCashflows(fixedRateCoupons_, idx, true); };

        void streamIndexContextIdx(size_t idx) override { this->indexCashflows(fixedRateCoupons_, idx, false); };

        void streamSort() override { this->sortCashflows(fixedRateCoupons_); };

        void streamCurrency(const Currency& currency) override {
            for (auto& coupon : fixedRateCoupons_) coupon.currency(currency);
        }

        void streamApplyCurrency(bool flag) override {
            for (auto& coupon : fixedRateCoupons_) coupon.applyCurrency(flag);
        }

        void addFixedRateCoupon(const FixedRateCoupon<adouble>& c) { fixedRateCoupons_.push_back(c); }

        const FixedRateCoupon<adouble>& fixedRateCoupon(size_t idx) const { return fixedRateCoupons_.at(idx); }

        FixedRateCoupon<adouble>& fixedRateCoupon(size_t idx) { return fixedRateCoupons_[idx]; }

        const std::vector<FixedRateCoupon<adouble>>& fixedRateCoupons() const { return fixedRateCoupons_; }

        std::vector<FixedRateCoupon<adouble>>& fixedRateCoupons() { return fixedRateCoupons_; }

        size_t fixedRateCouponCount() const { return fixedRateCoupons_.size(); }

        void rate(const InterestRate<adouble>& r) {
            for (auto& c : this->fixedRateCoupons_) { c.rate(r); }
        }

       private:
        std::vector<FixedRateCoupon<adouble>> fixedRateCoupons_;
    };

    /**
     * @class FloatingRateCouponStreamMixin
     * @brief Mixin class for floating rate coupon streams
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class FloatingRateCouponStreamMixin : public BaseCashflowStreamMixin<adouble> {
       public:
        void streamDiscountContextIdx(size_t idx) override { this->indexCashflows(floatingRateCoupons_, idx, true); };

        void streamIndexContextIdx(size_t idx) override { this->indexCashflows(floatingRateCoupons_, idx, false); };

        void streamSort() override { this->sortCashflows(floatingRateCoupons_); };

        void streamCurrency(const Currency& currency) override {
            for (auto& coupon : floatingRateCoupons_) coupon.currency(currency);
        }

        void streamApplyCurrency(bool flag) override {
            for (auto& coupon : floatingRateCoupons_) coupon.applyCurrency(flag);
        }

        void addFloatingRateCoupon(const FloatingRateCoupon<adouble>& c) { floatingRateCoupons_.push_back(c); }

        const FloatingRateCoupon<adouble>& floatingRateCoupon(size_t idx) const { return floatingRateCoupons_.at(idx); }

        FloatingRateCoupon<adouble>& floatingRateCoupon(size_t idx) { return floatingRateCoupons_[idx]; }

        const std::vector<FloatingRateCoupon<adouble>>& floatingRateCoupons() const { return floatingRateCoupons_; }

        std::vector<FloatingRateCoupon<adouble>>& floatingRateCoupons() { return floatingRateCoupons_; }

        size_t floatingRateCouponCount() const { return floatingRateCoupons_.size(); }

        void spread(adouble s) {
            for (auto& c : this->floatingRateCoupons_) { c.spread(s); }
        }

       private:
        std::vector<FloatingRateCoupon<adouble>> floatingRateCoupons_;
    };

    /**
     * @class DisbursementStreamMixin
     * @brief Mixin class for disbursement streams
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class DisbursementStreamMixin : public BaseCashflowStreamMixin<adouble> {
       public:
        void streamDiscountContextIdx(size_t idx) override { this->indexCashflows(disbursements_, idx, true); };

        void streamIndexContextIdx(size_t idx) override{};

        void streamSort() override { this->sortCashflows(disbursements_); };

        void streamCurrency(const Currency& currency) override {
            for (auto& disbursement : disbursements_) disbursement.currency(currency);
        }

        void streamApplyCurrency(bool flag) override {
            for (auto& disbursement : disbursements_) disbursement.applyCurrency(flag);
        }

        void addDisbursement(const Cashflow<adouble>& d) { disbursements_.push_back(d); }

        const Cashflow<adouble>& disbursement(size_t idx) const { return disbursements_.at(idx); }

        Cashflow<adouble>& disbursement(size_t idx) { return disbursements_[idx]; }

        const std::vector<Cashflow<adouble>>& disbursements() const { return disbursements_; }

        std::vector<Cashflow<adouble>>& disbursements() { return disbursements_; }

        size_t disbursementCount() const { return disbursements_.size(); }

       private:
        std::vector<Cashflow<adouble>> disbursements_;
    };

    /**
     * @class CashflowStream
     * @brief Holder class for cashflow streams
     *
     * @tparam adouble
     * @tparam T
     */
    template <typename adouble = double, template <typename> class... T>
    class CashflowStream : public T<adouble>... {
       public:
        /**
         * @brief sets the discount context index for all cashflows
         *
         * @param idx
         */
        void discountContextIdx(size_t idx) { (T<adouble>::streamDiscountContextIdx(idx), ...); };

        /**
         * @brief sets the index context index for all cashflows
         *
         * @param idx
         */
        void indexContextIdx(size_t idx) { (T<adouble>::streamIndexContextIdx(idx), ...); };

        /**
         * @brief sorts all cashflows
         *
         */
        void sort() { (T<adouble>::streamSort(), ...); };

        /**
         * @brief sets the currency for all cashflows
         *
         * @param currency
         */
        void currency(const Currency& currency) { (T<adouble>::streamCurrency(currency), ...); };

        /**
         * @brief sets the apply currency flag for all cashflows
         *
         * @param flag
         */
        void applyCurrency(bool flag) { (T<adouble>::streamApplyCurrency(flag), ...); };
    };

}  // namespace Atlas

#endif /* F805FCBC_206D_43D0_9D24_1DDD7CC33163 */
