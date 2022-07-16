#pragma once
#include "buckets.hpp"


namespace atlas {

	class Visitor;
	class ConstVisitor;

	class Instrument {
	public:
		virtual void accept(Visitor& visitor) = 0;
		virtual void accept(ConstVisitor& visitor) const = 0;
	};
	
	class CashFlowInstrument : public Instrument {
	public:
		CashFlowInstrument(const Date& start, const Date& end, Frequency freq, double faceAmount) : start_(start), end_(end), freq_(freq), faceAmount_(faceAmount) {
			Schedule schedule = makeSchedule().from(start_).to(end_).withFrequency(freq_);
			dates_ = schedule.dates();
		};
		CashFlowInstrument(const Date& start, const Date& end, Frequency freq) : start_(start), end_(end), freq_(freq) {
			Schedule schedule = makeSchedule().from(start_).to(end_).withFrequency(freq_);
			dates_ = schedule.dates();
		};
		CashFlowInstrument(std::vector<Date>& dates_) : dates_(dates_) {}
		CashFlowInstrument() {};
		double amount() const {
			return faceAmount_;
		}

		bool isAlive(const Date& today) const {
			if (today >= start_ && today < end_)
				return true;
			return false;
		}
		
		double faceAmount() const {
			return faceAmount_;
		}

		Date startDate() const {
			return start_;
		}

		Date endDate() const {
			return end_;
		}
			
		//todo
		double nextCashflow(Date& t) const {
			return 0;
		}
		double lastCashflow(Date& t) const {
			return 0;
		}
		void calcOutstanding()  {
			outstanding_.clear();
			double amount = 0;
			for (auto m : redemptions_)
				amount += m;

			outstanding_.push_back(amount);
			for (auto m : redemptions_)
			{ 
				amount -= m;
				outstanding_.push_back(amount);
			}
			outstanding_.pop_back();
		}
		//non const methods
		std::vector<Date>* dates() {
			return &dates_;
		}

		std::vector<double>* outstanding() {
			return &outstanding_;
		}
		std::vector<size_t>* DFIdx() {
			return &DFIdx_;
		}

		std::vector<double>* interest() {
			return &interest_;
		}

		std::vector<double>* redemptions() {
			return &redemptions_;
		}

		
	protected:
		Date start_;
		Date end_;
		std::vector<Date> dates_;
		std::vector<double> interest_;
		std::vector<double> redemptions_;
		std::vector<double> outstanding_;
		Frequency freq_ = Frequency::NoFrequency;
		double faceAmount_ = 0;
		std::vector<size_t> DFIdx_;
	};
}