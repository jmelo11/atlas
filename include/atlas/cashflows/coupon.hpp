#ifndef F0428475_403A_468B_857D_F05E5154A814
#define F0428475_403A_468B_857D_F05E5154A814

#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/cashflow.hpp>

namespace Atlas {

    /***
     * Coupon class
     *
     * @brief coupon is a cashflow that is paid periodically, and is based on a notional amount.
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
         * Constructor
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

        /***
         * Get the notional amount of the coupon
         * @return The notional amount of the coupon
         */
        inline double notional() const { return notional_; };

        /***
         * Gets the accrual start date of the coupon
         * @return The accrual start date of the coupon
         */
        inline Date startDate() const { return startDate_; }

        /***
         * Gets the accrual end date of the coupon
         * @return The accrual end date of the coupon
         */
        inline Date endDate() const { return endDate_; }

        /***
         * Gets the day counter of the coupon
         * @return The day counter of the coupon
         */
        virtual DayCounter dayCounter() const = 0;

        /***
         * Gets the accrued period of the coupon
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued period of the coupon
         */
        virtual double accruedPeriod(const Date& start, const Date& end) const = 0;

        /***
         * Gets the accrued amount of the coupon
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued amount of the coupon
         */
        virtual adouble accruedAmount(const Date& start, const Date& end) const = 0;

       private:
        Date startDate_  = Date();
        Date endDate_    = Date();
        double notional_ = 0;
    };

}  // namespace Atlas

#endif /* F0428475_403A_468B_857D_F05E5154A814 */
