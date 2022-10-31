#pragma once

#include <others/qlconfig.hpp>

namespace atlas {
	
	class YieldCurve {
	public:
		YieldCurve(const DayCounter& dayCounter) : dayCounter_(dayCounter) {}
		YieldCurve(const Date& refDate, const DayCounter& dayCounter): refDate_(refDate), dayCounter_(dayCounter){}
		virtual double df(double time) const {
			return 0;
		};
		virtual double df(const Date& date) const {
			return 0;
		};

		virtual double forwardDf(double t1, double t2) {
			if (t1 > t2)
				throw std::runtime_error("t2 must be less than t1");
			return df(t2) / df(t1);
		}
		
		virtual double forwardDf(const Date& d1, const Date& d2) {
			if (d1 > d2) 				
				throw std::runtime_error("d2 must be less than d1");
			return df(d2) / df(d1);
		}
		
		virtual double forwardRate(double t1, double t2, Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual) const {
			double df1 = df(t1);
			double df2 = df(t2);			
			return InterestRate::impliedRate(df1 / df2, dayCounter_, comp, freq, t2 - t1).rate();
		}
		virtual double forwardRate(const Date& d1, const Date& d2, Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual) const {
			double t1 = dayCounter_.yearFraction(refDate_, d1);
			double t2 = dayCounter_.yearFraction(refDate_, d2);
			return forwardRate(t1, t2, comp, freq);
		}
		Date refDate() const {
			return refDate_;
		}
	protected:
		Date refDate_;
		DayCounter dayCounter_;

	};

	template <class Interpolator = Linear>
	class ZeroRateCurve: public YieldCurve {
	public:
		ZeroRateCurve(const std::vector<Date>& dates, const std::vector<double>& rates, const DayCounter& dayCounter = Act360(),
			Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual, const Interpolator& interpolator = Interpolator()) :
			YieldCurve(dates[0], dayCounter), comp_(comp), freq_(freq)
		{
			if (dates.size() != rates.size())
				throw std::runtime_error("dates and rates size do not match");
			
			std::vector<std::pair<Date, double>> nodes;
			for (size_t i = 0; i < dates.size(); i++)
				nodes.push_back({ dates[i], rates[i] });

			std::sort(nodes.begin(), nodes.end());
			std::vector<Date>nodeDates;
			std::vector<double>nodeRates;
			for (const auto& node : nodes) {
				nodeDates.push_back(node.first);
				nodeRates.push_back(node.second);
			}
			
			for (const Date& date : nodeDates)
				times_.push_back(dayCounter.yearFraction(refDate_, date));
			rates_ = nodeRates;
			
			interpol_ = interpolator.interpolate(times_.begin(), times_.end(), rates_.begin());
			interpol_.enableExtrapolation();
		}
		ZeroRateCurve(const Date& refDate, const std::vector<double>& times, const std::vector<double>& rates, const DayCounter& dayCounter = Act360(),
			Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual, const Interpolator& interpolator = Interpolator()) :
			YieldCurve(refDate, dayCounter), comp_(comp), freq_(freq)
		{
			if (times.size() != rates.size())
				throw std::runtime_error("dates and rates size do not match");

			std::vector<std::pair<double, double>> nodes;
			for (size_t i = 0; i < times.size(); i++)
				nodes.push_back({ times[i], rates[i] });

			std::sort(nodes.begin(), nodes.end());					
			for (const auto& node : nodes) {
				times_.push_back(node.first);
				rates_.push_back(node.second);
			}			
			interpol_ = interpolator.interpolate(times_.begin(), times_.end(), rates_.begin());
		}
		double df(double time) const override {
			if (time == 0)
				return 1;
			InterestRate r(interpol_(time), dayCounter_, comp_, freq_);
			return r.discountFactor(time);
		}
		double df(const Date& date) const override {
			double t = dayCounter_.yearFraction(refDate_, date);			
			return df(t);
		}
		
	private:
		Compounding comp_;
		Frequency freq_;
		std::vector<double> times_;
		std::vector<double> rates_;
		Interpolation interpol_;
	};

	template <class Interpolator = Linear>
	class DiscountCurve : public YieldCurve {
	public:
		
		DiscountCurve(const std::vector<Date>& dates, const std::vector<double>& dfs, const DayCounter& dayCounter = Act360(), const Interpolator& interpolator = Interpolator()) :
			YieldCurve(dayCounter) {
			
			if (dates.size()!=dfs.size())
				throw std::runtime_error("dates and dfs size do not match");
			
			std::vector<std::pair<Date, double>> nodes;
			for (size_t i = 0; i < dates.size(); i++) 
				nodes.push_back({ dates[i], dfs[i]});
			
			std::sort(nodes.begin(), nodes.end());
			std::vector<Date>nodeDates;
			std::vector<double>nodeDFs;
			for (const auto& node : nodes) {
				nodeDates.push_back(node.first);
				nodeDFs.push_back(node.second);
			}
			refDate_ = nodeDates[0];
			for (const Date& date : nodeDates)
				times_.push_back(dayCounter.yearFraction(refDate_, date));

			dfs_ = nodeDFs;
			interpol_ = interpolator.interpolate(times_.begin(), times_.end(), dfs_.begin());
			interpol_.enableExtrapolation();
		}
		DiscountCurve(const Date& refDate, const std::vector<double>& times, const std::vector<double>& dfs, const DayCounter& dayCounter = Act360(), const Interpolator& interpolator = Interpolator()) :
			YieldCurve(refDate, dayCounter){
			if (times.size() != dfs.size())
				throw std::runtime_error("times and dfs size do not match");

			std::vector<std::pair<double, double>> nodes;
			for (size_t i = 0; i < times.size(); i++)
				nodes.push_back({ times[i], dfs[i] });

			std::sort(nodes.begin(), nodes.end());
			
			for (const auto node : nodes) {
				times_.push_back(node.first);
				dfs_.push_back(node.second);
			}
			interpol_ = interpolator.interpolate(times_.begin(), times_.end(), dfs_.begin());
		}
		double df(double time) const override {
			if (time == 0)
				return 1;
			return interpol_(time);
		}
		double df(const Date& date) const override {
			double t = dayCounter_.yearFraction(refDate_, date);
			return df(t);
		}

	private:

		std::vector<double> times_;
		std::vector<double> dfs_;
		Interpolation interpol_;
	};
}