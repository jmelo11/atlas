#ifndef C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E
#define C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E

#include <atlas/cashflows/leg.hpp>  //fix

namespace Atlas {

    class Visitor;
    class ConstVisitor;

    class Instrument {
       public:
        virtual void accept(Visitor& visitor) = 0;

        virtual void accept(ConstVisitor& visitor) const = 0;

        Date startDate() const { return startDate_; };

        Date endDate() const { return endDate_; };

        double notional() const { return notional_; };

       protected:
        Date startDate_ = Date();
        Date endDate_   = Date();
        double notional_          = 0.0;
    };
}  // namespace Atlas

#endif /* C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E */
