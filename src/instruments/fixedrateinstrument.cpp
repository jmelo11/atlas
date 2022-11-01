/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    void DAP::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void DAP::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

    void EqualPayment::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void EqualPayment::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

    void FixedBullet::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedBullet::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

    void FixedCustomStructure::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedCustomStructure::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }
}  // namespace atlas