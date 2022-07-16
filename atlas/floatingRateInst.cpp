#include "floatingRateInst.hpp"
#include "visitor.hpp"


namespace atlas {

	void FloatingBullet::accept(Visitor& visitor) {
		visitor.visit(*this);
	}

	void FloatingBullet::accept(ConstVisitor& visitor) const {
		visitor.visit(*this);
	}
}
