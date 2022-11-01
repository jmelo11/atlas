/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#include <atlas/instruments/fixfloatswap.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    void FixFloatSwap::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixFloatSwap::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }
}  // namespace atlas
