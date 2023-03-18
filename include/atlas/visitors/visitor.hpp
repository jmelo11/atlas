/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef C7EC2166_6C75_481E_8983_82B454069FCE
#define C7EC2166_6C75_481E_8983_82B454069FCE

#include <atlas/atlasconfig.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <iostream>

#define ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED std::cerr << "Warning: Visitor not implemented for " << __PRETTY_FUNCTION__ << std::endl;

namespace Atlas {

    class Visitor {
       public:
        virtual ~Visitor() = default;
        virtual void visit(FloatingRateInstrument& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
        virtual void visit(FixedRateInstrument& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
    };

    class ConstVisitor {
       public:
        virtual ~ConstVisitor() = default;
        virtual void visit(const FloatingRateInstrument& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
        virtual void visit(const FixedRateInstrument& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
    };
}  // namespace Atlas

#endif /* C7EC2166_6C75_481E_8983_82B454069FCE */
