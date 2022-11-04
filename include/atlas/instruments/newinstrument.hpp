#ifndef C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E
#define C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E

namespace Atlas {

    class Visitor;
    class ConstVisitor;

    class Instrument {
       public:
        virtual void accept(Visitor& visitor) = 0;

        virtual void accept(ConstVisitor& visitor) const = 0;
    };

#endif /* C7DE9C7B_D2B5_41EC_9FF7_36F8E938067E */
