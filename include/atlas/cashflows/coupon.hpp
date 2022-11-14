#ifndef F0428475_403A_468B_857D_F05E5154A814
#define F0428475_403A_468B_857D_F05E5154A814

#include <ql/interestrate.hpp>
#include <ql/time/daycounter.hpp>
#include <atlas/cashflows/cashflow.hpp>

namespace Atlas {

    class Coupon : public Cashflow {
       public:
        Coupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double notional);

        double notional() const;

        virtual QuantLib::DayCounter dayCounter() const = 0;

        double accruedPeriod(const QuantLib::Date& start, const QuantLib::Date& end) const;

        virtual double accruedAmount(const QuantLib::Date& start,
                                     const QuantLib::Date& end) const = 0;

        QuantLib::Date startDate() const { return startDate_; }  // accrual start date?

        QuantLib::Date endDate() const { return endDate_; }  // accrual end date?

       private:
        QuantLib::Date startDate_ = QuantLib::Date();
        QuantLib::Date endDate_   = QuantLib::Date();
        double notional_          = 0;
    };

}  // namespace Atlas

#endif /* F0428475_403A_468B_857D_F05E5154A814 */
