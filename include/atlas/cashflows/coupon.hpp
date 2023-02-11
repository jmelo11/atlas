#ifndef F0428475_403A_468B_857D_F05E5154A814
#define F0428475_403A_468B_857D_F05E5154A814

#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/cashflow.hpp>

namespace Atlas {

    /***
     * Coupon class
     *
     * A coupon is a cashflow that is paid periodically, and is based on a notional amount.
     */
    class Coupon : public Cashflow {
       public:
        /***
         * Constructor
         *
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         */
        Coupon(const Date& startDate, const Date& endDate, double notional);

        virtual ~Coupon(){};

        /***
         * Get the notional amount of the coupon
         * @return The notional amount of the coupon
         */
        double notional() const;

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
        double accruedPeriod(const Date& start, const Date& end) const;

        /***
         * Gets the accrued amount of the coupon
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued amount of the coupon
         */
        virtual double accruedAmount(const Date& start, const Date& end) const = 0;

        /***
         * Gets the accrual start date of the coupon
         * @return The accrual start date of the coupon
         */
        Date startDate() const { return startDate_; }

        /***
         * Gets the accrual end date of the coupon
         * @return The accrual end date of the coupon
         */
        Date endDate() const { return endDate_; }

       private:
        Date startDate_  = Date();
        Date endDate_    = Date();
        double notional_ = 0;
    };

}  // namespace Atlas

#endif /* F0428475_403A_468B_857D_F05E5154A814 */
