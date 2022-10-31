#pragma once

#include <instruments/floatingRateInst.hpp>
#include <visitors/visitor.hpp>

namespace atlas {

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
}
