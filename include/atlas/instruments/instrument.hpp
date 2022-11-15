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

        QuantLib::Date startDate() const { return startDate_; };

        QuantLib::Date endDate() const { return endDate_; };

        size_t dfIdx() const { return dfIdx_; };

        void dfIdx(size_t idx) { dfIdx_ = idx; };

        double notional() const { return notional_; };

       protected:
        QuantLib::Date startDate_ = QuantLib::Date();
        QuantLib::Date endDate_   = QuantLib::Date();
        size_t dfIdx_             = 0;
        double notional_          = 0.0;
    };
}  // namespace Atlas

#endif /* C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E */
