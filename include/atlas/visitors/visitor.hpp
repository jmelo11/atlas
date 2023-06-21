/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef C7EC2166_6C75_481E_8983_82B454069FCE
#define C7EC2166_6C75_481E_8983_82B454069FCE

#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <iostream>
#include <mutex>
#include <type_traits>
#include <variant>

#define ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED std::cerr << "Warning: Visitor not implemented for " << __FUNCTION__ << std::endl;

namespace Atlas {

    template <typename adouble>
    class FloatingRateInstrument;

    template <typename adouble>
    class FixedRateInstrument;

    template <typename adouble>
    class FxForward;

    template <typename adouble>
    class FixFloatSwap;

    /**
     * @brief Visitor class
     * 
     * @tparam adouble 
     */
    template <typename adouble>
    class Visitor {
       public:
        virtual ~Visitor() = default;

        virtual void visit(FloatingRateInstrument<adouble>& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

        virtual void visit(FixedRateInstrument<adouble>& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

        virtual void visit(FxForward<adouble>& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

        virtual void visit(FixFloatSwap<adouble>& inst){ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

    };

    /**
     * @brief ConstVisitor class
     * 
     * @tparam adouble 
     */
    template <typename adouble>
    class ConstVisitor {
       public:
        virtual ~ConstVisitor() = default;

        virtual void visit(const FloatingRateInstrument<adouble>& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

        virtual void visit(const FixedRateInstrument<adouble>& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

        virtual void visit(const FxForward<adouble>& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

        virtual void visit(const FixFloatSwap<adouble>& inst) const {ATLAS_WARNING_VISITOR_NOT_IMPLEMENTED};

    };

}  // namespace Atlas

#endif /* C7EC2166_6C75_481E_8983_82B454069FCE */
