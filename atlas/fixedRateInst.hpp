#pragma once
#include "instruments.hpp"

namespace atlas {

	class FixedRateInstrument : public CashFlowInstrument {
	public:
		FixedRateInstrument(const Date& start, const Date& end, Frequency freq, double faceAmount, const InterestRate& rate) : CashFlowInstrument(start, end, freq, faceAmount), rate_(rate) {}
		FixedRateInstrument(const Date& start, const Date& end, Frequency freq, double faceAmount) : CashFlowInstrument(start, end, freq, faceAmount) {}
		FixedRateInstrument(const Date& start, const Date& end, Frequency freq) : CashFlowInstrument(start, end, freq) {}
		FixedRateInstrument(std::vector<Date>& dates) : CashFlowInstrument(dates) {}
		FixedRateInstrument() : CashFlowInstrument() {};

		void setRate(const InterestRate& r) {
			rate_ = r;
		}

		void setRate(double r) {
			rate_ = InterestRate(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
		}

		InterestRate rate() const {
			return rate_;
		}

		void fillInterest() {
			size_t periods = dates_.size() - 1;
			interest_ = std::vector<double>(periods, 0);
			if (outstanding_.empty() || outstanding_.size() != dates_.size()-1)
				calcOutstanding();
			for (size_t i = 0; i < periods; i++)
				interest_[i] = (rate_.compoundFactor(dates_[i], dates_[i + 1]) - 1) * outstanding_[i];
		}
	protected:
		InterestRate rate_;
	};

	class DAP : public FixedRateInstrument {
	public:
		DAP(const Date& start, const Date& end, double faceAmount, const InterestRate& rate) : FixedRateInstrument(start, end, Frequency::Once, faceAmount, rate) {
			redemptions_.push_back(faceAmount);
			interest_.push_back(accrual(start_, end_));
		}
		DAP(const Date& start, const Date& end, double faceAmount, double rate) : FixedRateInstrument(start, end, Frequency::Once, faceAmount) {
			rate_ = InterestRate(rate, Act360(), Compounding::Simple, Frequency::Annual);
			redemptions_.push_back(faceAmount);
			interest_.push_back(accrual(start_, end_));
		}

		double payment() const {
			return interest_.back() + redemptions_.back();
		}

		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;

	
	private:

		double accrual(const Date& a, const Date& b) const {
			return (rate_.compoundFactor(a, b) - 1) * faceAmount_;
		}

	};

	class EqualPayment : public FixedRateInstrument {
	public:
		EqualPayment(const Date& start, const Date& end, Frequency freq, double faceAmount, const InterestRate& rate) : FixedRateInstrument(start, end, freq, faceAmount, rate) {
			calculateNotionals();
		}
		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;

		void calculateNotionals() {
			size_t pN = dates_.size() - 1;
			size_t kN = pN + 1;
			Array factors(kN, 0), B(kN, 0), K;

			for (size_t i = 1; i <= pN; i++) {
				factors[i - 1] = (rate_.compoundFactor(dates_[i - 1], dates_[i]) - 1);
			}
			factors[kN - 1] = -1;
			B = -faceAmount_ * factors;
			Matrix A(kN, kN, 0);
			for (size_t j = 0; j < kN; j++) {
				for (size_t i = 0; i < kN; i++) {
					if (j == 0 && i < kN - 1) {
						A[i][j] = -1;
					}
					else if (j > 0 && i == j - 1) {
						A[i][j] = 1;
					}
					else if (j > 0 && j <= i && i < kN - 1) {
						A[i][j] = -factors[i];
					}
					else if (i == kN - 1 && j > 0) {
						A[i][j] = 1;
					}
				}
			}
			//slow
			K = inverse(A) * B;
			redemptions_.clear();
			redemptions_.insert(redemptions_.begin(), K.begin() + 1, K.end());
			interest_.resize(redemptions_.size());
			double totalK = 0;
			for (size_t i = 0; i < pN; i++)
			{
				interest_[i] = factors[i] * (faceAmount_ - totalK);
				totalK += redemptions_[i];
			}
		}
	};

	class FixedBullet : public FixedRateInstrument {

	public:

		FixedBullet(const Date& start, const Date& end, Frequency freq, double amount, const InterestRate& rate) : FixedRateInstrument(start, end, freq, amount, rate) {
			redemptions_ = std::vector<double>(dates_.size() - 1, 0);
			redemptions_.back() = faceAmount_;
			fillInterest();
		};

		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;
	};
}