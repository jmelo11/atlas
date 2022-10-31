#pragma once

#include <instruments/instruments.hpp>
#include <curves/rateindex.hpp>
#include <instruments/leg.h>

namespace atlas {

    /*
    * asume que la frecuencia de la tasa siempre es anual
    */

    class FloatingRateInstrument :
        public SigleLegInstrument
    {
    public:
        FloatingRateInstrument(double spread) :
			spread_(spread){};
                   
        const RateIndex& index(){
            return leg_.metaData().index_;
        }

        double spread() const {
            return spread_;
        }
        void spread(double s) {
            spread_ = s;
        }

    protected:
        double spread_ = 0;
    };

    class FloatingBullet : public virtual FloatingRateInstrument
    {
    public:
        FloatingBullet(const Date& start, const Date& end, double faceAmount, Frequency frequency, double spread, MetaData& metaData) :
            FloatingRateInstrument(spread) {
            faceAmount_ = faceAmount;
			Schedule schedule = makeSchedule().from(start).to(end).withFrequency(frequency);
            std::vector<double> redemptions(schedule.size() - 2, 0);
			redemptions.push_back(faceAmount);
			leg_ = Leg(schedule.dates(), redemptions);
            leg_.metaData(metaData);
            
        };      
        void accept(Visitor& visitor) override;
        void accept(ConstVisitor& visitor) const override;
    };

    class FloatingCustomStructure : public FloatingRateInstrument
    {
    public:
        FloatingCustomStructure(std::vector<Date> dates, std::vector<double> redemptions, double spread, MetaData& metaData) :
            FloatingRateInstrument(spread) {
            for (const auto r : redemptions)
                faceAmount_ += r;
            leg_ = Leg(dates, redemptions);
            leg_.metaData(metaData);
        };
		/*	    
		FloatingCustomStructure(std::vector<Date> dates, std::vector<double> redemptions, double spread) :
			FloatingCustomStructure(dates, redemptions, spread, std::make_shared<const MetaData>()) {}
		*/
        void accept(Visitor& visitor) override;
        void accept(ConstVisitor& visitor) const override;
    };
}