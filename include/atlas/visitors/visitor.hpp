/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef C7EC2166_6C75_481E_8983_82B454069FCE
#define C7EC2166_6C75_481E_8983_82B454069FCE

#include <atlas/instruments/fixedrate/deposit.hpp>
#include <atlas/instruments/fixedrate/equalpaymentproduct.hpp>
#include <atlas/instruments/fixedrate/fixedbulletproduct.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletproduct.hpp>

namespace Atlas {

    class Visitor {
       public:
        virtual void visit(Deposit& inst)                   = 0;
        virtual void visit(FixedBulletProduct& inst)        = 0;
        virtual void visit(EqualPaymentProduct& inst)       = 0;
        virtual void visit(FloatingRateBulletProduct& inst) = 0;
    };

    class ConstVisitor {
       public:
        virtual void visit(const Deposit& inst) const                   = 0;
        virtual void visit(const FixedBulletProduct& inst) const        = 0;
        virtual void visit(const EqualPaymentProduct& inst) const       = 0;
        virtual void visit(const FloatingRateBulletProduct& inst) const = 0;
    };
}  // namespace Atlas

#endif /* C7EC2166_6C75_481E_8983_82B454069FCE */
