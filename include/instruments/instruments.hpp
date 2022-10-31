#pragma once

#include <instruments/leg.h>

namespace atlas {

	class Visitor;
	class ConstVisitor;
	
	class Instrument {
	public:
		
		virtual void accept(Visitor& visitor) = 0;
		virtual void accept(ConstVisitor& visitor) const = 0;

		const double faceAmount() const {
			return faceAmount_;
		}		
		
	protected:						
		double faceAmount_ = 0;
	};
	
	class SigleLegInstrument : public Instrument {
	public:
		SigleLegInstrument() {};
		SigleLegInstrument(Leg& leg) : leg_(leg) {};
		
		virtual bool isAlive(const Date& today) const {
			if (today >= leg_.startDate() && today < leg_.endDate())
				return true;
			return false;
		}
		
		void calculateOutstanding() {
			leg_.calculateOutstanding();
		}

		/* const */
		const Date& startDate() const {
			return leg_.startDate();
		}
		
		const Date& endDate() const {
			return leg_.endDate();
		}

		const std::vector<Date>& dates() const {
			return leg_.dates();
		}

		const std::vector<double>& constRedemptions() const {
			return leg_.constRedemptions();
		}

		const std::vector<double>& constOutstanding() const {
			return leg_.constOutstanding();
		}

		const std::vector<size_t>& constDFIdx() const {
			return leg_.constDFIdx();
		}

		const std::vector<size_t>& constFWDIdx() const {
			return leg_.constDFIdx();
		}

		std::vector<double>& outstanding() {
			return leg_.outstanding();
		}

		std::vector<double>& redemptions() {
			return leg_.redemptions();
		}
		
		std::vector<double>& interest() {
			return leg_.interest();
		}

		std::vector<size_t>& DFIdx() {
			return leg_.DFIdx();
		}
		

		std::vector<size_t>& FWDIdx() {
			return leg_.FWDIdx();
		}
	
		Leg& leg() {
			return leg_;
		}
			
		const Leg& constLeg() const {
			return leg_;
		}
		
		
	protected:
		 Leg leg_;
	};

	class MultiLegInstrument : public Instrument {
	public:
		virtual bool isAlive(const Date& today) const {
			if (today >= legs_[0].startDate() && today < legs_[0].endDate())
				return true;
			return false;
		}
		
		std::vector<Leg>& legs() {
			return legs_;
		}
		
		
		const std::vector<Leg>& constLegs() const {
			return legs_;
		}
		
		
	protected:
		std::vector<Leg> legs_;
	};
}