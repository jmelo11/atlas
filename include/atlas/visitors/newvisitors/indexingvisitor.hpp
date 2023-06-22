#ifndef D5EF9CD0_8800_48B2_A34D_C398B2C5BF70
#define D5EF9CD0_8800_48B2_A34D_C398B2C5BF70

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/derivatives/fixfloatswap.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/newvisitors/basevisitor.hpp>
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
     * @tparam adouble
     */
    template <typename adouble>
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
        void operator()(CustomFixedRateInstrument<adouble>& inst) override { indexFixIncomeInstrument(inst); }

        /**
         * @brief Indexes the cashflows of an EqualPaymentInstrument.
         *
         * @param inst An EqualPaymentInstrument.
         */
        void operator()(EqualPaymentInstrument<adouble>& inst) override { indexFixIncomeInstrument(inst); }

        /**
         * @brief Indexes the cashflows of a FixedRateBulletInstrument.
         *
         * @param inst A FixedRateBulletInstrument.
         */
        void operator()(FixedRateBulletInstrument<adouble>& inst) override { indexFixIncomeInstrument(inst); }

        /**
         * @brief Indexes the cashflows of a ZeroCouponInstrument.
         *
         * @param inst A ZeroCouponInstrument.
         */
        void operator()(ZeroCouponInstrument<adouble>& inst) override { indexFixIncomeInstrument(inst); }

        /**
         * @brief Indexes the cashflows of a CustomFloatingRateInstrument.
         *
         * @param inst A CustomFloatingRateInstrument.
         */
        void operator()(CustomFloatingRateInstrument<adouble>& inst) override { indexFixIncomeInstrument(inst); }

        /**
         * @brief Indexes the cashflows of a FloatingRateBulletInstrument.
         *
         * @param inst A FloatingRateBulletInstrument.
         */
        void operator()(FloatingRateBulletInstrument<adouble>& inst) override { indexFixIncomeInstrument(inst); }

        /**
         * @brief Indexes the cashflows of a FloatingRateEqualRedemptionInstrument.
         *
         * @param inst A FloatingRateEqualRedemptionInstrument.
         */
        void operator()(FloatingRateEqualRedemptionInstrument<adouble>& inst) override { indexFixIncomeInstrument(inst); }

        /**
         * @brief Indexes the cashflows of an FX forward.
         *
         * @param inst The FX forward
         */
        void operator()(FxForward<adouble>& inst) override {
            auto& ccy1Cashflow = inst.leg().redemptions()[0];
            auto& ccy2Cashlfow = inst.leg().redemptions()[1];
            MarketRequest::ExchangeRate fwdPrice(ccy1Cashflow.currencyCode(), ccy2Cashlfow.currencyCode(), inst.endDate());
            MarketRequest::ExchangeRate spotPrice(ccy1Cashflow.currencyCode(), 0, Date());

            if (fxPricesMap_.find(fwdPrice) == fxPricesMap_.end()) {
                std::lock_guard<std::mutex> lock(mtx_);
                fxPricesVector_.push_back(fwdPrice);
                fxPricesMap_[fwdPrice] = fxPricesVector_.size() - 1;
            }
            if (fxPricesMap_.find(spotPrice) == fxPricesMap_.end()) {
                std::lock_guard<std::mutex> lock(mtx_);
                fxPricesVector_.push_back(spotPrice);
                fxPricesMap_[spotPrice] = fxPricesVector_.size() - 1;
            }
            ccy1Cashflow.fxIdx(fxPricesMap_.at(fwdPrice));
            ccy2Cashlfow.fxIdx(fxPricesMap_.at(spotPrice));
            indexCashflow(ccy1Cashflow);
            ccy2Cashlfow.isIndexed(true);
        };

        /**
         * @brief Indexes the cashflows of a fixed float swap.
         *
         * @param inst The fixed-float swap
         */
        void operator()(FixFloatSwap<adouble>& inst) override {
            auto& firstLeg  = inst.firstLeg();
            auto& secondLeg = inst.secondLeg();

            for (auto& coupon : firstLeg.coupons()) { indexCashflow(coupon); }
            for (auto& redemption : firstLeg.redemptions()) { indexCashflow(redemption); }

            for (auto& coupon : secondLeg.coupons()) { indexCashflow(coupon); }
            for (auto& redemption : secondLeg.redemptions()) { indexCashflow(redemption); }

            std::lock_guard<std::mutex> lock(mtx_);
            indexExchangeRate(firstLeg.coupons()[0], false);
        }

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
        template <typename I>
        void indexFixIncomeInstrument(I& inst) {
            indexCashflow(inst.disbursement());
            indexExchangeRate(inst.disbursement(), false);
            auto& leg         = inst.leg();
            auto& coupons     = leg.coupons();
            auto& redemptions = leg.redemptions();

            for (auto& coupon : coupons) { indexCashflow(coupon); }
            for (auto& redemption : redemptions) { indexCashflow(redemption); }
        };

        /**
         * @brief Helper generic function to index an cashflow (either fixed, floating or redemption) in the indexer. It
         * indexes the discount factor, the forward rate and the exchange rate.
         *
         * @tparam Flow A cashflow type.
         * @param cashflow A cashflow.
         */
        template <typename Flow>
        void indexCashflow(Flow& cashflow) {
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

            // fwd rate
            if constexpr (std::is_same_v<Flow, FloatingRateCoupon<adouble>>) {
                if (!cashflow.hasForecastContext()) { throw std::runtime_error("Cashflow does not have a forecast curve context."); }
                size_t curveIdx = cashflow.forecastContextIdx();
                MarketRequest::ForwardRate fwd(cashflow.startDate(), cashflow.endDate(), curveIdx);
                if (fwdsMap_.find(fwd) == fwdsMap_.end()) {
                    fwdsVector_.push_back(fwd);
                    fwdsMap_[fwd] = fwdsVector_.size() - 1;
                }
                cashflow.fwdIdx(fwdsMap_[fwd]);
            }

            // fx
            if (cashflow.applyCcy()) { indexExchangeRate(cashflow, true); }
            cashflow.isIndexed(true);
        };

        /**
         * @brief Indexes the exchange rate of a cashflow. The exchange rate can be set at the evaluation date, meaning that the rate will be
         * calculated each time the evaluation date is changed, or at the payment date.
         * @details The date at which the exchange rate is indexed is set by the atPaymentDate parameter. This might have an effect in the pricing
         * of the instrument.
         *
         * @param cashflow A cashflow.
         * @param atPaymentDate If true, the exchange rate is indexed at the payment date of the cashflow, otherwise is set to Date() (evaluation
         * date).
         */
        void indexExchangeRate(Cashflow<adouble>& cashflow, bool atPaymentDate = false) {
            Date fxDate;
            if (atPaymentDate) {
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
        }

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
