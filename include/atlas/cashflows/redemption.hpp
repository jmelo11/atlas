#ifndef A0BBBC2B_D8BB_4FFA_8288_2D14D574A14E
#define A0BBBC2B_D8BB_4FFA_8288_2D14D574A14E

#include <atlas/cashflows/cashflow.hpp>

namespace Atlas {

    class Redemption : public Cashflow {
       public:
        Redemption(const QuantLib::Date& date, double amount) : date_(date), amount_(amount){};

        QuantLib::Date date() const override;

        double amount() const override;

        bool hasOcurred(const QuantLib::Date& date) const override;

       private:
        QuantLib::Date date_ = QuantLib::Date();
        double amount_       = 0;
    };

}  // namespace Atlas

#endif /* A0BBBC2B_D8BB_4FFA_8288_2D14D574A14E */
