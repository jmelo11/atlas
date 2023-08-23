#ifndef D5EF9CD0_8800_48B2_A34D_C398B2C5BF70
#define D5EF9CD0_8800_48B2_A34D_C398B2C5BF70

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponfixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/instruments/floatingrate/zerocouponfloatingrateinstrument.hpp>
#include <atlas/visitors/basevisitor.hpp>
#include <mutex>
#include <set>
#include <unordered_map>
namespace Atlas {

    /**
     * @class IndexingVisitor
     * @brief A class for indexing the cashflows of an instrument. Indexing is needed to compute all market variables ralated to the instrument's
     * cashflows.
     * @details It's a Visitor, so it updates the state of the given instrument with the relevant indices.
     * @ingroup Visitors
     *
     * @tparam adouble The number type used for the calculations. Can be double or an AAD type.
     */
    template <typename adouble = double>
    class IndexingVisitor : public BaseVisitor<adouble> {
       public:
        /**
         * @brief Construct a new Indexing Visitor object
         */
        IndexingVisitor(bool showLogs = false) : BaseVisitor<adouble>(showLogs){};

        void operator()(std::monostate& inst) override { this->template printLogs<IndexingVisitor>(this, "monostate"); }

        /**
         * @brief Indexes the cashflows of a CustomFixedRateInstrument.
         *
         * @param inst A CustomFixedRateInstrument.
         */
        void operator()(CustomFixedRateInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Indexes the cashflows of an EqualPaymentInstrument.
         *
         * @param inst An EqualPaymentInstrument.
         */
        void operator()(EqualPaymentInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Indexes the cashflows of a FixedRateBulletInstrument.
         *
         * @param inst A FixedRateBulletInstrument.
         */
        void operator()(FixedRateBulletInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Indexes the cashflows of a ZeroCouponFixedRateInstrument.
         *
         * @param inst A ZeroCouponFixedRateInstrument.
         */
        void operator()(ZeroCouponFixedRateInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Indexes the cashflows of a CustomFloatingRateInstrument.
         *
         * @param inst A CustomFloatingRateInstrument.
         */
        void operator()(CustomFloatingRateInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Indexes the cashflows of a FloatingRateBulletInstrument.
         *
         * @param inst A FloatingRateBulletInstrument.
         */
        void operator()(FloatingRateBulletInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Indexes the cashflows of a FloatingRateEqualRedemptionInstrument.
         *
         * @param inst A FloatingRateEqualRedemptionInstrument.
         */
        void operator()(FloatingRateEqualRedemptionInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Indexes the cashflows of a ZeroCouponFloatingRateInstrument.
         *
         */
        void operator()(ZeroCouponFloatingRateInstrument<adouble>& inst) override { indexCashflowStream(inst.cashflows()); }

        /**
         * @brief Get the Request object
         *
         * @return MarketRequest
         */

        MarketRequest getResults() const {
            MarketRequest request;
            request.dfs    = dfsVector_;
            request.fwds   = fwdsVector_;
            request.prices = pricesVector_;
            request.fxs    = fxPricesVector_;
            return request;
        };

        /**
         * @brief Clears the indexer.
         *
         */
        void reset() {
            dfsMap_.clear();
            dfsVector_.clear();
            fwdsMap_.clear();
            fwdsVector_.clear();
            pricesMap_.clear();
            pricesVector_.clear();
        };

       private:
        /**
         * @brief Indexes standard fixed income instrument.
         *
         * @param inst A fixed income instrument with floating rate leg.
         */
        template <class CS>
        void indexCashflowStream(CS& cashflows) {
            if constexpr (std::is_base_of_v<DisbursementStreamMixin<adouble>, CS>) {
                for (auto& cf : cashflows.disbursements()) {
                    indexDiscountFactor(cf);
                    indexExchangeRate(cf);
                }
            }
            if constexpr (std::is_base_of_v<RedemptionStreamMixin<adouble>, CS>) {
                for (auto& cf : cashflows.redemptions()) {
                    indexDiscountFactor(cf);
                    indexExchangeRate(cf);
                }
            }
            if constexpr (std::is_base_of_v<FixedRateCouponStreamMixin<adouble>, CS>) {
                for (auto& cf : cashflows.fixedRateCoupons()) {
                    indexDiscountFactor(cf);
                    indexForwardRate(cf);
                    indexExchangeRate(cf);
                }
            }
            if constexpr (std::is_base_of_v<FloatingRateCouponStreamMixin<adouble>, CS>) {
                for (auto& cf : cashflows.floatingRateCoupons()) {
                    indexDiscountFactor(cf);
                    indexForwardRate(cf);
                    indexExchangeRate(cf);
                }
            }
        };

        template <typename C>
        void indexDiscountFactor(C& cashflow) {
            if (!cashflow.hasDiscountContext()) { throw std::runtime_error("Cashflow does not have a discount curve context."); }

            size_t curveIdx         = cashflow.discountContextIdx();
            const Date& paymentDate = cashflow.paymentDate();

            std::lock_guard<std::mutex> lock(mtx_);
            evaluationDates_.insert(paymentDate);

            MarketRequest::DiscountFactor df(curveIdx, paymentDate);
            // If the discount factor has not been indexed, add it to the vector
            if (dfsMap_.find(df) == dfsMap_.end()) {
                dfsVector_.push_back(df);
                dfsMap_[df] = dfsVector_.size() - 1;
            }
            cashflow.dfIdx(dfsMap_[df]);
        };

        template <typename C>
        void indexForwardRate(C& cashflow) {
            if constexpr (std::is_same_v<C, FloatingRateCoupon<adouble>>) {
                if (!cashflow.hasIndexContextIdx()) { throw std::runtime_error("Cashflow does not have a forecast curve context."); }
                size_t curveIdx = cashflow.indexContextIdx();
                MarketRequest::ForwardRate fwd(cashflow.startDate(), cashflow.endDate(), curveIdx);
                if (fwdsMap_.find(fwd) == fwdsMap_.end()) {
                    fwdsVector_.push_back(fwd);
                    fwdsMap_[fwd] = fwdsVector_.size() - 1;
                }
                cashflow.fwdIdx(fwdsMap_[fwd]);
            }
        };

        template <typename C>
        void indexExchangeRate(C& cashflow) {
            Date fxDate;
            if (cashflow.applyCurrency()) {
                fxDate = cashflow.paymentDate();
            } else {
                fxDate = Date();
            }
            MarketRequest::ExchangeRate fx(cashflow.currencyCode(), 0, fxDate);  // cashflow ccy to local ccy

            if (fxPricesMap_.find(fx) == fxPricesMap_.end()) {
                fxPricesVector_.push_back(fx);
                fxPricesMap_[fx] = fxPricesVector_.size() - 1;
            }
            cashflow.fxIdx(fxPricesMap_[fx]);
            cashflow.isIndexed(true);
        };

        std::unordered_map<MarketRequest::ForwardRate, size_t> fwdsMap_;
        std::vector<MarketRequest::ForwardRate> fwdsVector_;

        std::unordered_map<MarketRequest::DiscountFactor, size_t> dfsMap_;
        std::vector<MarketRequest::DiscountFactor> dfsVector_;

        std::vector<MarketRequest::Price> pricesVector_;
        std::unordered_map<MarketRequest::Price, size_t> pricesMap_;

        std::vector<MarketRequest::ExchangeRate> fxPricesVector_;
        std::unordered_map<MarketRequest::ExchangeRate, size_t> fxPricesMap_;

        std::set<Date> evaluationDates_;
        std::mutex mtx_;
    };
}  // namespace Atlas

#endif /* D5EF9CD0_8800_48B2_A34D_C398B2C5BF70 */
