/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    void FloatingBullet::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FloatingBullet::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

    void FloatingCustomStructure::accept(Visitor& visitor) {
        visitor.visit(*this);
    }
    void FloatingCustomStructure::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }
}  // namespace atlas
