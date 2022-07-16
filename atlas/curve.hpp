#pragma once
#include "QL.hpp"

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
		virtual double forwardRate(double t1, double t2) const {
			double df1 = df(t1);
			double df2 = df(t2);
			return (df1 / df2 - 1) / (t2 - t1);
		}
		virtual double forwardRate(const Date& d1, const Date& d2) const {
			double t1 = dayCounter_.yearFraction(refDate_, d1);
			double t2 = dayCounter_.yearFraction(refDate_, d2);
			return forwardRate(t1, t2);
		}
		Date refDate() const {
			return refDate_;
		}
	protected:
		const Date refDate_;
		DayCounter dayCounter_;

	};


	template <class Interpolator = Linear>
	class ZeroRateCurve: public YieldCurve {
	public:
		ZeroRateCurve(const std::vector<Date>& dates, const std::vector<double>& rates, const DayCounter& dayCounter = Act360(),
			Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual, const Interpolator& interpolator = Interpolator()) :
			YieldCurve(dates[0], dayCounter), rates_(rates), comp_(comp), freq_(freq)
		{
			for (const Date& date : dates)
				times_.push_back(dayCounter.yearFraction(refDate_, date));
			interpol_ = interpolator.interpolate(times_.begin(), times_.end(), rates_.begin());
		}
		ZeroRateCurve(const Date& refDate, const std::vector<double>& times, const std::vector<double>& rates, const DayCounter& dayCounter = Act360(),
			Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual, const Interpolator& interpolator = Interpolator()) :
			YieldCurve(refDate, dayCounter), times_(times), rates_(rates), comp_(comp), freq_(freq)
		{
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
}