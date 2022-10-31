#pragma once
#include <instruments/fixedRateInst.hpp>
#include <instruments/floatingRateInst.hpp>
#include <instruments/vanillaswap.h>

namespace atlas {
	class Visitor {
	public:
		virtual void visit(DAP& inst) = 0;
		virtual void visit(EqualPayment& inst) = 0;
		virtual void visit(FixedBullet& inst) = 0;
		virtual void visit(FloatingBullet& inst) = 0;
		virtual void visit(FloatingCustomStructure& inst) = 0;
		virtual void visit(FixedCustomStructure& inst) = 0;
		virtual void visit(VanillaSwap& inst) = 0;
	};

	class ConstVisitor {
	public:
		virtual void visit(const DAP& inst) const = 0;
		virtual void visit(const EqualPayment& inst) const = 0;
		virtual void visit(const FixedBullet& inst) const = 0;
		virtual void visit(const FloatingBullet& inst) const = 0;
		virtual void visit(const FloatingCustomStructure& inst) const = 0;
		virtual void visit(const FixedCustomStructure& inst) const = 0;
		virtual void visit(const VanillaSwap& inst) const = 0;
	};
}