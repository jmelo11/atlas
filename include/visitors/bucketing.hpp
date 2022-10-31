#pragma once

#include <instruments/instruments.hpp>
#include <visitors/visitor.hpp>
#include <data/buckets.hpp>

namespace atlas {

	template <class T>
	class PaymentAtMaturity : public ConstVisitor {
	public:
		PaymentAtMaturity(std::vector<Profile<T>>& profiles) :profiles_(profiles) {};
		void visit(const DAP& dap) override {
			for (auto& profile : profiles_) {
				Date alive = profile.referenceDate();
				if (dap.isAlive(alive)) {
					for (auto& bucket : *profile.buckets()) {
						if (dap.leg().endDate() >= bucket.start_ && dap.leg().endDate() <= bucket.end_) {
							double v = dap.faceAmount();
							bucket.value_ += v;
						}
					}
				}
			}
		};
		std::vector<Profile<T>> profile() {
			return profiles_;
		}
	private:
		std::vector<Profile<T>>& profiles_;
	};

	class ConstantRollover : public ConstVisitor {
		void visit(const DAP& dap) {};
	};

}