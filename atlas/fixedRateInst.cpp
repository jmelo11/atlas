#include "fixedRateInst.hpp"
#include "visitor.hpp"

namespace atlas {
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
}