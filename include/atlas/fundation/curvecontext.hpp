#ifndef A3BCC3FF_7BC1_4BE8_94EE_D93451712071
#define A3BCC3FF_7BC1_4BE8_94EE_D93451712071

namespace Atlas {

    template <typename adouble>
    class YieldTermStructure;

    template <typename adouble>
    class YieldTermStructureManager;

    template <typename adouble>
    class InterestRateIndex;

    /**
     * @class CurveContext
     * @brief This class is a container for a curve, its index, its currency and its name.
     * @details This class can only be created by the YieldTermStructureManager class.
     *
     * @tparam adouble
     */
    template <typename adouble>
    class CurveContext {
       public:
        inline const std::string& name() const { return name_; }
        inline const YieldTermStructure<adouble>& curve() const { return curve_; }
        inline const InterestRateIndex<adouble>& index() const { return index_; }
        inline const Currency& currency() const { return currency_; }
        inline bool isRiskFree() const { return isRiskFree_; }
        inline size_t idx() const { return idx_; }

       private:
        CurveContext(size_t idx, const std::string& name, const YieldTermStructure<adouble>& curve, const InterestRateIndex<adouble>& index,
                     const Currency& currency, bool isRiskFree)
        : idx_(idx), name_(name), curve_(curve), index_(index), currency_(currency), isRiskFree_(isRiskFree) {}

        size_t idx_;
        std::string name_;
        YieldTermStructure<adouble> curve_;
        InterestRateIndex<adouble> index_;
        Currency currency_;
        bool isRiskFree_;

        friend class YieldTermStructureManager<adouble>;
    };
}  // namespace Atlas

#endif /* A3BCC3FF_7BC1_4BE8_94EE_D93451712071 */
