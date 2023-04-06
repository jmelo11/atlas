/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef C7EC2166_6C75_481E_8983_82B454069FCE
#define C7EC2166_6C75_481E_8983_82B454069FCE

#include <atlas/atlasconfig.hpp>
#include <iostream>
#include <type_traits>

#define ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED std::cerr << "Warning: Visitor not implemented for " << __PRETTY_FUNCTION__ << std::endl;

namespace Atlas {

    template <typename adouble>
    class FloatingRateInstrument;

    template <typename adouble>
    class FixedRateInstrument;

    template <typename adouble>
    class Forward;

    template <typename adouble>
    class Visitor {
       public:
        virtual ~Visitor() = default;
        virtual void visit(FloatingRateInstrument<adouble>& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
        virtual void visit(FixedRateInstrument<adouble>& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
        virtual void visit(Forward<adouble>& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
    };

    template <typename adouble>
    class ConstVisitor {
       public:
        virtual ~ConstVisitor() = default;
        virtual void visit(const FloatingRateInstrument<adouble>& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
        virtual void visit(const FixedRateInstrument<adouble>& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
        virtual void visit(const Forward<adouble>& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};
    };

}  // namespace Atlas

#endif /* C7EC2166_6C75_481E_8983_82B454069FCE */
