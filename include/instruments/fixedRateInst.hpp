#pragma once

#include <instruments/instruments.hpp>

namespace atlas {

	class FixedRateInstrument : public SigleLegInstrument {
	public:
		FixedRateInstrument(const InterestRate& rate) : rate_(rate) {};
		
		void rate(const InterestRate& r) {
			rate_ = r;
		}

		const InterestRate& rate() const {
			return rate_;
		}

		void rate(double r) {
			rate_ = InterestRate(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
		}

		InterestRate& rate() {
			return rate_;
		}
	
		void calculateInterests() {			
			const auto& dates = leg().dates();
			size_t periods = dates.size();
			interest().clear();
			if (outstanding().empty() || outstanding().size() != periods)
				calculateOutstanding();
			for (size_t i = 1; i < periods; i++) {
				double amount = outstanding().at(i-1);
				double compFactor = rate_.compoundFactor(dates.at(i-1), dates.at(i));
				interest().push_back((compFactor - 1) * amount );			
			}			
		}
			
	protected:
		InterestRate rate_;		
	};

	class DAP : public FixedRateInstrument {
	public:
		DAP(const Date& start, const Date& end, double faceAmount, const InterestRate& rate, MetaData& metaData):
			FixedRateInstrument(rate) {
			faceAmount_ = faceAmount;
			
			leg_ = Leg( { start, end }, {faceAmount_}) ;
			leg_.metaData(metaData);
		}

		double payment() {		
			return interest().back() + redemptions().back();
		}

		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;
	
	private:

		double accrual(const Date& a, const Date& b) {
			return (rate_.compoundFactor(a, b) - 1) * redemptions().back();
		}
	};

	class EqualPayment : public FixedRateInstrument {
	public:
		
		EqualPayment(const Date& start, const Date& end, Frequency freq, double faceAmount, const InterestRate& rate, MetaData& metaData):
			FixedRateInstrument(rate) {
			faceAmount_ = faceAmount;
			Schedule schedule = makeSchedule().from(start).to(end).withFrequency(freq);
			std::vector<double> redemptions(schedule.size() - 1, 0);
			leg_ = Leg(schedule.dates(), redemptions);
			leg_.metaData(metaData);
			calculateNotionals();
		}
		/*
		EqualPayment(const Date& start, const Date& end, Frequency freq, double faceAmount, const InterestRate& rate) 
			: EqualPayment(start, end, freq, faceAmount, rate, std::make_shared<const MetaData>()) {
		}
		*/
		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;

		void calculateNotionals() {
			const auto& dates_ = dates();
			size_t pN = dates_.size() - 1;
			size_t kN = pN + 1;
			Array factors(kN, 0), B(kN, 0), K;

			for (size_t i = 1; i <= pN; i++) {
				factors[i - 1] = (rate_.compoundFactor(dates_.at(i - 1), dates_.at(i)) - 1);
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
			//slow, multicore?
			K = inverse(A) * B;
			redemptions().clear();
			interest().clear();
			
			redemptions().insert(redemptions().begin(), K.begin() + 1, K.end());
			
			//interest_.resize(redemptions_.size());
			double totalK = 0;
			for (size_t i = 0; i < pN; i++)
			{
				interest().push_back(factors[i] * (faceAmount_ - totalK));
				totalK += redemptions().at(i);
			}
		}

	};

	class FixedBullet : public FixedRateInstrument {

	public:

		FixedBullet(const Date& start, const Date& end, Frequency freq, double faceAmount, const InterestRate& rate, MetaData& metaData) : 
			FixedRateInstrument(rate){
			faceAmount_ = faceAmount;
			Schedule schedule = makeSchedule().from(start).to(end).withFrequency(freq);
			std::vector<double> redemptions(schedule.size() - 2, 0);
			redemptions.push_back(faceAmount_);
			leg_ = Leg(schedule.dates(), redemptions);
			leg_.metaData(metaData);
			//calculateInterests();
		};
		/*
		FixedBullet(const Date& start, const Date& end, Frequency freq, double amount, const InterestRate& rate):
			FixedBullet(start, end, freq, amount, rate, std::make_unique<const MetaData>()) {};
		*/
		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;

	};

	class FixedCustomStructure : public virtual FixedRateInstrument {
	public:
		FixedCustomStructure(std::vector<Date>& dates, std::vector<double>& redemptions, const InterestRate& rate, MetaData& metaData)
			: FixedRateInstrument(rate) {
			for (const auto r: redemptions)
				faceAmount_ += r;
			leg_ = Leg(dates, redemptions);
			leg_.metaData(metaData);
		};
		/*
		FixedCustomStructure(std::vector<Date>& dates, std::vector<double>& redemptions, const InterestRate& rate)
			: FixedCustomStructure(dates, redemptions, rate, std::make_unique<const MetaData>()) {};
		*/
		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;
	};
	
	
}