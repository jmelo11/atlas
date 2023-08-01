#ifndef DA81CF99_5B14_4342_BCB0_5B49F140B61C
#define DA81CF99_5B14_4342_BCB0_5B49F140B61C

#include <atlas/fundation/curvecontext.hpp>
#include <atlas/others/auxfuncs.hpp>
#include <atlas/parsers/parsingmethods.hpp>
#include <atlas/rates/index/interestrateindex.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {

    /**
     * @class YieldTermStructureManager
     * @brief This class is responsable for storing all yield term structure related data, so processes relying on it can access it easely.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class YieldTermStructureManager {
       public:
        /**
         * @brief Construct a new Yield Term Structure Manager object
         *
         */
        YieldTermStructureManager() = default;

        /**
         * @brief Construct a new Yield Term Structure Manager object
         *
         * @param refDate reference date of the manager
         * @param localCurrency local currency of the manager
         */
        YieldTermStructureManager(const Date& refDate, const Currency& localCurrency) : refDate_(refDate), localCurrency_(localCurrency){};

        /**
         * @brief Adds a curve context to the manager
         *
         * @param name name of the curve context
         * @param curve curve of the curve context
         * @param index index of the curve context
         * @param ccy currency of the curve context
         * @param isRiskFree if the curve is risk free
         *
         * @return size_t
         */
        size_t addCurveContext(const std::string& name, const YieldTermStructure<adouble>& curve,
                               const InterestRateIndex<adouble>& index = InterestRateIndex<adouble>(), const Currency& ccy = Currency(),
                               bool isRiskFree = false) {
            if (hasCurveContext(name)) { throw std::invalid_argument("A curve CurveContext with the given name already exists."); }
            if (ccy == Currency() && isRiskFree) { throw std::invalid_argument("A risk free curve needs have a defined currency."); }
            if (hasRiskFreeCurve(ccy)) { throw std::invalid_argument("A risk free curve for the given currency already exists."); }
            contexts_.push_back(CurveContext<adouble>(contexts_.size(), name, curve, index, ccy, isRiskFree));
            return contexts_.size() - 1;
        }

        /**
         * @brief Gets the curve context with the given position
         *
         * @param idx position of the curve context
         */
        inline const CurveContext<adouble>& curveContext(size_t idx) const { return contexts_.at(idx); }

        /**
         * @brief Gets the curve context with the given name
         *
         * @param name name of the curve context
         */
        const CurveContext<adouble>& curveContext(const std::string& name) const {
            auto it =
                std::find_if(contexts_.begin(), contexts_.end(), [&name](const CurveContext<adouble>& context) { return context.name_ == name; });
            if (it == contexts_.end()) { throw std::invalid_argument("No curve CurveContext with the given name exists."); }
            return *it;
        }

        /**
         * @brief Checks if a curve context with the given name exists
         *
         * @param name
         * @return true
         * @return false
         */
        inline bool hasCurveContext(const std::string& name) const {
            return std::any_of(contexts_.begin(), contexts_.end(), [&name](const CurveContext<adouble>& context) { return context.name_ == name; });
        }

        /**
         * @brief Checks if the manager has a risk free curve for the given currency
         *
         * @param ccy
         * @return true
         * @return false
         */
        inline bool hasRiskFreeCurve(const Currency& ccy) const {
            return std::any_of(contexts_.begin(), contexts_.end(),
                               [&ccy](const CurveContext<adouble>& context) { return context.currency_ == ccy && context.isRiskFree_; });
        }

        /**
         * @brief Gets the risk free curve context for the given currency
         *
         * @param ccy currency of the curve context
         */
        size_t riskFreeCurveIdx(int ccyCode) const {
            ccyCode = codeMapping(ccyCode);
            auto it = std::find_if(contexts_.begin(), contexts_.end(), [&ccyCode](const CurveContext<adouble>& context) {
                if (context.currency_ == Currency()) return false;
                return context.currency_.numericCode() == ccyCode && context.isRiskFree_;
            });
            if (it == contexts_.end()) { throw std::invalid_argument("No curve CurveContext with the given name exists."); }
            const CurveContext<adouble>& context = *it;
            return context.idx();
        }

        /**
         * @brief Prints a summary of the manager
         *
         */
        void summary() {
            std::vector<std::vector<std::string>> tableData;
            tableData.push_back({"idx", "name", "refDate", "currency", "isRiskFree", "hasValidIndex"});

            for (const auto& context : contexts_) {
                std::string ccy        = context.currency_ != Currency() ? context.currency_.name() : "undefined";
                std::string isRiskFree = context.isRiskFree_ ? "true" : "false";
                std::string hasIndex   = context.index_.isValid() ? "true" : "false";
                tableData.push_back({std::to_string(context.idx_), context.name_, parseDate(context.curve_.refDate()), ccy, isRiskFree, hasIndex});
            }
            printTable(tableData);
        }

        /**
         * @brief Returns the reference date of the manager
         *
         * @return const Date&
         */
        inline const Date& refDate() const { return refDate_; }

        /**
         * @brief Return the local currency of the manager
         *
         */
        inline const Currency& localCurrency() const { return localCurrency_; }

       private:
        /**
         * @brief Helper function to map the code of the currency to the code of the risk free curve
         *
         * @param code
         * @return int
         */
        int codeMapping(int code) const { return code == 0 ? localCurrency_.numericCode() : code; }

        Date refDate_           = Date();
        Currency localCurrency_ = Currency();
        std::vector<CurveContext<adouble>> contexts_;
    };
}  // namespace Atlas

#endif /* DA81CF99_5B14_4342_BCB0_5B49F140B61C */
