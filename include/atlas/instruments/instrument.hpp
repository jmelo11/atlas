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

        const Leg& constLeg(size_t i) const { return legs_.at(i); };

        Leg& leg(size_t i) { return legs_[i]; };

       protected:
        std::vector<Leg> legs_;
    };
}  // namespace Atlas

#endif /* C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E */
