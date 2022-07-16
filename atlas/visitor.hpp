#pragma once
#include "fixedRateInst.hpp"
#include "floatingRateInst.hpp"

namespace atlas {
	class Visitor {
	public:
		virtual void visit(DAP& inst) = 0;
		virtual void visit(EqualPayment& inst) = 0;
		virtual void visit(FixedBullet& inst) = 0;
		virtual void visit(FloatingBullet& inst) = 0;
	};

	class ConstVisitor {
	public:
		virtual void visit(const DAP& inst) const = 0;
		virtual void visit(const EqualPayment& inst) const = 0;
		virtual void visit(const FixedBullet& inst) const = 0;
		virtual void visit(const FloatingBullet& inst) const = 0;
	};
}