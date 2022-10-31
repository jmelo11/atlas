#pragma once

#include <instruments/vanillaswap.h>
#include <visitors/visitor.hpp>

namespace atlas {

	void VanillaSwap::accept(Visitor& visitor) {
		visitor.visit(*this);
	}

	void VanillaSwap::accept(ConstVisitor& visitor) const {
		visitor.visit(*this);
	}	
}
