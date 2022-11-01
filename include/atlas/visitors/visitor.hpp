/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef C7EC2166_6C75_481E_8983_82B454069FCE
#define C7EC2166_6C75_481E_8983_82B454069FCE

#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/fixfloatswap.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {

    class Visitor {
       public:
        virtual void visit(FloatingBullet& inst)          = 0;
        virtual void visit(DAP& inst)                     = 0;
        virtual void visit(EqualPayment& inst)            = 0;
        virtual void visit(FixedBullet& inst)             = 0;
        virtual void visit(FloatingCustomStructure& inst) = 0;
        virtual void visit(FixedCustomStructure& inst)    = 0;
        virtual void visit(FixFloatSwap& inst)            = 0;
    };

    class ConstVisitor {
       public:
        virtual void visit(const DAP& inst) const                     = 0;
        virtual void visit(const EqualPayment& inst) const            = 0;
        virtual void visit(const FixedBullet& inst) const             = 0;
        virtual void visit(const FloatingBullet& inst) const          = 0;
        virtual void visit(const FloatingCustomStructure& inst) const = 0;
        virtual void visit(const FixedCustomStructure& inst) const    = 0;
        virtual void visit(const FixFloatSwap& inst) const            = 0;
    };
}  // namespace Atlas

#endif /* C7EC2166_6C75_481E_8983_82B454069FCE */
