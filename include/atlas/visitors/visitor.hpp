/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef C7EC2166_6C75_481E_8983_82B454069FCE
#define C7EC2166_6C75_481E_8983_82B454069FCE

#include <atlas/instruments/fixedrate/customfixedrateproduct.hpp>
#include <atlas/instruments/fixedrate/deposit.hpp>
#include <atlas/instruments/fixedrate/equalpaymentproduct.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletproduct.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptionproduct.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateproduct.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletproduct.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptionproduct.hpp>
#include <iostream>

#define ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED std::cerr << "Warning: Visitor not implemented for " << __PRETTY_FUNCTION__ << std::endl;

namespace Atlas {

    class Visitor {
       public:
        virtual ~Visitor()                                           = default;
        virtual void visit(Deposit& inst)                            = 0;
        virtual void visit(FixedRateBulletProduct& inst)             = 0;
        virtual void visit(EqualPaymentProduct& inst)                = 0;
        virtual void visit(FixedRateEqualRedemptionProduct& inst)    = 0;
        virtual void visit(FloatingRateBulletProduct& inst)          = 0;
        virtual void visit(FloatingRateEqualRedemptionProduct& inst) = 0;
        virtual void visit(CustomFixedRateProduct& inst)             = 0;
        virtual void visit(CustomFloatingRateProduct& inst)          = 0;
        virtual void visit(FixedRateInstrument& inst) { ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED };
        virtual void visit(EqualPaymentInstrument& inst) { ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED };

    };

    class ConstVisitor {
       public:
        virtual ~ConstVisitor()                                                  = default;
        virtual void visit(const Deposit& inst) const                            = 0;
        virtual void visit(const FixedRateBulletProduct& inst) const             = 0;
        virtual void visit(const EqualPaymentProduct& inst) const                = 0;
        virtual void visit(const FixedRateEqualRedemptionProduct& inst) const    = 0;
        virtual void visit(const FloatingRateBulletProduct& inst) const          = 0;
        virtual void visit(const FloatingRateEqualRedemptionProduct& inst) const = 0;
        virtual void visit(const CustomFixedRateProduct& inst) const             = 0;
        virtual void visit(const CustomFloatingRateProduct& inst) const          = 0;
        virtual void visit(const FixedRateInstrument& inst) const { ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED };
        virtual void visit(const EqualPaymentInstrument& inst) const { ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED };
    };
}  // namespace Atlas

#endif /* C7EC2166_6C75_481E_8983_82B454069FCE */
