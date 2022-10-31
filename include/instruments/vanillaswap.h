#pragma once
#include <instruments/instruments.hpp>

namespace atlas {
	
	class VanillaSwap : public MultiLegInstrument {
	public:
		enum Side {
			Payer,
			Receiver
		};
		
		VanillaSwap(Side side, const Date& startDate, const Date& endDate, const InterestRate& rate, Frequency fixLegFrequency, Frequency floatLegFrequency, double faceAmount, double spread, MetaData& metaData) :
			MultiLegInstrument(), spread_(spread), side_(side), rate_(rate) {
			faceAmount_ = faceAmount;

			Schedule fixSchedule = makeSchedule().from(startDate).to(endDate).withFrequency(fixLegFrequency);
			std::vector<double> fixRedemptions(fixSchedule.size() - 2, 0);
			fixRedemptions.push_back(faceAmount);
			
			Leg fixLeg(fixSchedule.dates(), fixRedemptions);
			fixLeg.metaData(metaData);
			

			Schedule floatSchedule = makeSchedule().from(startDate).to(endDate).withFrequency(floatLegFrequency);
			std::vector<double> floatRedemptions(floatSchedule.size() - 2, 0);
			floatRedemptions.push_back(faceAmount);
			
			Leg floatLeg(floatSchedule.dates(), floatRedemptions);
			floatLeg.metaData(metaData);
			
			legs_.push_back(fixLeg);
			legs_.push_back(floatLeg);						
		}

		

		Side side() const {
			return side_;
		}
		
		double spread() const {
			return spread_;
		}

		void rate(InterestRate& rate) {
			rate_ = rate;
		}
		
		const InterestRate& rate() const {
			return rate_;
		};

		void accept(Visitor& visitor) override;

		void accept(ConstVisitor& visitor) const override;
		
		
	private:
		InterestRate rate_;
		double spread_;
		Side side_;
	};
}