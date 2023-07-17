#ifndef F0428475_403A_468B_857D_F05E5154A814
#define F0428475_403A_468B_857D_F05E5154A814

#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/cashflow.hpp>

namespace Atlas {

    /**
     * @class Coupon
     * @brief Coupon is a cashflow that is paid periodically, and is based on a notional amount.
     * @ingroup Cashflows
     *
     * @tparam adouble The type of the floating point number used in the coupon
     */
    template <typename adouble>
    class Coupon : public Cashflow<adouble> {
       public:
        /**
         * @brief Construct a new Coupon object
         *
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         */
        Coupon(const Date& startDate, const Date& endDate, double notional)
        : Cashflow<adouble>(), startDate_(startDate), endDate_(endDate), notional_(notional) {
            this->paymentDate_ = endDate;  // paymentDate shouldnt be same as endDate
        };

        /**
         * @brief Construct a new Coupon object
         *
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param discountCurveContext The discount curve context of the coupon
         */
        Coupon(const Date& startDate, const Date& endDate, double notional, const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : Cashflow<adouble>(discountCurveContext), startDate_(startDate), endDate_(endDate), notional_(notional) {
            this->paymentDate_ = endDate;  // paymentDate shouldnt be same as endDate
        };

        virtual ~Coupon(){};

        /**
         * @brief Get the notional amount of the coupon
         *
         * @return The notional amount of the coupon
         */
        inline double notional() const { return notional_; };

        /**
         * @brief Gets the accrual start date of the coupon
         *
         * @return The accrual start date of the coupon
         */
        inline Date startDate() const { return startDate_; }

        /**
         * @brief Gets the accrual end date of the coupon
         *
         * @return The accrual end date of the coupon
         */
        inline Date endDate() const { return endDate_; }

        /**
         * @brief Gets the day counter of the coupon
         *
         * @return The day counter of the coupon
         */
        virtual DayCounter dayCounter() const = 0;

        /**
         * @brief Gets the accrual period of the coupon
         * @details Values are calculated considering the coupon dates. If dates are before or after the coupon dates, the accrued period is 0. In
         * other words, the accrued period is the period relevant for the given date interval given.
         *
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued period of the coupon
         */
        virtual double accruedPeriod(const Date& refStart, const Date& refEnd) const = 0;

        /**
         * @brief Gets the accrued amount of the coupon
         * @details Values are calculated considering the coupon dates. If dates are before or after the coupon dates, the accrued amount is 0. In
         * other words, the accrued amount is the amount relevant for the given date interval given.
         *
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued amount of the coupon
         */
        virtual adouble accruedAmount(const Date& refStart, const Date& refEnd) const = 0;

       protected:
        std::pair<Date, Date> accrualDates(const Date& refStart, const Date& refEnd) const {
            if (refStart > refEnd) throw std::invalid_argument("Start date must be before or equal to end date");
            Date startDate = std::max(refStart, this->startDate());
            Date endDate   = std::min(refEnd, this->endDate());
            return std::make_pair(startDate, endDate);
        }

       private:
        Date startDate_  = Date();
        Date endDate_    = Date();
        double notional_ = 0;
    };

    /**
     * @brief Possible optimization using CRTP
     * @details Breaks inheritance in Pybidn11.
     * @tparam adouble 
     * @tparam CouponType 
     */
    template <typename adouble, template <typename> class CouponType>
    class BaseCoupon : public Cashflow<adouble> {
       public:
        /**
         * @brief Construct a new Coupon object
         *
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         */
        BaseCoupon(const Date& startDate, const Date& endDate, double notional)
        : Cashflow<adouble>(), startDate_(startDate), endDate_(endDate), notional_(notional) {
            this->paymentDate_ = endDate;  // paymentDate shouldnt be same as endDate
        };

        /**
         * @brief Construct a new Coupon object
         *
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param discountCurveContext The discount curve context of the coupon
         */
        BaseCoupon(const Date& startDate, const Date& endDate, double notional, const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : Cashflow<adouble>(discountCurveContext), startDate_(startDate), endDate_(endDate), notional_(notional) {
            this->paymentDate_ = endDate;  // paymentDate shouldnt be same as endDate
        };

        /**
         * @brief Get the notional amount of the coupon
         *
         * @return The notional amount of the coupon
         */
        inline double notional() const { return notional_; };

        /**
         * @brief Gets the accrual start date of the coupon
         *
         * @return The accrual start date of the coupon
         */
        inline Date startDate() const { return startDate_; }

        /**
         * @brief Gets the accrual end date of the coupon
         *
         * @return The accrual end date of the coupon
         */
        inline Date endDate() const { return endDate_; }

        /**
         * @brief Gets the day counter of the coupon
         *
         * @return The day counter of the coupon
         */
        DayCounter dayCounter() const { return static_cast<const CouponType<adouble>&>(*this).dayCounter(); };

        /**
         * @brief Gets the accrual period of the coupon
         * @details Values are calculated considering the coupon dates. If dates are before or after the coupon dates, the accrued period is 0. In
         * other words, the accrued period is the period relevant for the given date interval given.
         *
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued period of the coupon
         */
        double accruedPeriod(const Date& refStart, const Date& refEnd) const {
            return static_cast<const CouponType<adouble>&>(*this).accruedPeriod(refStart, refEnd);
        };

        /**
         * @brief Gets the accrued amount of the coupon
         * @details Values are calculated considering the coupon dates. If dates are before or after the coupon dates, the accrued amount is 0. In
         * other words, the accrued amount is the amount relevant for the given date interval given.
         *
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued amount of the coupon
         */
        adouble accruedAmount(const Date& refStart, const Date& refEnd) const {
            return static_cast<const CouponType<adouble>&>(*this).accruedAmount(refStart, refEnd);
        };

       protected:
        std::pair<Date, Date> accrualDates(const Date& refStart, const Date& refEnd) const {
            if (refStart > refEnd) throw std::invalid_argument("Start date must be before or equal to end date");
            Date startDate = std::max(refStart, this->startDate());
            Date endDate   = std::min(refEnd, this->endDate());
            return std::make_pair(startDate, endDate);
        }

       private:
        Date startDate_  = Date();
        Date endDate_    = Date();
        double notional_ = 0;
    };
}  // namespace Atlas

#endif /* F0428475_403A_468B_857D_F05E5154A814 */
