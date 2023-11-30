#ifndef BF3A9536_A414_4E35_85DC_4E82FB26AE67
#define BF3A9536_A414_4E35_85DC_4E82FB26AE67

#include <atlas/atlasconfig.hpp>
#include <iostream>
#include <variant>

namespace Atlas {

    // FixedIncome
    template <typename adouble>
    class CustomFixedRateInstrument;

    template <typename adouble>
    class EqualPaymentInstrument;

    template <typename adouble>
    class FixedRateBulletInstrument;

    template <typename adouble>
    class ZeroCouponFixedRateInstrument;

    template <typename adouble>
    class CustomFloatingRateInstrument;

    template <typename adouble>
    class FloatingRateBulletInstrument;

    template <typename adouble>
    class FloatingRateEqualRedemptionInstrument;

    template <typename adouble>
    class ZeroCouponFloatingRateInstrument;

    template <typename adouble>
    class FixedRateEqualRedemptionInstrument;

    /**
     * @brief Instruments is a std::variant of all the instruments in Atlas. It is used to implement the visitor pattern for instruments.
     * @tparam adouble
     */
    template <typename adouble = double>
    using InstrumentVariant =
        std::variant<std::monostate, CustomFixedRateInstrument<adouble>, EqualPaymentInstrument<adouble>, FixedRateBulletInstrument<adouble>,
                     ZeroCouponFixedRateInstrument<adouble>, CustomFloatingRateInstrument<adouble>, FloatingRateBulletInstrument<adouble>,
                     ZeroCouponFloatingRateInstrument<adouble>, FixedRateEqualRedemptionInstrument<adouble>>;

    /**
     * @defgroup Visitors Visitors
     */

    /**
     * @ingroup Visitors
     * @class BaseVisitor
     * @brief This class is used to implement the visitor pattern for instruments, using std::variant and std::visit.
     * It requires the user to implement the operator() for each childest-instrument type. Each implementation of this visitor could take an
     * instrument by reference and modify it.
     *
     * @tparam adouble The number type used for the calculations. Can be double or an AAD type.
     */
    template <typename adouble = double>
    class BaseVisitor {
       public:
        BaseVisitor(bool showLogs = false) : showLogs_(showLogs){};
        virtual ~BaseVisitor()                                                        = default;
        virtual void operator()(CustomFixedRateInstrument<adouble>& inst)             = 0;
        virtual void operator()(EqualPaymentInstrument<adouble>& inst)                = 0;
        virtual void operator()(FixedRateBulletInstrument<adouble>& inst)             = 0;
        virtual void operator()(FixedRateEqualRedemptionInstrument<adouble>& inst)    = 0;
        virtual void operator()(ZeroCouponFixedRateInstrument<adouble>& inst)         = 0;
        virtual void operator()(CustomFloatingRateInstrument<adouble>& inst)          = 0;
        virtual void operator()(FloatingRateBulletInstrument<adouble>& inst)          = 0;
        virtual void operator()(FloatingRateEqualRedemptionInstrument<adouble>& inst) = 0;
        virtual void operator()(ZeroCouponFloatingRateInstrument<adouble>& inst)      = 0;
        virtual void operator()(std::monostate& ms)                                   = 0;

       protected:
        template <typename T>
        void printLogs(const T* d, const std::string& message) const {
            if (showLogs_) { std::cout << typeid(*d).name() << ": " << message << std::endl; }
        };
        bool showLogs() const { return showLogs_; };
        void showLogs(bool showLogs) { showLogs_ = showLogs; };

       private:
        bool showLogs_;
    };

    /**
     * @ingroup Visitors
     * @class BaseConstVisitor
     * @brief This class is used to implement the visitor pattern for instruments, using std::variant and std::visit. It
     * requires the user to implement the operator() for each childest-instrument type. Different from BaseVisitor, this class takes an
     * instrument by const reference and can not modify it.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class BaseConstVisitor {
       public:
        BaseConstVisitor(bool showLogs = false) : showLogs_(showLogs){};
        virtual ~BaseConstVisitor()                                                               = default;
        virtual void operator()(const CustomFixedRateInstrument<adouble>& inst) const             = 0;
        virtual void operator()(const EqualPaymentInstrument<adouble>& inst) const                = 0;
        virtual void operator()(const FixedRateBulletInstrument<adouble>& inst) const             = 0;
        virtual void operator()(const FixedRateEqualRedemptionInstrument<adouble>& inst) const    = 0;
        virtual void operator()(const ZeroCouponFixedRateInstrument<adouble>& inst) const         = 0;
        virtual void operator()(const CustomFloatingRateInstrument<adouble>& inst) const          = 0;
        virtual void operator()(const FloatingRateBulletInstrument<adouble>& inst) const          = 0;
        virtual void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& inst) const = 0;
        virtual void operator()(const ZeroCouponFloatingRateInstrument<adouble>& inst) const      = 0;
        virtual void operator()(const std::monostate& ms) const                                   = 0;

       protected:
        template <typename T>
        void printLogs(const T* d, const std::string& message) const {
            if (showLogs_) { std::cout << typeid(*d).name() << ": " << message << std::endl; }
        };
        bool showLogs() const { return showLogs_; };
        void showLogs(bool showLogs) { showLogs_ = showLogs; };

       private:
        bool showLogs_;
    };

}  // namespace Atlas

#endif /* BF3A9536_A414_4E35_85DC_4E82FB26AE67 */
